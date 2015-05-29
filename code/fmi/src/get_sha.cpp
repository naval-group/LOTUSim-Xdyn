/*
 * get_sha.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cady
 */

#include <sstream>

#include <boost/functional/hash.hpp>

#include "calculate_hash.hpp"
#include "get_sha.hpp"
#include "YamlSimulatorInput.hpp"

std::string sha(const std::string& s, const YamlSimulatorInput& i);
std::string sha(const std::string& s, const YamlSimulatorInput& i)
{
    std::stringstream ss;
    boost::hash<YamlSimulatorInput> hash;
    std::size_t seed = hash(i);
    boost::hash_combine(seed, s);
    ss << seed;
    return ss.str();
}

std::string fmi::get_sha(const YamlSimulatorInput& input)
{
    return sha("@GIT_SHA1@", input);
}
