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

#include "run_manager.hpp"

using namespace bitpit;

/*!
 *  This static method executes the application work flow.
 *  It declares the object of the class it belongs to.
 *  It expresses the work flow by calling:
 *   - preprocess
 *   - compute
 *   - postprocess
 *
 *  \param[in] nProcessors the number of MPI ranks
 *  \param[in] rank        the MPI rank of the process owing the object
*/
void RunManager::execute(int nProcessors, int rank)
{
    RunManager m_manager(nProcessors,rank);

    m_manager.preprocess();
    m_manager.compute();
    m_manager.postprocess();

}

/*!
 *  Constructor
 *  It sets m_nProcessors and m_rank to values passed from the caller
 *  It dynamically allocates the Solver member
 *  It constructs a default dictionary
*/
RunManager::RunManager(int nProcessors, int rank)
    : m_nProcessors(nProcessors), m_rank(rank), m_dictionary(), m_solver(nullptr)
{
    //Declare solver
    m_solver = std::unique_ptr<Solver>(new Solver(m_nProcessors,m_rank));

}

/*!
 *  Preprocessing method.
 *  Briefly, it prepares the solver (basically the SystemSolver object) for the solving call by:
 *   - reading the XML user dictionary
 *   - initializing the system solver
 *   - reading (in parallel) the matrix (in CSR format, see MatrixReader class for details) from disk
 *   - reading (in parallel) the right-hand side from disk (see RhsReader class for details)
 *   - possibly, reading (in parallel) the initial solution guess from disk (see InitialSolutionReader class for details)
*/
void RunManager::preprocess()
{
    log::cout() << "" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;
    log::cout() << "| PREPROCESS                                          |" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;

    log::cout() << "" << std::endl;
    log::cout() << "    Reading Dictionary..." << std::endl;
    log::cout() << "    ---------------------" << std::endl;
    //m_dictionary.readXML("../../data/dictionary.xml");
    m_dictionary.readXMLbitpit("./dictionary.xml");

    log::cout() << "" << std::endl;
    log::cout() << "    Initializing Solver..." << std::endl;
    log::cout() << "    ----------------------" << std::endl;
    m_solver->getSystem() = std::unique_ptr<SystemSolver>(new SystemSolver(m_dictionary.isDebug()));


    log::cout() << "" << std::endl;
    log::cout() << "    Reading matrix..." << std::endl;
    log::cout() << "    -----------------" << std::endl;
    //Declare matrix reader
    m_solver->getMatrixReader() = std::unique_ptr<MatrixReader>(new MatrixReader(m_nProcessors,m_rank,
            m_dictionary.getMatrixDir(),m_dictionary.getMatrixName(),m_dictionary.getMatrixApp()));
    //Read matrix
    m_solver->getMatrixReader()->readMatrixCSRFormat( m_solver->getMatrix() );

    //Initialiaze linear system
    if(m_nProcessors > m_solver->getMatrixReader()->getNRows()) {
        log::cout() << "Number of processes greater than the global number of rows. Not supported, yet." << std::endl;
#if ENABLE_MPI == 1
        MPI_Finalize();
        exit(1);
#else
        exit(1);
#endif

    }
    else {
        log::cout() << "" << std::endl;
        log::cout() << "    Initializing solver..." << std::endl;
        log::cout() << "    ----------------------" << std::endl;
        m_solver->getSystem()->initialize(*(m_solver->getMatrix()));
    }

    //Declare RHS reader
    log::cout() << "" << std::endl;
    log::cout() << "    Reading RHS..." << std::endl;
    log::cout() << "    ----------------------" << std::endl;
    m_solver->getRhsReader() = std::unique_ptr<RhsReader>(new RhsReader(m_nProcessors,m_rank,
            m_dictionary.getRhsDir(),m_dictionary.getRhsName(),m_dictionary.getRhsApp()));
    //Read RHS
    m_solver->getRhsReader()->read(m_solver->getSystem(),m_solver->getMatrixReader()->getNRows());

    //Read initial solution
    if(m_dictionary.isHaveInitialSolution()) {
        log::cout() << "" << std::endl;
        log::cout() << "    Reading Initial Solution..." << std::endl;
        log::cout() << "    ----------------------" << std::endl;
        //Declare Initial Solution reader
        m_solver->getInitialSolutionReader() = std::unique_ptr<InitialSolutionReader>(new InitialSolutionReader(m_nProcessors,m_rank,
                m_dictionary.getInitialSolutionDir(),m_dictionary.getInitialSolutionName(),m_dictionary.getInitialSolutionApp()));
        //Read Initial Solution
        m_solver->getInitialSolutionReader()->read(m_solver->getSystem(),m_solver->getMatrixReader()->getNRows());
    }
    else {
        log::cout() << "No initial solution will be set. PETSc solution default initialization is used" << std::endl;
    }

}

/*!
 *  Computing method.
 *  Basically it calls the solve method of the SystemSolver class on m_system member of the m_solver member of this class
*/
void RunManager::compute()
{
    log::cout() << "" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;
    log::cout() << "| COMPUTE                                             |" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;

    log::cout() << "" << std::endl;
    log::cout() << "    Solving Linear System..." << std::endl;
    log::cout() << "    ----------------------" << std::endl;
    m_solver->getSystem()->solve();

}

/*!
 *  Postprocessing method.
 *  If user set by dictionary the system dump in mode "on",
 *  this method calls for SystemSolver PETSc based dump and matrix, right-hand side and solution are dumped in ASCII files.
 *  Otherwise, nothing happens, but log message printing
*/
void RunManager::postprocess()
{
    log::cout() << "" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;
    log::cout() << "| POSTPROCESS                                         |" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;

    if(m_dictionary.isDumpOn()) {
        log::cout() << "" << std::endl;
        log::cout() << "    Dumping Linear System..." << std::endl;
        log::cout() << "    ------------------------" << std::endl;
        m_solver->getSystem()->dump(m_dictionary.getDumpDir(),m_dictionary.getDumpName());
    }



}
