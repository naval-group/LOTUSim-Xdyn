/*
 * generate_fmi_xml.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cady
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <ssc/check_ssc_version.hpp>
#include <ssc/text_file_reader.hpp>
#include "get_git_sha.h"
#include "EmitFMIXml.hpp"

std::string description(const std::string& des);
std::string description(const std::string& des)
{
    std::stringstream ss;
    ss << des << " created during the project 'Bassin Numerique (IRT Jules Verne)'." << std::endl
       << "(c) SIREHNA 2014-2015." << std::endl
       << std::endl
       << "ID: " << get_git_sha() << std::endl
       << "SHA of the SSC used: " << LONG_SSC_GIT_SHA << std::endl
       << std::endl;
    return ss.str();
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << description("FMI XML generator");
        std::cerr << "Need at least one YAML file." << std::endl;
        return -1;
    }
    const std::vector<std::string> filenames(argv+1, argv+argc);
    const ssc::text_file_reader::TextFileReader yaml_reader(filenames);
    std::cout << fmi::emit(fmi::build(yaml_reader.get_contents())) << std::endl;
    return 0;
}
