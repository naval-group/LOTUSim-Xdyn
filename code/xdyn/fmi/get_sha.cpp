/*
 * get_sha.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cady
 */

#include "get_sha.hpp"
#include "Sha.hpp"
#include "xdyn/external_data_structures/YamlSimulatorInput.hpp"
#include "get_git_sha/get_git_sha.h"
#include "yaml_parser/check_input_yaml.hpp"

#include <sstream>

std::string sha(const std::string& s, const YamlSimulatorInput& i);
std::string sha(const std::string& s, const YamlSimulatorInput& i)
{
    std::stringstream ss;
    Sha yamlSha1(s, i);
    ss << yamlSha1;
    return ss.str();
}

std::string fmi::get_sha(const YamlSimulatorInput& input)
{
    return sha(get_git_sha(), input);
}
