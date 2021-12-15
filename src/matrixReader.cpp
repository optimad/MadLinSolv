/*---------------------------------------------------------------------------*\
 *
 *  MadLinSolv
 *
  *  -------------------------------------------------------------------------
 *  License
 *  This file is part of MadLinSolv.
 *
 *  MadLinSolv is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  MadLinSolv is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with MadLinSolv. If not, see <http://www.gnu.org/licenses/>.
 *
 \*---------------------------------------------------------------------------*/

#include <bitpit_IO.hpp>

#include "matrixReader.hpp"


using namespace bitpit;


/*!
 * Constructor
 * It sets m_nProcessors and m_rank to values passed from the caller
 * File_Handler is default constructed. Row, cols and non-zeros members are set to zero.
 * \param[in] nProcessors number of MPI processes
 * \param[in] rank process MPI rank
 */
MatrixReader::MatrixReader(int nProcessors, int rank) :
        m_nProcessors(nProcessors), m_rank(rank),m_fileHandler(),m_nRows(0),m_nCols(0),m_nNz(0)
{

}

/*!
 * Constructor
 * It sets m_nProcessors and m_rank to values passed from the caller
 * File_Handler is constructed with folder, file name and extension from the caller.
 * Row, cols and non-zeros members are set to zero.
 * \param[in] nProcessors number of MPI processes
 * \param[in] rank process MPI rank
 * \param[in] dir_ matrix folder name
 * \param[in] name_ matrix file name
 * \param[in] app_ matrix file extension
 */
MatrixReader::MatrixReader(int nProcessors, int rank,const std::string & dir_, const std::string & name_, const std::string & app_) :
        m_nProcessors(nProcessors), m_rank(rank), m_fileHandler(dir_,name_,app_),m_nRows(0),m_nCols(0),m_nNz(0)
{

}

/*!
 * It reads the matrix from disk, populates and assemblies bitpit SparseMatrix objects.
 * \param[in] system a reference to the unique pointer to the bitpit SparseMatrix to be filled
 */
void MatrixReader::readMatrixCSRFormat(std::unique_ptr<SparseMatrix> & matrix)
{
    log::cout() << "Matrix path: " << m_fileHandler.getPath() << std::endl;
    std::fstream inMatrix(m_fileHandler.getPath().c_str(), std::ifstream::in);
    if(inMatrix.is_open()) {
        readMatrixCSRFormatInfo(inMatrix);

        std::vector<int> procRows = computeLinesPerProc();
        log::cout() << "lines per proc = " << procRows << std::endl;
        std::vector<int> startRows = computeStartLinePerProc(procRows);
        log::cout() << "start per proc = " << startRows << std::endl;

        //Initialize matrix
#if ENABLE_MPI == 1
        matrix = std::unique_ptr<SparseMatrix>(new SparseMatrix(MPI_COMM_WORLD,true,procRows[m_rank],procRows[m_rank],m_nNz/m_nProcessors));
#else
        matrix = std::unique_ptr<SparseMatrix>(new SparseMatrix(procRows[m_rank],procRows[m_rank],m_nNz/m_nProcessors));
#endif

        readMatrixCSRFormatMatrix(inMatrix, procRows,startRows,matrix);

        inMatrix.close();
    } else {
        log::cout() << "File " << m_fileHandler.getPath() << " not open!" << std::endl;
    }
#if ENABLE_MPI==1
    MPI_Barrier(MPI_COMM_WORLD);
#endif

    matrix->assembly();

}

/*!
 * It reads the matrix header from file,
 * \param fileStream the stream from the matrix file
 */
void MatrixReader::readMatrixCSRFormatInfo(std::fstream & fileStream)
{

    std::string line;
    std::getline(fileStream,line);
    while(std::getline(fileStream,line)) {
        line = utils::string::trim(line);
        if(line.substr(0,1) != "#") {
            std::stringstream ss(line);
            ss >> m_nRows;
            ss >> m_nCols;
            ss >> m_nNz;
            break;
        }
    }
    log::cout() << "nRows = " << m_nRows << std::endl;
    log::cout() << "nCols = " << m_nCols << std::endl;
    log::cout() << "nNz = " << m_nNz << std::endl;
}

/*!
 * It reads the body of the matrix file populating the bitpit SparseMatrix object.
 * \param[in] fileStream the stream from the input initial solution file
 * \param[in] procLines a vector of m_nProcessors elements containing the number of file lines for each process
 * \param[in] startLines a vector of m_nProcessors elements containing the line number which each process starts reading at
 * \param[in] system a reference to the unique pointer to the system which the user wants to fill the solution in
 */
void MatrixReader::readMatrixCSRFormatMatrix(std::fstream & fileStream, const std::vector<int> & procLines,
        const std::vector<int> & startLines, std::unique_ptr<SparseMatrix> & matrix)
{
    std::string line;
    //jump lines before rank lines
    for(int l = 0; l < startLines[m_rank]; ++l) {
        std::getline(fileStream,line);
    }
    //read rank lines
    std::vector<long> rowPattern;
    std::vector<double> rowValues;
    rowPattern.reserve(100);
    rowValues.reserve(100);
    for(int l = 0; l < procLines[m_rank]; ++l) {
        genericIO::lineStream(fileStream, rowPattern);
        genericIO::lineStream(fileStream, rowValues);
        log::cout() << "pattern " << rowPattern << std::endl;
        log::cout() << "values " << rowValues << std::endl;
        matrix->addRow(rowPattern,rowValues);
        rowPattern.clear();
        rowValues.clear();
    }
}

/*!
 * It computes the line number which each process has to start reading at into the matrix file
 * \param[in] procRows a vector of m_nProcessors elements containing the number of file lines for each process
 * \return a vector of m_nProcessors elements containing the line number which each process starts reading at
 */
std::vector<int> MatrixReader::computeLinesPerProc()
{
    std::vector<int> rows(m_nProcessors,0);

    int division = m_nRows / m_nProcessors;
    int reminder = m_nRows % m_nProcessors;
    log::cout() << "division " << division << std::endl;
    log::cout() << "reminder " << reminder << std::endl;
    for(int & stride : rows) {
        stride = division;
        if(reminder > 0){
            stride += 1;
            --reminder;
        }
    }
    return rows;
}

/*!
 * It gets the global number of matrix rows as read in the matrix file header
 * @return the global number of matrix rows as read in the matrix file header
 */
int MatrixReader::getNRows()
{
    return m_nRows;
}

/*!
 * It computes the line number which each process has to start reading at into the matrix file
 * \param[in] procRows a vector of m_nProcessors elements containing the number of file lines for each process
 * \return a vector of m_nProcessors elements containing the line number which each process starts reading at
 */
std::vector<int> MatrixReader::computeStartLinePerProc(const std::vector<int> & procRows)
{
    std::vector<int> startLines(m_nProcessors,0);

    for(int p = 1; p < m_nProcessors; ++p) {
        for(int pp = 0; pp < p; ++pp) {
            startLines[p] += 2 * procRows[pp];
        }
    }

    return startLines;
}

/*!
 * It sets the global number of rows
 * \param[in] nRows the global number of rows
 */
void MatrixReader::setNRows(int nRows)
{
    m_nRows = nRows;
}

/*!
 * It sets the global number of columns
 * \param[in] nRows the global number of columns
 */
void MatrixReader::setNCols(int nCols)
{
    m_nCols = nCols;
}

/*!
 * It sets the global number of non-zeros
 * \param[in] nRows the global number of non-zeros
 */
void MatrixReader::setNNz(int nNz)
{
    m_nNz = nNz;
}

/*!
 * It sets the matrix folder name into the file handler
 * @param dir matrix folder name
 */
void MatrixReader::setDirectory(const std::string& dir)
{
    m_fileHandler.setDirectory(dir);
}

/*!
 * It sets the matrix file name into the file handler
 * @param dir matrix file name
 */
void MatrixReader::setName(const std::string& name)
{
    m_fileHandler.setName(name);
}

/*!
 * It sets the matrix file extension into the file handler
 * @param dir matrix file extension
 */
void MatrixReader::setAppendix(const std::string& app)
{
    m_fileHandler.setAppendix(app);
}

/*!
 * It gets the matrix file path from the file handler
 * @return a string containing the file path
 */
std::string MatrixReader::getPath()
{
    return m_fileHandler.getPath();
}

/*!
 * It gets the matrix folder name from the file handler
 * @return a string containing the folder name
 */
std::string MatrixReader::getDirectory()
{
    return m_fileHandler.getDirectory();
}

/*!
 * It gets the matrix file name from the file handler
 * @return a string containing the file name
 */
std::string MatrixReader::getName()
{
    return m_fileHandler.getName();
}

/*!
 * It gets the matrix file name from the file handler
 * @return a string containing the file name
 */
std::string MatrixReader::getAppendix()
{
    return m_fileHandler.getAppendix();
}
