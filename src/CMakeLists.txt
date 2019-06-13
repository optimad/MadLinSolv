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

#Specify the version being used as well as the language
cmake_minimum_required(VERSION 2.8)

# Set executable properties
set(MADLINSOLV_EXECUTABLE_NAME madlinsolv CACHE INTERNAL "Executable name of the solver" FORCE)

include_directories(${PETSC_INCLUDES})

file(GLOB sources "*.cpp")
add_executable(${MADLINSOLV_EXECUTABLE_NAME} ${sources})

target_link_libraries(${MADLINSOLV_EXECUTABLE_NAME} ${BITPIT_LIBRARIES})
#target_link_libraries(${MADLINSOLV_EXECUTABLE_NAME} ${LAPACKE_LIBRARIES})
#target_link_libraries(${MADLINSOLV_EXECUTABLE_NAME} ${LAPACK_LIBRARIES})

INSTALL (TARGETS ${MADLINSOLV_EXECUTABLE_NAME} DESTINATION bin)
