/*
 * calculate_hash.cpp
 *
 *  Created on: May 19, 2015
 *      Author: cady
 */

#include <boost/functional/hash.hpp> //std::hash_range, std::hash_combine

#include "calculate_hash.hpp"


std::size_t hash_value(const YamlRotation& yaml)
{
    std::size_t seed = 0;
    boost::hash_range(seed, yaml.convention.begin(), yaml.convention.end());
    boost::hash_combine(seed, yaml.order_by);
    return seed;
}

std::size_t hash_value(const YamlEnvironmentalConstants& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.g);
    boost::hash_combine(seed, yaml.nu);
    boost::hash_combine(seed, yaml.rho);
    return seed;
}

std::size_t hash_value(const YamlModel& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.model);
    boost::hash_combine(seed, yaml.yaml);
    return seed;
}

std::size_t hash_value(const YamlBody& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.dynamics);
    boost::hash_range(seed, yaml.external_forces.begin(), yaml.external_forces.end());
    boost::hash_combine(seed, yaml.initial_position_of_body_frame_relative_to_NED_projected_in_NED);
    boost::hash_combine(seed, yaml.initial_velocity_of_body_frame_relative_to_NED_projected_in_body);
    boost::hash_combine(seed, yaml.mesh);
    boost::hash_combine(seed, yaml.name);
    boost::hash_combine(seed, yaml.position_of_body_frame_relative_to_mesh);
    return seed;
}

std::size_t hash_value(const YamlSpeed& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.frame);
    boost::hash_combine(seed, yaml.u);
    boost::hash_combine(seed, yaml.v);
    boost::hash_combine(seed, yaml.w);
    boost::hash_combine(seed, yaml.p);
    boost::hash_combine(seed, yaml.q);
    boost::hash_combine(seed, yaml.r);
    return seed;
}

std::size_t hash_value(const YamlDynamics& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.added_mass);
    boost::hash_combine(seed, yaml.centre_of_inertia);
    boost::hash_combine(seed, yaml.hydrodynamic_forces_calculation_point_in_body_frame);
    boost::hash_combine(seed, yaml.rigid_body_inertia);
    return seed;
}

std::size_t hash_value(const YamlDynamics6x6Matrix& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.frame);
    boost::hash_combine(seed, yaml.hdb_filename);
    boost::hash_combine(seed, yaml.read_from_file);
    boost::hash_range(seed, yaml.row_1.begin(), yaml.row_1.end());
    boost::hash_range(seed, yaml.row_2.begin(), yaml.row_2.end());
    boost::hash_range(seed, yaml.row_3.begin(), yaml.row_3.end());
    boost::hash_range(seed, yaml.row_4.begin(), yaml.row_4.end());
    boost::hash_range(seed, yaml.row_5.begin(), yaml.row_5.end());
    boost::hash_range(seed, yaml.row_6.begin(), yaml.row_6.end());
    return seed;
}

std::size_t hash_value(const YamlPoint& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.frame);
    boost::hash_combine(seed, yaml.name);
    boost::hash_combine(seed, yaml.x);
    boost::hash_combine(seed, yaml.y);
    boost::hash_combine(seed, yaml.z);
    return seed;
}

std::size_t hash_value(const YamlAngle& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.phi);
    boost::hash_combine(seed, yaml.theta);
    boost::hash_combine(seed, yaml.psi);
    return seed;
}

std::size_t hash_value(const YamlPosition& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.angle);
    boost::hash_combine(seed, yaml.coordinates);
    boost::hash_combine(seed, yaml.frame);
    return seed;
}

std::size_t hash_value(const YamlCoordinates& yaml)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, yaml.x);
    boost::hash_combine(seed, yaml.y);
    boost::hash_combine(seed, yaml.z);
    return seed;
}

std::size_t hash_value(const YamlSimulatorInput& yaml)
{
    std::size_t seed = 0;
    boost::hash_range(seed, yaml.bodies.begin(), yaml.bodies.end());
    boost::hash_range(seed, yaml.environment.begin(), yaml.environment.end());
    boost::hash_combine(seed, yaml.environmental_constants);
    boost::hash_combine(seed, yaml.rotations);
    return seed;
}
