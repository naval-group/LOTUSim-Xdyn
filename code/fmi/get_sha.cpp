/*
 * get_sha.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cady
 */

#include <sstream>

#include "Sha.hpp"
#include "YamlSimulatorInput.hpp"
#include "get_sha.hpp"
#include "get_git_sha.h"

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
