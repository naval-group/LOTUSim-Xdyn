/*
 * get_sha.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cady
 */

#include <sstream>

#include "calculate_sha1.hpp"
#include "YamlSimulatorInput.hpp"
#include "get_sha.hpp"

std::string sha(const std::string& s, const YamlSimulatorInput& i);
std::string sha(const std::string& s, const YamlSimulatorInput& i)
{
    std::stringstream ss;
    YamlSha1 yamlSha1(s, i);
    ss << yamlSha1;
    return ss.str();
}

std::string fmi::get_sha(const YamlSimulatorInput& input)
{
    return sha("@GIT_SHA1@", input);
}
