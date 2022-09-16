/*
 * generate_body_for_tests.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "xdyn/core/unit_tests/generate_body_for_tests.hpp"
#include "xdyn/external_data_structures/YamlRotation.hpp"
#include "xdyn/core/BodyBuilder.hpp"
#include "xdyn/test_data_generator/TriMeshTestData.hpp"

BodyPtr get_body(const std::string& name)
{
    return get_body(name, two_triangles());
}

BodyPtr get_body(const std::string& name, const VectorOfVectorOfPoints& points)

{
    YamlRotation rot;
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    rot.order_by = "angle";
    return BodyBuilder(rot).build(name, points, 0, 0, rot, true);
}
