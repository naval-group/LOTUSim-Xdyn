/*
 * calculate_hash.hpp
 *
 *  Created on: May 19, 2015
 *      Author: cady
 */

#ifndef CALCULATE_HASH_HPP_
#define CALCULATE_HASH_HPP_

#include "YamlRotation.hpp"
#include "YamlEnvironmentalConstants.hpp"
#include "YamlModel.hpp"
#include "YamlBody.hpp"
#include "YamlDynamics.hpp"
#include "YamlDynamics6x6Matrix.hpp"
#include "YamlPoint.hpp"
#include "YamlCoordinates.hpp"
#include "YamlSimulatorInput.hpp"

std::size_t hash_value(const YamlRotation& yaml);
std::size_t hash_value(const YamlEnvironmentalConstants& yaml);
std::size_t hash_value(const YamlModel& yaml);
std::size_t hash_value(const YamlBody& yaml);
std::size_t hash_value(const YamlDynamics& yaml);
std::size_t hash_value(const YamlDynamics6x6Matrix& yaml);
std::size_t hash_value(const YamlPoint& yaml);
std::size_t hash_value(const YamlPosition& yaml);
std::size_t hash_value(const YamlCoordinates& yaml);
std::size_t hash_value(const YamlSimulatorInput& yaml);
std::size_t hash_value(const YamlAngle& yaml);
std::size_t hash_value(const YamlSpeed& yaml);


#endif  /* CALCULATE_HASH_HPP_ */
