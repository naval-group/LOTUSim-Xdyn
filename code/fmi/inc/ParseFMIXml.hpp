/*
 * ParseFMIXml.hpp
 *
 *  Created on: May 21, 2015
 *      Author: cady
 */

#ifndef PARSEFMIXML_HPP_
#define PARSEFMIXML_HPP_

#include "FMIXml.hpp"

namespace fmi
{
    Xml parse(const std::string& xml);
}

#endif  /* PARSEFMIXML_HPP_ */
