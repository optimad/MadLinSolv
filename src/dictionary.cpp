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

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <bitpit_IO.hpp>

#include "dictionary.hpp"

using namespace bitpit;

/*!
 * It reads the XML dictionary using libxml2 C API and sets the values for Dictionary members.
 * Beware: libxml2 has C and not C++ API
 * \param[in] filename path of XML dictionary file
 */
void Dictionary::readXML(std::string filename)
{

    LIBXML_TEST_VERSION

    xmlDocPtr doc;
    doc = xmlReadFile(filename.c_str(),NULL,0);
    if(doc == NULL) {
        log::cout() << filename << std::endl;
    }
    else {
        std::string content;
        std::string name;
        xmlNode *root_element = NULL;
        xmlNode *children = NULL;
        root_element = xmlDocGetRootElement(doc);
        xmlNode *cur_node = NULL;

//        cur_node = root_element->children;
//        cur_node = cur_node->next;


         for (cur_node = root_element->children; cur_node != NULL; cur_node = cur_node->next) {
             if (cur_node->type == XML_ELEMENT_NODE) {
                 printf("node type: Element, name: %s\n", cur_node->name);
                 name = reinterpret_cast<const char*>(cur_node->name);
                 if(name == "Solver") {
                     //children = cur_node->children;
                     for (children = cur_node->children; children != NULL; children = children->next) {
                         if (children->type == XML_ELEMENT_NODE) {
                             name = reinterpret_cast<const char*>(children->name);
                             if( name == "debug") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setDebug(content == "true");
                             }
                             else {
                                 log::cout() << "No other settings are allowed for Solver!" << std::endl;
                             }
                         }
                     }
                 }
                 else if (name == "Matrix") {
                     for (children = cur_node->children; children != NULL; children = children->next) {
                         if (children->type == XML_ELEMENT_NODE) {
                             name = reinterpret_cast<const char*>(children->name);
                             if(name == "directory") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setMatrixDir(content);
                             }
                             else if (name == "name") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setMatrixName(content);
                             }
                             else if (name == "appendix") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setMatrixApp(content);
                             }
                             else {
                                 log::cout() << "No other settings are allowed for Matrix!" << std::endl;
                             }
                         }
                     }
                 }
                 else if (name == "RHS") {
                     for (children = cur_node->children; children != NULL; children = children->next) {
                         if (children->type == XML_ELEMENT_NODE) {
                             name = reinterpret_cast<const char*>(children->name);
                             if(name == "directory") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setRhsDir(content);
                             }
                             else if (name == "name") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setRhsName(content);
                             }
                             else if (name == "appendix") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setRhsApp(content);
                             }
                             else {
                                 log::cout() << "No other settings are allowed for RHS!" << std::endl;
                             }
                         }
                     }
                 }
                 else if (name == "InitialSolution") {
                     for (children = cur_node->children; children != NULL; children = children->next) {
                         if (children->type == XML_ELEMENT_NODE) {
                             name = reinterpret_cast<const char*>(children->name);
                             if( name == "haveIt") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setHaveInitialSolution(content == "true");
                             }
                             else if(name == "directory") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setInitialSolutionDir(content);
                             }
                             else if (name == "name") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setInitialSolutionName(content);
                             }
                             else if (name == "appendix") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setInitialSolutionApp(content);
                             }
                             else {
                                 log::cout() << "No other settings are allowed for InitialSolution!" << std::endl;
                             }
                         }
                     }
                 }
                 else if (name == "Dump") {
                     for (children = cur_node->children; children != NULL; children = children->next) {
                         if (children->type == XML_ELEMENT_NODE) {
                             name = reinterpret_cast<const char*>(children->name);
                             if( name == "on") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setDumpOn(content == "true");
                             }
                             else if(name == "directory") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setDumpDir(content);
                             }
                             else if (name == "name") {
                                 content = reinterpret_cast<const char*>(children->children->content);
                                 setDumpName(content);
                             }
                             else {
                                 log::cout() << "No other settings are allowed for dump!" << std::endl;
                             }
                         }
                     }
                 }
                 else {
                     log::cout() << "Only Solver, Matrix, RHS, InitialSolution and Dump are available..." << std::endl;
                 }
             }
         }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
}

/*!
 * It gets the value XML block option and set the value of the var variable
 * @param blockXML the XML block containing the option
 * @param option the name of the option
 * @param var the boolean variable to be set at option value
 */
template<>
void Dictionary::absorboption<bool>(bitpit::Config::Section & blockXML, std::string option, bool & var)
{
    if(blockXML.hasOption(option)){
        std::string input = blockXML.get(option);
        input = bitpit::utils::string::trim(input);
        if(!input.empty()){
            std::stringstream ss(input);
            ss >> std::boolalpha >> var;
        }
    }
}

/*!
 * It reads the XML dictionary using bitpit API based on libxml2 C API and sets the values for Dictionary members.
 * Beware: libxml2 has C and not C++ API
 * \param[in] filename path of XML dictionary file
 */
void Dictionary::readXMLbitpit(std::string filename)
{
    bitpit::config::reset("MadLinSolv", 1);
    bitpit::config::read(filename);

    if(bitpit::config::root.hasSection("Solver")){
        bitpit::Config::Section & blockXML = bitpit::config::root.getSection("Solver");
        absorboption(blockXML, "debug", debug);
    }
    if(bitpit::config::root.hasSection("Matrix")){
        bitpit::Config::Section & blockXML = bitpit::config::root.getSection("Matrix");
        absorboption(blockXML, "directory", matrix_dir);
        absorboption(blockXML, "name", matrix_name);
        absorboption(blockXML, "appendix", matrix_app);
    }
    if(bitpit::config::root.hasSection("RHS")){
        bitpit::Config::Section & blockXML = bitpit::config::root.getSection("RHS");
        absorboption(blockXML, "directory", rhs_dir);
        absorboption(blockXML, "name", rhs_name);
        absorboption(blockXML, "appendix", rhs_app);
    }
    if(bitpit::config::root.hasSection("InitialSolution")){
        bitpit::Config::Section & blockXML = bitpit::config::root.getSection("InitialSolution");
        absorboption(blockXML, "haveIt", haveInitialSolution);
        absorboption(blockXML, "directory", initialSolution_dir);
        absorboption(blockXML, "name", initialSolution_name);
        absorboption(blockXML, "appendix", initialSolution_app);
    }
    if(bitpit::config::root.hasSection("Dump")){
        bitpit::Config::Section & blockXML = bitpit::config::root.getSection("Dump");
        absorboption(blockXML, "directory", dump_dir);
        absorboption(blockXML, "name", dump_name);
        absorboption(blockXML, "on", dumpOn);
    }
}

/*!
 * It gets the debug flag
 * \return a copy of the boolean flag
 */
bool Dictionary::isDebug() const
{
    return debug;
}

/*!
 * It sets the debug activation flag
 * \param[in] debug boolean to activate debug mode
 */
void Dictionary::setDebug(bool debug)
{
    this->debug = debug;
}

/*!
 * It gets the dump folder path
 * @return a constant reference to the dump folder path string
 */
const std::string& Dictionary::getDumpDir() const
{
    return dump_dir;
}

/*!
 * It sets the dump folder path
 * \param[in] dumpDir path to dump output folder
 */
void Dictionary::setDumpDir(const std::string& dumpDir)
{
    dump_dir = dumpDir;
}

/*!
 * It gets the dump file name
 * @return a constant reference to the dump file name string
 */
const std::string& Dictionary::getDumpName() const
{
    return dump_name;
}

/*!
 * It sets the dump file name
 * \param[in] dumpName prefix for dump files
 */
void Dictionary::setDumpName(const std::string& dumpName)
{
    dump_name = dumpName;
}

/*!
 * It gets the value of boolean activating the dump
 * @return a copy of the dump boolean value
 */
bool Dictionary::isDumpOn() const
{
    return dumpOn;
}

/*!
 * It sets the dump activation flag
 * \param[in] dumpOn boolean to activate system dump
 */
void Dictionary::setDumpOn(bool dumpOn)
{
    this->dumpOn = dumpOn;
}

/*!
 * It gets the value of boolean activating the initial solution guess reading
 * @return a copy of the initial solution guess reading boolean value
 */
bool Dictionary::isHaveInitialSolution() const
{
    return haveInitialSolution;
}

/*!
 * It sets the value of boolean activating the initial solution guess reading
 * \param[in] haveInitialSolution boolean to provide initial solution guess by file
 */
void Dictionary::setHaveInitialSolution(bool haveInitialSolution)
{
    this->haveInitialSolution = haveInitialSolution;
}

/*!
 * It gets the initial solution file extension
 * @return a constant reference to the initial solution extension string
 */
const std::string& Dictionary::getInitialSolutionApp() const
{
    return initialSolution_app;
}

/*!
 * It sets the initial solution file extension
 * \param[in] initialSolutionApp extension of initial solution file
 */
void Dictionary::setInitialSolutionApp(const std::string& initialSolutionApp)
{
    initialSolution_app = initialSolutionApp;
}

/*!
 * It gets the initial solution folder path
 * @return a constant reference to the initial solution folder path string
 */
const std::string& Dictionary::getInitialSolutionDir() const
{
    return initialSolution_dir;
}

/*!
 * It sets the initial solution folder path
 * \param[in] initialSolutionDir path to initial solution file folder
 */
void Dictionary::setInitialSolutionDir(const std::string& initialSolutionDir)
{
    initialSolution_dir = initialSolutionDir;
}

/*!
 * It gets the initial solution file name
 * @return a constant reference to the initial solution file name string
 */
const std::string& Dictionary::getInitialSolutionName() const
{
    return initialSolution_name;
}

/*!
 * It sets the initial solution file name
 * \param[in] initialSolutionName initial solution file name without extension
 */
void Dictionary::setInitialSolutionName(const std::string& initialSolutionName)
{
    initialSolution_name = initialSolutionName;
}

/*!
 * It gets the initial solution extension
 * @return a constant reference to the initial solution extension string
 */
const std::string& Dictionary::getMatrixApp() const
{
    return matrix_app;
}

/*!
 * It sets the matrix file extension
 * \param[in] matrixApp extension of matrix file
 */
void Dictionary::setMatrixApp(const std::string& matrixApp)
{
    matrix_app = matrixApp;
}

/*!
 * It gets the matrix folder path
 * @return a constant reference to the matrix folder path string
 */
const std::string& Dictionary::getMatrixDir() const
{
    return matrix_dir;
}

/*!
 * It sets the matrix folder path
 * \param[in] matrixDir path of matrix file folder
 */
void Dictionary::setMatrixDir(const std::string& matrixDir)
{
    matrix_dir = matrixDir;
}

/*!
 * It gets the matrix file name
 * @return a constant reference to the matrix file name string
 */
const std::string& Dictionary::getMatrixName() const
{
    return matrix_name;
}

/*!
 * It sets the matrix file name
 * \param[in] matrixName matrix file name without extension
 */
void Dictionary::setMatrixName(const std::string& matrixName)
{
    matrix_name = matrixName;
}

/*!
 * It gets the right-hand side extension
 * @return a constant reference to the right-hand side extension string
 */
const std::string& Dictionary::getRhsApp() const
{
    return rhs_app;
}

/*!
 * It sets the right-hand side file extension
 * \param[in] rhsApp extension of the right-hand side file
 */
void Dictionary::setRhsApp(const std::string& rhsApp)
{
    rhs_app = rhsApp;
}

/*!
 * It gets the right-hand side folder path
 * @return a constant reference to the right-hand side folder path string
 */
const std::string& Dictionary::getRhsDir() const
{
    return rhs_dir;
}

/*!
 * It sets the right-hand side folder path
 * \param[in] rhsDir path of right-hand side folder
 */
void Dictionary::setRhsDir(const std::string& rhsDir)
{
    rhs_dir = rhsDir;
}

/*!
 * It gets the right-hand side file name
 * @return a constant reference to the right-hand side file name string
 */
const std::string& Dictionary::getRhsName() const
{
    return rhs_name;
}

/*!
 * It sets the right-hand side file name
 * \param[in] rhsName right-hand side file name
 */
void Dictionary::setRhsName(const std::string& rhsName)
{
    rhs_name = rhsName;
}
