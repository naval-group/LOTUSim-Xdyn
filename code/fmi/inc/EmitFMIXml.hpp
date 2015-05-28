/*
 * EmitFMIXml.hpp
 *
 *  Created on: May 22, 2015
 *      Author: cady
 */

#ifndef EMITFMIXML_HPP_
#define EMITFMIXML_HPP_

#include <string>

namespace fmi
{
    struct Xml;
    std::string emit(const Xml& xml);
}

#endif  /* EMITFMIXML_HPP_ */
