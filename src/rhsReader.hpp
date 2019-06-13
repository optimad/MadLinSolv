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

#ifndef __MADLINSOLV_RHSEADER_HPP__
#define __MADLINSOLV_RHSREADER_HPP__

#include <fstream>

#include <bitpit_IO.hpp>
#include <bitpit_LA.hpp>

using namespace bitpit;

/*!
 *  \authors        Marco Cisternino
 *
 *  \brief The right-hand side reader class
 *
 *  This class is intended to
 *  read from the disk the user right-hand side
 *  file format is serial and ASCII but reading is parallel (possible future development:MPI I/O):
 *  \verbatim
 *                     file format
 *           ------------------------------
 *  line 1   | global_number_of_elements  |   <-- header
 *  line 2   | element_1_value            |   --
 *  line 3   | element_2_value            |     |
 *  ...      | ...                        |     |---> body
 *  ...      | ...                        |     |
 *  line N+1 | element_N_value            |   --
 *           ------------------------------
 *  \endverbatim
 */

class RhsReader {

public:

    RhsReader(int nProcessors, int rank);
    RhsReader(int nProcessors, int rank, const std::string & dir_, const std::string & name_, const std::string & app_);

    void read(std::unique_ptr<SystemSolver> & system, int expectedElements);

    void setDirectory(const std::string & dir);
    void setName(const std::string & name);
    void setAppendix(const std::string & app);

private:
    void readInfo(std::fstream & fileStream, int expectedElements);
    void readRhs(std::fstream & fileStream, const std::vector<int> & procRows, const std::vector<int> & startRows,
            std::unique_ptr<SystemSolver> & system);

    std::vector<int> computeRowsPerProc();
    std::vector<int> computeStartRowPerProc(const std::vector<int> & procRows);

    int m_nProcessors;                                  /**<number of MPI processes*/
    int m_rank;                                         /**<MPI rank of the process*/

    FileHandler m_fileHandler;                          /**<bitpit file handler*/

    int m_nRows;                                        /**<number of rows as read in header file*/



};



#endif
