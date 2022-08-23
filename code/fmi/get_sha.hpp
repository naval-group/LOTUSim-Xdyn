/*
 * get_sha.hpp
 *
 *  Created on: May 29, 2015
 *      Author: cady
 */

#ifndef GET_SHA_HPP_
#define GET_SHA_HPP_

#include <string>

struct YamlSimulatorInput;

namespace fmi
{
    std::string get_sha(const YamlSimulatorInput& input);
}

#endif  /* GET_SHA_HPP_ */
