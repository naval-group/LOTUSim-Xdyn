/*
 * EmitFMIXml.hpp
 *
 *  Created on: May 22, 2015
 *      Author: cady
 */

#ifndef EMITFMIXML_HPP_
#define EMITFMIXML_HPP_

#include <string>

#include "FMIXml.hpp"

namespace fmi
{
    std::string emit(const Xml& xml);
    Xml build(const std::string& yaml);
}

#endif  /* EMITFMIXML_HPP_ */
