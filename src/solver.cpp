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

#if ENABLE_MPI==1
#    include <mpi.h>
#endif

#include "solver.hpp"

#include <bitpit_LA.hpp>

using namespace bitpit;


/*!
 *  Default Constructor
 *  It sets all the unique_ptr members to nullptr
 *  They should be allocated dynamically elsewhere
 *  It sets m_nProcessors and m_rank to 1 and 0 in serial runs (meaning ENABLE_MPI == 0)
 *  and calling MPI routines for parallel ones
 */
Solver::Solver() :
        m_matrixReader(nullptr), m_matrix(nullptr), m_rhsReader(nullptr), m_initialSolutionReader(nullptr), m_system(nullptr)
{
#if ENABLE_MPI==1
    MPI_Comm_size(MPI_COMM_WORLD, &m_nProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
#else
    m_nProcessors = 1;
    m_rank = 0;
#endif
}

/*!
 *  Constructor
 *  It sets all the unique_ptr members to nullptr
 *  They should be allocated dynamically elsewhere
 *  It sets m_nProcessors and m_rank to values passed from the caller
 */
Solver::Solver(int nProcessors, int rank) :
        m_nProcessors(nProcessors), m_rank(rank), m_matrixReader(nullptr), m_matrix(nullptr),
        m_rhsReader(nullptr), m_initialSolutionReader(nullptr), m_system(nullptr)
{

}

/*!
 * It gets the m_matrixReader member
 * @return a reference to MatrixReader unique pointer
 */
std::unique_ptr<MatrixReader>& Solver::getMatrixReader()
{
    return m_matrixReader;
}

/*!
 * It gets the m_matrix member
 * @return a reference to the SparseMatrix unique pointer
 */
std::unique_ptr<SparseMatrix>& Solver::getMatrix()
{
    return m_matrix;
}

/*!
 * It gets the m_rhsReader member
 * @return a reference to the RhsReader unique pointer
 */
std::unique_ptr<RhsReader>& Solver::getRhsReader()
{
    return m_rhsReader;
}

/*!
 * It gets the m_initialSolutionReader member
 * @return a reference to the InitialSolutionReader unique pointer
 */
std::unique_ptr<InitialSolutionReader>& Solver::getInitialSolutionReader()
{
    return m_initialSolutionReader;
}

/*!
 * It gets the m_system member
 * @return a reference to the SystemSolver unique pointer
 */
std::unique_ptr<SystemSolver> & Solver::getSystem()
{
    return m_system;
}
