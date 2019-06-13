/*!
 * Absorb an option of a section of a stored xml tree
 * \param[in] blockXML data structure of a xml section
 * \param[in] option name of the option to absorb
 * \param[out] var stored data
 */
template<typename T>
void Dictionary::absorboption(bitpit::Config::Section & blockXML, std::string option, T & var)
{
    if(blockXML.hasOption(option)){
        std::string input = blockXML.get(option);
        input = bitpit::utils::string::trim(input);
        if(!input.empty()){
            std::stringstream ss(input);
            ss >> var;
        }
    }
}
