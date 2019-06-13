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
#include <bitpit_LA.hpp>

#include <initialSolutionReader.hpp>


using namespace bitpit;

/*!
 * Constructor
 * It sets m_nProcessors and m_rank to values passed from the caller
 * File_Handler is default constructed. Row member is set to zero.
 * \param[in] nProcessors number of MPI processes
 * \param[in] rank process MPI rank
 */
InitialSolutionReader::InitialSolutionReader(int nProcessors, int rank) :
                        m_nProcessors(nProcessors), m_rank(rank),m_fileHandler(),m_nRows(0)
{

}

/*!
 * Constructor
 * It sets m_nProcessors and m_rank to values passed from the caller
 * File_Handler is constructed with folder, file name and extension from the caller.
 * Row member is set to zero.
 * \param[in] nProcessors number of MPI processes
 * \param[in] rank process MPI rank
 * \param[in] dir_ initial solution folder name
 * \param[in] name_ initial solution file name
 * \param[in] app_ initial solution file extension
 */
InitialSolutionReader::InitialSolutionReader(int nProcessors, int rank,
        const std::string& dir_, const std::string& name_,
        const std::string& app_) :
                        m_nProcessors(nProcessors), m_rank(rank), m_fileHandler(dir_,name_,app_),m_nRows(0)
{

}

/*!
 * It reads the initial solution guess from disk and sets values in system solution container,
 * checking if number of elements in file header is equal to the expected one.
 * \param[in] system a reference to the unique pointer to the system which the user wants to fill the solution in
 * \param[in] expectedElements number of elements the user expects in the file (header number of elements)
 */
void InitialSolutionReader::read(std::unique_ptr<SystemSolver> & system, int expectedElements)
{

    log::cout() << "InitialSolution path: " << m_fileHandler.getPath() << std::endl;
    std::fstream inInitialSolution(m_fileHandler.getPath().c_str(), ifstream::in);
    if(inInitialSolution.is_open()) {
        readInfo(inInitialSolution,expectedElements);

        std::vector<int> procRows = computeLinesPerProc();
        log::cout() << "InitialSolution lines per proc = " << procRows << std::endl;
        std::vector<int> startRows = computeStartLinePerProc(procRows);
        log::cout() << "InitialSolution start per proc = " << startRows << std::endl;

        readInitialSolution(inInitialSolution, procRows,startRows, system);

        inInitialSolution.close();
    } else {
        log::cout() << "File " << m_fileHandler.getPath() << " not open!" << std::endl;
    }
#if ENABLE_MPI==1
    MPI_Barrier(MPI_COMM_WORLD);
#endif


}

/*!
 * It reads the initial solution guess header from file,
 * checking if number of elements in file header is equal to the expected one.
 * \param fileStream the stream from the input initial solution file
 * \param[in] expectedElements number of elements the user expects in the file (header number of elements)
 */
void InitialSolutionReader::readInfo(std::fstream & fileStream, int expectedElements)
{
    std::string line;
    std::getline(fileStream,line);
    while(std::getline(fileStream,line)) {
        line = utils::string::trim(line);
        if(line.substr(0,1) != "#") {
            std::stringstream ss(line);
            ss >> m_nRows;
            break;
        }
    }
    log::cout() << "nRows = " << m_nRows << std::endl;

    if(expectedElements != m_nRows) {
        log::cout() << "InitialSolution and matrix have different number of rows. Please, check file " << m_fileHandler.getPath() << std::endl;
#if ENABLE_MPI == 1
        MPI_Finalize();
#endif
        exit(1);
    }
}

/*!
 * It reads the body of the initial solution file setting values in system solution container,
 * \param[in] fileStream the stream from the input initial solution file
 * \param[in] procLines a vector of m_nProcessors elements containing the number of file lines for each process
 * \param[in] startLines a vector of m_nProcessors elements containing the line number which each process starts reading at
 * \param[in] system a reference to the unique pointer to the system which the user wants to fill the solution in
 */
void InitialSolutionReader::readInitialSolution(std::fstream & fileStream, const std::vector<int> & procLines, const std::vector<int> & startLines,
        std::unique_ptr<SystemSolver>& system) {

    std::string line;
    //jump lines before rank lines
    for(int l = 0; l < startLines[m_rank]; ++l) {
        std::getline(fileStream,line);
    }
    //read rank rows
    double *initialSolution = system->getSolutionRawPtr();
      for (int i = 0; i < procLines[m_rank]; ++i) {
        fileStream >> initialSolution[i];
    }
    system->restoreSolutionRawPtr(initialSolution);

}

/*!
 * It computes the number of lines each process has to read into the initial solution file
 * \return a vector of m_nProcessors elements containing the number of file lines for each process
 */
std::vector<int> InitialSolutionReader::computeLinesPerProc()
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
 * It computes the line number which each process has to start reading at into the initial solution file
 * \param[in] procRows a vector of m_nProcessors elements containing the number of file lines for each process
 * \return a vector of m_nProcessors elements containing the line number which each process starts reading at
 */
std::vector<int> InitialSolutionReader::computeStartLinePerProc(const std::vector<int> & procRows)
{
    std::vector<int> startLines(m_nProcessors,0);

    for(int p = 1; p < m_nProcessors; ++p) {
        for(int pp = 0; pp < p; ++pp) {
            startLines[p] += procRows[pp];
        }
    }

    return startLines;
}

/*!
 * It sets the initial solution folder name into the file handler
 * @param dir initial solution folder name
 */
void InitialSolutionReader::setDirectory(const std::string& dir)
{
    m_fileHandler.setDirectory(dir);
}

/*!
 * It sets the initial solution file name into the file handler
 * @param dir initial solution file name
 */
void InitialSolutionReader::setName(const std::string& name)
{
    m_fileHandler.setName(name);
}

/*!
 * It sets the initial solution file extension into the file handler
 * @param dir initial solution file extension
 */
void InitialSolutionReader::setAppendix(const std::string& app)
{
    m_fileHandler.setAppendix(app);
}
