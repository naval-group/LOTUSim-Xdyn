#include "Sha.hpp"

template <> void Sha1::append(const std::string&s)
{
    sha1.process_bytes(s.c_str(), s.size());
    up_to_date=false;
}

void sha1_append(Sha1& sha1, const YamlRotation& yaml)
{
    sha1.append(yaml.convention);
    sha1.append(yaml.order_by);
}

void sha1_append(Sha1& sha1, const YamlEnvironmentalConstants& yaml)
{
    sha1.append(yaml.g);
    sha1.append(yaml.nu);
    sha1.append(yaml.rho);
}

void sha1_append(Sha1& sha1, const YamlModel& yaml)
{
    sha1.append(yaml.model);
    sha1.append(yaml.yaml);
}

void sha1_append(Sha1& sha1, const YamlBody& yaml)
{
    for (const auto& cf:yaml.external_forces) sha1_append(sha1, cf);
    sha1_append(sha1,yaml.dynamics);
    sha1_append(sha1, yaml.initial_position_of_body_frame_relative_to_NED_projected_in_NED);
    sha1_append(sha1, yaml.initial_velocity_of_body_frame_relative_to_NED_projected_in_body);
    sha1.append(yaml.mesh);
    sha1.append(yaml.name);
    sha1_append(sha1, yaml.position_of_body_frame_relative_to_mesh);
}

void sha1_append(Sha1& sha1, const YamlSpeed& yaml)
{
    sha1.append(yaml.frame);
    sha1.append(yaml.u);
    sha1.append(yaml.v);
    sha1.append(yaml.w);
    sha1.append(yaml.p);
    sha1.append(yaml.q);
    sha1.append(yaml.r);
}

void sha1_append(Sha1& sha1, const YamlDynamics& yaml)
{
    sha1_append(sha1, yaml.added_mass);
    sha1_append(sha1, yaml.centre_of_inertia);
    sha1_append(sha1, yaml.hydrodynamic_forces_calculation_point_in_body_frame);
    sha1_append(sha1, yaml.rigid_body_inertia);
}

void sha1_append(Sha1& sha1, const YamlDynamics6x6Matrix& yaml)
{
    sha1.append(yaml.frame);
    sha1.append(yaml.hdb_filename);
    sha1.append(yaml.read_from_file);
    sha1.append(yaml.row_1);
    sha1.append(yaml.row_2);
    sha1.append(yaml.row_3);
    sha1.append(yaml.row_4);
    sha1.append(yaml.row_5);
    sha1.append(yaml.row_6);
}

void sha1_append(Sha1& sha1, const YamlPoint& yaml)
{
    sha1.append(yaml.frame);
    sha1.append(yaml.name);
    sha1.append(yaml.x);
    sha1.append(yaml.y);
    sha1.append(yaml.z);
}

void sha1_append(Sha1& sha1, const YamlAngle& yaml)
{
    sha1.append(yaml.phi);
    sha1.append(yaml.theta);
    sha1.append(yaml.psi);
}

void sha1_append(Sha1& sha1, const YamlPosition& yaml)
{
    sha1_append(sha1, yaml.angle);
    sha1_append(sha1, yaml.coordinates);
    sha1.append(yaml.frame);
}

void sha1_append(Sha1& sha1, const YamlCoordinates& yaml)
{
    sha1.append(yaml.x);
    sha1.append(yaml.y);
    sha1.append(yaml.z);
}

void sha1_append(Sha1& sha1, const YamlSimulatorInput& yaml)
{
    for (const auto& body:yaml.bodies) sha1_append(sha1, body);
    for (const auto& env:yaml.environment) sha1_append(sha1, env);
    sha1_append(sha1, yaml.environmental_constants);
    sha1_append(sha1, yaml.rotations);
}
