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

#include <bitpit_IO.hpp>

#include "run_manager.hpp"

using namespace bitpit;

/*
 * Main
 */
int main(int argc, char *argv[])
{
    // Initialize parallel
    int nProcessors;
    int rank;

#if ENABLE_MPI==1
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nProcessors);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
    nProcessors = 1;
    rank = 0;
#endif

    // Initialize logger
    log::manager().initialize(log::COMBINED, "MadLinSolv", true, ".", nProcessors, rank);
#if ENABLE_DEBUG==1
    log::cout().setVisibility(log::GLOBAL);
#endif

    // Log file header
    log::cout() << "|=====================================================|" << std::endl;
    log::cout() << "|                                                     |" << std::endl;
    log::cout() << "|                     MadLinSolv                      |" << std::endl;
    log::cout() << "|                                                     |" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;
    log::cout() << "| version:  v0.1                                      |" << std::endl;
    log::cout() << "|                                                     |" << std::endl;
    log::cout() << "|=====================================================|" << std::endl;

    //
    // Execution
    //
    RunManager::execute(nProcessors,rank);


    //
    // Finalization
    //

#if ENABLE_MPI==1
    // MPI finalization
    MPI_Finalize();
#endif

}
