#---------------------------------------------------------------------------
#
#  MadLinSolv
#
#  -------------------------------------------------------------------------
#  License
#  This file is part of MadLinSolv.
#
#  MadLinSolv is free software: you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License v3 (LGPL)
#  as published by the Free Software Foundation.
#
#  MadLinSolv is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
#  License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with MadLinSolv. If not, see <http://www.gnu.org/licenses/>.
#
#---------------------------------------------------------------------------*/

# Specify the version being used as well as the language
cmake_minimum_required(VERSION 2.8)

#------------------------------------------------------------------------------------#
# Functions
#------------------------------------------------------------------------------------#

# Initialize test directory
function(initializeTestDirectory TEST_SETUP_TARGET TEST_DIRECTORY)
    getTestSetupTarget(TEST_SETUP_TARGET ${TEST_DIRECTORY})
    set(TEST_SETUP_TARGET ${TEST_SETUP_TARGET} PARENT_SCOPE)

    add_custom_target(${TEST_SETUP_TARGET} ALL)

    ADD_DEPENDENCIES(${TEST_SETUP_TARGET} ${MADLINSOLV_EXECUTABLE_NAME})

    # Copy the controldict file
#    add_custom_command(
#        TARGET ${TEST_SETUP_TARGET}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${CMAKE_CURRENT_SOURCE_DIR}/ControlDict.dat"
#            "${CMAKE_CURRENT_BINARY_DIR}/ControlDict.dat"
#        MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/ControlDict.dat"
#        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ControlDict.dat"
#    )

endfunction()

# Add a serial test
function(addSerialTest TEST_NAME TEST_EXPECTED_RESULTS WORKING_DIRECTORY)
    addTest("${TEST_NAME}" "${TEST_EXPECTED_RESULTS}" "${WORKING_DIRECTORY}" 1 1)
endfunction()

# Add a parallel MPI test
function(addParallelMPITest TEST_NAME TEST_EXPECTED_RESULTS WORKING_DIRECTORY)
    if (NOT ENABLE_MPI)
        return()
    endif ()

    # Get the number of processors to run the test on
    list(LENGTH ARGN ARGN_LENGTH)
    if (${ARGN_LENGTH} GREATER 0)
        list(GET ARGN 0 N_PROCS)
    else ()
        set(N_PROCS "${MPIEXEC_MAX_NUMPROCS}")
    endif ()

    # Add the test
    addTest("${TEST_NAME}" "${TEST_EXPECTED_RESULTS}" "${WORKING_DIRECTORY}" ${N_PROCS} 1)
endfunction()

# Add a test
function(addTest TEST_NAME TEST_EXPECTED_RESULTS WORKING_DIRECTORY N_PROCS)
    if (NOT ENABLE_MPI AND ${N_PROCS} GREATER 1)
        return()
    endif ()

    # Test command
    set(TEST_COMMAND "$<TARGET_FILE:${MADLINSOLV_EXECUTABLE_NAME}>")
    if (${N_PROCS} GREATER 1)
        set(TEST_COMMAND "${MPIEXEC} ${MPIEXEC_PREFLAGS} ${MPIEXEC_NUMPROC_FLAG} ${N_PROCS} ${MPIEXEC_POSTFLAGS}  ${TEST_COMMAND}")
    endif ()

    # Add test
    add_test(NAME ${TEST_NAME} COMMAND python ${CMAKE_BINARY_DIR}/test/test_driver.py --command "${TEST_COMMAND}" --expected "${TEST_EXPECTED_RESULTS}" WORKING_DIRECTORY "${WORKING_DIRECTORY}")

endfunction()

# Get the test setup target for the test
function(getTestSetupTarget TEST_SETUP_TARGET TEST_DIRECTORY)
    set(${TEST_SETUP_TARGET} "test_${TEST_DIRECTORY}_setup" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------------#
# Subdirectories
#------------------------------------------------------------------------------------#
set(TEST_DIRECTORIES "")
#list(APPEND TEST_DIRECTORIES "naca0012")

add_custom_target("test_setup")
foreach (TEST_DIRECTORY IN LISTS TEST_DIRECTORIES)
    # Add directory
    add_subdirectory("${TEST_DIRECTORY}")

    # Update the dependencies of the test setup target
    getTestSetupTarget(TEST_SETUP_TARGET ${TEST_DIRECTORY})
    ADD_DEPENDENCIES(test_setup ${TEST_SETUP_TARGET})
endforeach()

#------------------------------------------------------------------------------------#
# Targets
#------------------------------------------------------------------------------------#

add_custom_target(check DEPENDS ${MADLINSOLV_EXECUTABLE_NAME} test_setup COMMAND ${CMAKE_MAKE_PROGRAM} test)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/test_driver.py"
    "${CMAKE_CURRENT_BINARY_DIR}/test_driver.py"
)
