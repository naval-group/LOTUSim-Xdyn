/*
 * generate_body_for_tests.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef SIMULATOR_TEST_HPP_
#define SIMULATOR_TEST_HPP_

#include <string>

#include "xdyn/core/Body.hpp"
#include "external_data_structures/GeometricTypes3d.hpp"

BodyPtr get_body(const std::string& names);
BodyPtr get_body(const std::string& name, const VectorOfVectorOfPoints& points);

#endif
