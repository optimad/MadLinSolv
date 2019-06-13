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

#ifndef __MADLINSOLV_RUN_MANAGER_HPP__
#define __MADLINSOLV_RUN_MANAGER_HPP__

#include <memory>

#include "solver.hpp"
#include "dictionary.hpp"

/*!
 *  \authors        Marco Cisternino
 *
 *  \brief The run manager class
 *
 *  This class is intended to manage the execution of this application through its static method execute.
 *  It encapsulates the XML dictionary user interface and the solver class
 *  The application work flow is expressed by the sequence of its methods:
 *   - preprocess
 *   - compute
 *   - postprocess
 */

class RunManager {

public:
    static void execute(int nProcessors, int rank); //TODO pass argc argv from main to Solver and SystemSolver
private:
    int m_nProcessors;                                  /**<number of MPI processes*/
    int m_rank;                                         /**<MPI rank of the process*/
    Dictionary m_dictionary;                            /**<XML user interface object*/

    std::unique_ptr<Solver> m_solver;                   /**<unique pointer to Solver. It manages bitpit system solvers and disk file readers*/

    RunManager(int nProcessors, int rank);
    RunManager(RunManager const&) = delete;
    RunManager & operator=(RunManager const&) = delete;

    void preprocess();
    void compute();
    void postprocess();

};



#endif
