/*
 * CenterOfMass.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: cady
 */

#include "CenterOfMass.hpp"

CenterOfMass::CenterOfMass(const EPoint& G_, const double v_) : G(G_), volume(v_), all_facets_are_in_same_plane(false)
{
}

CenterOfMass& CenterOfMass::operator+=(const CenterOfMass& com)
{
    G += com.G*com.volume;
    volume += com.volume;
    return *this;
}
