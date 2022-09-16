/*
 * YamlPosition.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "xdyn/external_data_structures/YamlPosition.hpp"

YamlPosition::YamlPosition() : coordinates(YamlCoordinates()),
                               angle(YamlAngle()),
                               frame("")
{}

YamlPosition::YamlPosition(const YamlCoordinates& c, const YamlAngle& a, const std::string& f) :
        coordinates(c),
        angle(a),
        frame(f)
{
}

YamlPosition YamlPosition::Origin(const std::string& frame)
{
    return YamlPosition(YamlCoordinates(0,0,0), YamlAngle(0,0,0), frame);
}

std::ostream& operator<<(std::ostream& os, const YamlPosition& w)
{
    os << std::endl
       << "    frame: " << w.frame << std::endl
       << "    coordinates:" << std::endl
       << "        x: " << w.coordinates.x << std::endl
       << "        y: " << w.coordinates.y << std::endl
       << "        z: " << w.coordinates.z << std::endl
       << "    angle:" << std::endl
       << "        phi: " << w.angle.phi << std::endl
       << "        theta: " << w.angle.theta << std::endl
       << "        psi: " << w.angle.psi << std::endl;
    return os;
}
