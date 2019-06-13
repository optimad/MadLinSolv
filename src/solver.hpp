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

#ifndef __MADLINSOLV_SOLVER_HPP__
#define __MADLINSOLV_SOLVER_HPP__

#include <memory>

#include <bitpit_LA.hpp>

#include "matrixReader.hpp"
#include "rhsReader.hpp"
#include "initialSolutionReader.hpp"

using namespace bitpit;

/*!
 *  \authors        Marco Cisternino
 *
 *  \brief The solver class
 *
 *  This class is intended to
 *  read from the disk the components of a linear system (matrix, right-hand side and initial solution guess),
 *  declare the linear system object (which containing the matrix, the right-hand side and the solution) from bitpit library
 */

class Solver {

public:

    Solver();
    Solver(int nProcessors, int rank);

    std::unique_ptr<MatrixReader> & getMatrixReader();
    std::unique_ptr<SparseMatrix> & getMatrix();
    std::unique_ptr<RhsReader> & getRhsReader();
    std::unique_ptr<InitialSolutionReader> & getInitialSolutionReader();
    std::unique_ptr<SystemSolver> & getSystem();

private:

    int m_nProcessors;
    int m_rank;

    std::unique_ptr<MatrixReader> m_matrixReader;                   /**<unique pointer to MatrixReader. It reads the matrix from disk*/
    std::unique_ptr<SparseMatrix> m_matrix;                         /**<unique pointer to SparseMatrix. It is the bitpit implementation for sparse matrices*/
    std::unique_ptr<RhsReader> m_rhsReader;                         /**<unique pointer to RhsReader. It reads the right-hand side from disk*/
    std::unique_ptr<InitialSolutionReader> m_initialSolutionReader; /**<unique pointer to InitialSolutionReader. It reads the initial solution guess from disk*/
    std::unique_ptr<SystemSolver> m_system;                         /**<unique pointer to SystemSolver. It is the bitpit wrapper to PETSc methods for setting and solving linear systems*/

};

#endif
