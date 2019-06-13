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

# Add a target to generate API documentation with Doxygen
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)

IF(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF()

  option(DOC_EXTRACT_PRIVATE "If set, all private members are included in the documentation" OFF)
  IF (DOC_EXTRACT_PRIVATE)
    SET(DOXY_EXTRACT_PRIVATE "YES")
  ELSE ()
    SET(DOXY_EXTRACT_PRIVATE "NO")
  ENDIF ()

  SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in     )
  SET( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile              )
  SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html )
  SET( doxy_output_root     ${CMAKE_CURRENT_BINARY_DIR}                 ) # Pasted into Doxyfile.in
  SET( doxy_input           ${PROJECT_SOURCE_DIR}/src                   ) # Pasted into Doxyfile.in
  SET( doxy_extra_files     ${CMAKE_CURRENT_SOURCE_DIR}/extra/MadLinSolv.hpp )

  if (NOT "${DOC_EXTRACT_PRIVATE_INTERNAL}" STREQUAL "${DOC_EXTRACT_PRIVATE}")
    if (EXISTS ${doxy_html_index_file})
      FILE (REMOVE ${doxy_html_index_file})
    endif ()
    set(DOC_EXTRACT_PRIVATE_INTERNAL ${DOC_EXTRACT_PRIVATE} CACHE INTERNAL "This is the value of the last time DOC_EXTRACT_PRIVATE was set successfully." FORCE)
  endif ()

  CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )

  ADD_CUSTOM_COMMAND( OUTPUT ${doxy_html_index_file}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      # The following should be ${doxyfile} only but it
                      # will break the dependency.
                      # The optimal solution would be creating a
                      # custom_command for ${doxyfile} generation
                      # but I still have to figure out how...
                      MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
                      DEPENDS ${doxy_extra_files} ${doxyfile_in} ${PATCHMAN_SOURCES} ${PATCHMAN_HEADERS} ${EXAMPLES_SOURCES}
                      COMMENT "Generating HTML documentation")

  ADD_CUSTOM_TARGET( doc ALL DEPENDS ${doxy_html_index_file} )
  add_custom_target(clean-doc COMMAND ${CMAKE_MAKE_PROGRAM} clean WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc/${CMAKE_PROJECT_NAME}-${MADLINSOLV_VERSION} )
ENDIF()
