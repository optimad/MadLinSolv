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

#ifndef __MADLINSOLV_MATRIXREADER_HPP__
#define __MADLINSOLV_MATRIXREADER_HPP__

#include <memory>
#include <fstream>
#include <string>

#include <bitpit_IO.hpp>
#include <bitpit_LA.hpp>

using namespace bitpit;

/*!
 *  \authors        Marco Cisternino
 *
 *  \brief The Compressed Sparse Rows matrix reader class
 *
 *  This class is intended to
 *  read from the matrix the user right-hand side
 *  file format is serial and ASCII but reading is parallel (possible future development:MPI I/O):
 *
 *  \verbatim
 *                                          CSR  file format
 *            -----------------------------------------------------------------------------
 *  line 1    | global_number_of_rows global_number_of_columns global_number_of_nonzeros  | ---> header
 *  line 2    | element_1_nonzeros_indices                                                | ---
 *  line 3    | element_1_nonzeros_value                                                  |   |
 *  ...       | ...                                                                       |   | ---> body
 *  line 2N   | element_N_nonzeros_indices                                                |   |
 *  line 2N+1 | element_N_nonzeros_values                                                 | ---
 *            -----------------------------------------------------------------------------
 *  \endverbatim
 */
class MatrixReader {

public:

    MatrixReader(int nProcessors, int rank);
    MatrixReader(int nProcessors, int rank, const std::string & dir_, const std::string & name_, const std::string & app_);

    void readMatrixCSRFormat(std::unique_ptr<SparseMatrix> & matrix);
    void readMatrixCSRFormatInfo(std::fstream & fileStream);
    void readMatrixCSRFormatMatrix(std::fstream & fileStream, const std::vector<int> & procLines,
            const std::vector<int> & startLines, std::unique_ptr<SparseMatrix> & matrix);

    void setNRows(int nRows);
    void setNCols(int nCols);
    void setNNz(int nNz);
    void setDirectory(const std::string & dir);
    void setName(const std::string & name);
    void setAppendix(const std::string & app);

    std::string getPath();
    std::string getDirectory();
    std::string getName();
    std::string getAppendix();

    int getNRows();

private:

    std::vector<int> computeLinesPerProc();
    std::vector<int> computeStartLinePerProc(const std::vector<int> & procRows);

    int m_nProcessors;                                  /**<number of MPI processes*/
    int m_rank;                                         /**<MPI rank of the process*/

    FileHandler m_fileHandler;                          /**<bitpit file handler*/

    int m_nRows;                                        /**<number of rows as read in header file*/
    int m_nCols;                                        /**<number of columns as read in header file*/
    int m_nNz;                                          /**<number of non-zeros as read in header file*/

};

#endif
