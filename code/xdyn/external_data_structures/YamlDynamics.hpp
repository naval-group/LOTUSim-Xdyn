/*
 * YamlDynamics.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLDYNAMICS_HPP_
#define YAMLDYNAMICS_HPP_

#include "xdyn/external_data_structures/YamlPoint.hpp"
#include "xdyn/external_data_structures/YamlDynamics6x6Matrix.hpp"

struct YamlDynamics
{
    YamlDynamics();
    YamlPoint centre_of_inertia;
    YamlDynamics6x6Matrix rigid_body_inertia;
    YamlDynamics6x6Matrix added_mass;
    YamlCoordinates hydrodynamic_forces_calculation_point_in_body_frame;
};

#endif /* YAMLDYNAMICS_HPP_ */
