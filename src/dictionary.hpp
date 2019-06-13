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

#ifndef __MADLINSOLV_DICTIONARY_HPP__
#define __MADLINSOLV_DICTIONARY_HPP__

#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "bitpit_IO.hpp"

/*!
 *  \authors        Marco Cisternino
 *
 *  \brief The dictionary class
 *
 *  This class is intended as XML user interface for controlling the solver
 *  At the moment the XML nodes admitted are in the following template. An example can be find in data folder (dictionary.xml)
 *  \verbatim
 *  <?xml version="1.0" encoding="UTF-8"?>
 *  <MadLinSolv website="">
 *    <Solver>
 *      <debug>...true/false...</debug>                             --> it controls the PETSc log_summary options and the PETSc true residuals print
 *    </Solver>
 *    <Matrix>
 *      <directory>...matrix folder...</directory>                  --> it controls the input folder for matrix file
 *      <name>...matrix file name...</name>                         --> it controls matrix file name
 *      <appendix>...matrix extension...</appendix>                 --> it controls matrix extension
 *    </Matrix>
 *    <RHS>
 *      <directory>...right-hand side folder...</directory>         --> it controls the input folder for right-hand side file
 *      <name>...right-hand side file name...</name>                --> it controls right-hand side file name
 *      <appendix>...right-hand side extension</appendix>           --> it controls right-hand side extension
 *    </RHS>
 *    <InitialSolution>
 *      <haveIt>...true/false...</haveIt>                           --> it controls if the user wants to provide an initial guess for the solution
 *      <directory>...initial solution guess folder...</directory>  --> it controls the input folder for initial solution guess file
 *      <name>...initial solution guess name...</name>              --> it controls the initial solution guess file name
 *      <appendix>...initial solution guess extension...</appendix> --> it controls the initial solution guess extension
 *    </InitialSolution>
 *    <Dump>
 *      <on>...true/false...</on>                                   --> it controls if the user wants to print matrix, right-hand side and solution file
 *      <directory>...dump folder...</directory>                    --> it controls the output folder for matrix, right-hand side and solution file
 *      <name>...dump name...</name>                                --> it controls the dump files prefix (outputs are [dump name]matrix.txt, [dump name]rhs.txt), [dump name]solution.txt)
 *    </Dump>
 *  </MadLinSolv>
 *  \endverbatim
 */
class Dictionary {

public:

    void readXML(std::string filename);
    void readXMLbitpit(std::string filename);

    bool isDebug() const;
    void setDebug(bool debug);
    const std::string& getDumpDir() const;
    void setDumpDir(const std::string& dumpDir);
    const std::string& getDumpName() const;
    void setDumpName(const std::string& dumpName);
    bool isDumpOn() const;
    void setDumpOn(bool dumpOn);
    bool isHaveInitialSolution() const;
    void setHaveInitialSolution(bool haveInitialSolution);
    const std::string& getInitialSolutionApp() const;
    void setInitialSolutionApp(const std::string& initialSolutionApp);
    const std::string& getInitialSolutionDir() const;
    void setInitialSolutionDir(const std::string& initialSolutionDir);
    const std::string& getInitialSolutionName() const;
    void setInitialSolutionName(const std::string& initialSolutionName);
    const std::string& getMatrixApp() const;
    void setMatrixApp(const std::string& matrixApp);
    const std::string& getMatrixDir() const;
    void setMatrixDir(const std::string& matrixDir);
    const std::string& getMatrixName() const;
    void setMatrixName(const std::string& matrixName);
    const std::string& getRhsApp() const;
    void setRhsApp(const std::string& rhsApp);
    const std::string& getRhsDir() const;
    void setRhsDir(const std::string& rhsDir);
    const std::string& getRhsName() const;
    void setRhsName(const std::string& rhsName);

private:
    bool debug;                             /**<boolean for controlling PETSc log and residuals print*/
    std::string matrix_dir;                 /**<matrix folder*/
    std::string matrix_name;                /**<matrix name*/
    std::string matrix_app;                 /**<matrix extension*/
    std::string rhs_dir;                    /**<right-hand side folder*/
    std::string rhs_name;                   /**<right-hand side name*/
    std::string rhs_app;                    /**<right-hand side extension*/
    bool haveInitialSolution;               /**<boolean for activating initial solution guess reading*/
    std::string initialSolution_dir;        /**<initial solution folder*/
    std::string initialSolution_name;       /**<initial solution name*/
    std::string initialSolution_app;        /**<initial solution extension*/
    bool dumpOn;                            /**<boolean for activating system components outputs*/
    std::string dump_dir;                   /**<dump output folder*/
    std::string dump_name;                  /**<dump output file name prefix*/

    template<typename T>
    void absorboption(bitpit::Config::Section & blockXML, std::string option, T & var);
};

#include "dictionary.tpp"


#endif
