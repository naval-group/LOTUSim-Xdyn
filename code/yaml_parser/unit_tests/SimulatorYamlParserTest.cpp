/*
 * SimulatorYamlParserTest.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#include "gmock/gmock.h"
#include <boost/algorithm/string.hpp> // For boost::replace_all
using namespace testing; // So we can use 'ElementsAre' unqualified
#include "yaml_parser/external_data_structures_parsers.hpp"
#include "SimulatorYamlParserTest.hpp"
#include "test_data_generator/yaml_data.hpp"
#include "yaml_parser/SimulatorYamlParser.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "parse_controllers.hpp"
#include "parse_time_series.hpp"
#include "precal_test_data.hpp"

const YamlSimulatorInput SimulatorYamlParserTest::yaml = SimulatorYamlParser(test_data::full_example_with_propulsion()).parse();
const YamlSimulatorInput SimulatorYamlParserTest::old_yaml = SimulatorYamlParser(test_data::full_example_with_propulsion_and_old_key_name()).parse();

SimulatorYamlParserTest::SimulatorYamlParserTest() : a(ssc::random_data_generator::DataGenerator(1212))
{
}

SimulatorYamlParserTest::~SimulatorYamlParserTest()
{
}

void SimulatorYamlParserTest::SetUp()
{
}

void SimulatorYamlParserTest::TearDown()
{
}

TEST_F(SimulatorYamlParserTest, can_parse_rotations)
{
    const YamlRotation rot = yaml.rotations;
    ASSERT_EQ("angle", rot.order_by);
    ASSERT_EQ(3, rot.convention.size());
    ASSERT_EQ("z", rot.convention.at(0));
    ASSERT_EQ("y'", rot.convention.at(1));
    ASSERT_EQ("x''", rot.convention.at(2));
}

TEST_F(SimulatorYamlParserTest, can_parse_environment)
{
    ASSERT_EQ(2, yaml.environment.size());
    const std::vector<YamlModel> env = yaml.environment;
    ASSERT_EQ("no wind", env.at(0).model);
    ASSERT_EQ("no waves", env.at(1).model);
    ASSERT_EQ("constant sea elevation in NED frame:\n  unit: m\n  value: 12\nmodel: no waves", env.at(1).yaml);
}

TEST_F(SimulatorYamlParserTest, can_parse_environmental_constants)
{
    ASSERT_DOUBLE_EQ(9.81, yaml.environmental_constants.g);
    ASSERT_DOUBLE_EQ(1000, yaml.environmental_constants.rho);
    ASSERT_DOUBLE_EQ(1.18e-6, yaml.environmental_constants.nu);
    ASSERT_TRUE(yaml.environmental_constants.rho_air.is_initialized());
    ASSERT_DOUBLE_EQ(1.225, yaml.environmental_constants.rho_air.get());
}

TEST_F(SimulatorYamlParserTest, can_parse_external_forces)
{
    ASSERT_EQ(5, yaml.bodies.at(0).external_forces.size());
    ASSERT_EQ("gravity", yaml.bodies.at(0).external_forces.at(0).model);
    ASSERT_EQ("non-linear hydrostatic (fast)", yaml.bodies.at(0).external_forces.at(1).model);
    ASSERT_EQ("quadratic damping", yaml.bodies.at(0).external_forces.at(2).model);
    ASSERT_EQ("wageningen B-series", yaml.bodies.at(0).external_forces.at(3).model);
    ASSERT_EQ("wageningen B-series", yaml.bodies.at(0).external_forces.at(4).model);
}

TEST_F(SimulatorYamlParserTest, can_parse_bodies)
{
    ASSERT_EQ(1, yaml.bodies.size());
    ASSERT_EQ("body 1", yaml.bodies.at(0).name);
    ASSERT_EQ("test_ship.stl", yaml.bodies.at(0).mesh);
}

TEST_F(SimulatorYamlParserTest, can_parse_position_of_body_frame_relative_to_mesh)
{
    ASSERT_DOUBLE_EQ(1,yaml.bodies.front().position_of_body_frame_relative_to_mesh.angle.phi);
    ASSERT_DOUBLE_EQ(3,yaml.bodies.front().position_of_body_frame_relative_to_mesh.angle.theta);
    ASSERT_DOUBLE_EQ(2,yaml.bodies.front().position_of_body_frame_relative_to_mesh.angle.psi);
    ASSERT_EQ("mesh",yaml.bodies.front().position_of_body_frame_relative_to_mesh.frame);
    ASSERT_DOUBLE_EQ(10,yaml.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.x);
    ASSERT_DOUBLE_EQ(0.21,yaml.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.y);
    ASSERT_DOUBLE_EQ(33e3,yaml.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.z);
}

TEST_F(SimulatorYamlParserTest, can_parse_initial_position_of_body_frame_relative_to_NED)
{
    ASSERT_EQ("NED",yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.frame);
    ASSERT_DOUBLE_EQ(4,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.x);
    ASSERT_DOUBLE_EQ(8,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.y);
    ASSERT_DOUBLE_EQ(12,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.z);
    ASSERT_DOUBLE_EQ(1.3,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.phi);
    ASSERT_DOUBLE_EQ(1.4,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.theta);
    ASSERT_DOUBLE_EQ(1.5,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.psi);
}

TEST_F(SimulatorYamlParserTest, can_parse_initial_velocity_of_body_frame_relative_to_NED)
{
    ASSERT_EQ("body 1",yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.frame);
    ASSERT_DOUBLE_EQ(-8,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u);
    ASSERT_DOUBLE_EQ(-9,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.v);
    ASSERT_DOUBLE_EQ(14,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.w);
    ASSERT_DOUBLE_EQ(56,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.p);
    ASSERT_DOUBLE_EQ(7,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.q);
    ASSERT_DOUBLE_EQ(6,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.r);
}

TEST_F(SimulatorYamlParserTest, can_parse_centre_of_inertia)
{
    ASSERT_DOUBLE_EQ(4,yaml.bodies.front().dynamics.centre_of_inertia.x);
    ASSERT_DOUBLE_EQ(7,yaml.bodies.front().dynamics.centre_of_inertia.y);
    ASSERT_DOUBLE_EQ(-10,yaml.bodies.front().dynamics.centre_of_inertia.z);
    ASSERT_EQ("body 1",yaml.bodies.front().dynamics.centre_of_inertia.frame);
    ASSERT_EQ("centre of inertia",yaml.bodies.front().dynamics.centre_of_inertia.name);
}

TEST_F(SimulatorYamlParserTest, can_parse_inertia_matrix)
{
    const YamlDynamics6x6Matrix M = yaml.bodies.front().dynamics.rigid_body_inertia;
    ASSERT_EQ("body 1", M.frame);
    ASSERT_EQ(6, M.row_1.size());
    ASSERT_EQ(6, M.row_2.size());
    ASSERT_EQ(6, M.row_3.size());
    ASSERT_EQ(6, M.row_4.size());
    ASSERT_EQ(6, M.row_5.size());
    ASSERT_EQ(6, M.row_6.size());
    ASSERT_DOUBLE_EQ(1, M.row_1.at(0));
    ASSERT_DOUBLE_EQ(2, M.row_1.at(1));
    ASSERT_DOUBLE_EQ(3, M.row_1.at(2));
    ASSERT_DOUBLE_EQ(4, M.row_1.at(3));
    ASSERT_DOUBLE_EQ(5, M.row_1.at(4));
    ASSERT_DOUBLE_EQ(6, M.row_1.at(5));
    ASSERT_DOUBLE_EQ(7, M.row_2.at(0));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(1));
    ASSERT_DOUBLE_EQ(1, M.row_2.at(2));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(3));
    ASSERT_DOUBLE_EQ(11, M.row_2.at(4));
    ASSERT_DOUBLE_EQ(13, M.row_2.at(5));
    ASSERT_DOUBLE_EQ(20, M.row_3.at(0));
    ASSERT_DOUBLE_EQ(30, M.row_3.at(1));
    ASSERT_DOUBLE_EQ(3, M.row_3.at(2));
    ASSERT_DOUBLE_EQ(80, M.row_3.at(3));
    ASSERT_DOUBLE_EQ(0.9, M.row_3.at(4));
    ASSERT_DOUBLE_EQ(0.1, M.row_3.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(2));
    ASSERT_DOUBLE_EQ(4, M.row_4.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(3));
    ASSERT_DOUBLE_EQ(5, M.row_5.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(4));
    ASSERT_DOUBLE_EQ(6, M.row_6.at(5));
}

TEST_F(SimulatorYamlParserTest, can_parse_added_mass_matrix)
{
    const YamlDynamics6x6Matrix M =  yaml.bodies.front().dynamics.added_mass;
    ASSERT_EQ("body 1", M.frame);
    ASSERT_EQ(6, M.row_1.size());
    ASSERT_EQ(6, M.row_2.size());
    ASSERT_EQ(6, M.row_3.size());
    ASSERT_EQ(6, M.row_4.size());
    ASSERT_EQ(6, M.row_5.size());
    ASSERT_EQ(6, M.row_6.size());
    ASSERT_DOUBLE_EQ(1, M.row_1.at(0));
    ASSERT_DOUBLE_EQ(2, M.row_1.at(1));
    ASSERT_DOUBLE_EQ(3, M.row_1.at(2));
    ASSERT_DOUBLE_EQ(4, M.row_1.at(3));
    ASSERT_DOUBLE_EQ(5, M.row_1.at(4));
    ASSERT_DOUBLE_EQ(6, M.row_1.at(5));
    ASSERT_DOUBLE_EQ(-7, M.row_2.at(0));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(1));
    ASSERT_DOUBLE_EQ(1, M.row_2.at(2));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(3));
    ASSERT_DOUBLE_EQ(11, M.row_2.at(4));
    ASSERT_DOUBLE_EQ(13, M.row_2.at(5));
    ASSERT_DOUBLE_EQ(-20, M.row_3.at(0));
    ASSERT_DOUBLE_EQ(30, M.row_3.at(1));
    ASSERT_DOUBLE_EQ(3, M.row_3.at(2));
    ASSERT_DOUBLE_EQ(80, M.row_3.at(3));
    ASSERT_DOUBLE_EQ(0.9, M.row_3.at(4));
    ASSERT_DOUBLE_EQ(0.1, M.row_3.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(2));
    ASSERT_DOUBLE_EQ(4.1, M.row_4.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(3));
    ASSERT_DOUBLE_EQ(5.2, M.row_5.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(4));
    ASSERT_DOUBLE_EQ(6.3, M.row_6.at(5));
}

TEST_F(SimulatorYamlParserTest, can_parse_added_mass_matrix_from_hdb_file)
{
    const YamlDynamics6x6Matrix M =  yaml.bodies.front().dynamics.added_mass;
    const YamlSimulatorInput input = SimulatorYamlParser(test_data::added_mass_from_hdb_file()).parse();
    ASSERT_TRUE(input.bodies.front().dynamics.added_mass.read_from_file);
    ASSERT_EQ("test_ship.hdb", input.bodies.front().dynamics.added_mass.hdb_filename);
}

TEST_F(SimulatorYamlParserTest, should_get_an_error_message_when_using_the_old_key_for_hdb_files)
{
    bool exception_thrown = false;
    try {
        std::string yaml_data = test_data::added_mass_from_precal_file();
        boost::replace_all(yaml_data, "from raodb:", "from precal:");
        const YamlSimulatorInput input = SimulatorYamlParser(yaml_data).parse();
        ASSERT_TRUE(input.bodies.front().dynamics.added_mass.read_from_file);
        ASSERT_EQ("test_ship.hdb", input.bodies.front().dynamics.added_mass.hdb_filename);
    }
    catch(const InvalidInputException& exception) {
        ASSERT_EQ("Error parsing 'body' section (body 'body 1'): In node 'added mass matrix at the "
                  "center of gravity and projected in the body frame': You used YAML key 'from "
                  "precal' but this key is deprecated: you should use 'from hdb' instead.",
                  exception.get_message());
        exception_thrown = true;
    }
    if (!exception_thrown) {
        ASSERT_FALSE(true) << "No exception thrown!";
    }
}

TEST_F(SimulatorYamlParserTest, can_parse_added_mass_matrix_from_precal_file)
{
    const std::string filename = "ONRT_SIMMAN.raodb.ini";
    const YamlSimulatorInput input = SimulatorYamlParser(test_data::added_mass_from_precal_file()).parse();
    ASSERT_TRUE(input.bodies.front().dynamics.added_mass.read_from_file);
    ASSERT_EQ(filename, input.bodies.front().dynamics.added_mass.precal_filename);
}

TEST_F(SimulatorYamlParserTest, should_not_throw_even_if_no_mesh_is_defined)
{
    ASSERT_NO_THROW(SimulatorYamlParser(test_data::falling_ball_example()).parse());
}

TEST_F(SimulatorYamlParserTest, can_have_test_data_with_just_waves)
{
    const YamlSimulatorInput input = SimulatorYamlParser(test_data::waves()).parse();
    ASSERT_EQ(1,input.environment.size());
    ASSERT_EQ("waves",input.environment[0].model);
}

TEST_F(SimulatorYamlParserTest, can_parse_hydrodynamic_calculation_point)
{
    const YamlCoordinates P = yaml.bodies.front().dynamics.hydrodynamic_forces_calculation_point_in_body_frame;
    ASSERT_DOUBLE_EQ(0.696, P.x);
    ASSERT_DOUBLE_EQ(0, P.y);
    ASSERT_DOUBLE_EQ(1.418, P.z);
}

TEST_F(SimulatorYamlParserTest, can_parse_forced_dof)
{
    const std::string yaml = "blocked dof:\n"
                             "   from CSV:\n"
                             "     - state: u\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [4.2]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n";
    const YamlBlockedDOF input = parse(yaml);
    ASSERT_EQ(1, input.from_yaml.size());
    ASSERT_EQ(BlockableState::P, input.from_yaml.front().state);
    ASSERT_EQ(InterpolationType::PIECEWISE_CONSTANT, input.from_yaml.front().interpolation);
    ASSERT_THAT(input.from_yaml.front().t, ElementsAre(4.2));
    ASSERT_THAT(input.from_yaml.front().value, ElementsAre(5));
    ASSERT_EQ(1, input.from_csv.size());
    ASSERT_EQ(BlockableState::U, input.from_csv.front().state);
    ASSERT_EQ(InterpolationType::SPLINE, input.from_csv.front().interpolation);
    ASSERT_EQ("T", input.from_csv.front().t);
    ASSERT_EQ("PS", input.from_csv.front().value);
    ASSERT_EQ("test.csv", input.from_csv.front().filename);
}

TEST_F(SimulatorYamlParserTest, can_parse_blocked_DOF_even_if_there_is_nothing_to_parse)
{
    const auto y  = parse("");
    ASSERT_TRUE(y.from_csv.empty());
    ASSERT_TRUE(y.from_yaml.empty());
}

TEST_F(SimulatorYamlParserTest, should_throw_if_forcing_anything_other_than_uvwpqr)
{
    const std::string yaml = "blocked dof:\n"
                             "   from CSV:\n"
                             "     - state: x\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [4.2]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n";
    ASSERT_THROW(parse(yaml), InvalidInputException);
}

TEST_F(SimulatorYamlParserTest, interpolation_type_should_be_valid)
{
    const std::string invalid_yaml =
                                 "blocked dof:\n"
                                 "   from YAML:\n"
                                 "     - state: p\n"
                                 "       t: [5,4.2]\n"
                                 "       value: [5,6]\n"
                                 "       interpolation: something\n";
    ASSERT_THROW(parse(invalid_yaml), InvalidInputException);
}

TEST_F(SimulatorYamlParserTest, bug_3235)
{
    ASSERT_NO_THROW(SimulatorYamlParser(test_data::bug_3235()).parse());
}

TEST_F(SimulatorYamlParserTest, can_parse_rigid_body_inertia_matrix)
{
    ASSERT_EQ("body 1", yaml.bodies.at(0).dynamics.rigid_body_inertia.frame);
    ASSERT_EQ("", yaml.bodies.at(0).dynamics.rigid_body_inertia.hdb_filename);
    ASSERT_FALSE(yaml.bodies.at(0).dynamics.rigid_body_inertia.read_from_file);
    ASSERT_EQ(6, yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.size());
    ASSERT_DOUBLE_EQ(1,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(0));
    ASSERT_DOUBLE_EQ(2,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(1));
    ASSERT_DOUBLE_EQ(3,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(2));
    ASSERT_DOUBLE_EQ(4,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(3));
    ASSERT_DOUBLE_EQ(5,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(4));
    ASSERT_DOUBLE_EQ(6,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(5));
    ASSERT_DOUBLE_EQ(7,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(0));
    ASSERT_DOUBLE_EQ(2,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(1));
    ASSERT_DOUBLE_EQ(1,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(2));
    ASSERT_DOUBLE_EQ(2,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(3));
    ASSERT_DOUBLE_EQ(11,  yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(4));
    ASSERT_DOUBLE_EQ(13,  yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(5));
    ASSERT_DOUBLE_EQ(20,  yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(0));
    ASSERT_DOUBLE_EQ(30,  yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(1));
    ASSERT_DOUBLE_EQ(3,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(2));
    ASSERT_DOUBLE_EQ(80,  yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(3));
    ASSERT_DOUBLE_EQ(0.9, yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(4));
    ASSERT_DOUBLE_EQ(0.1, yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(5));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(0));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(1));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(2));
    ASSERT_DOUBLE_EQ(4,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(3));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(4));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(5));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(0));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(1));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(2));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(3));
    ASSERT_DOUBLE_EQ(5,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(4));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(5));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(0));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(1));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(2));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(3));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(4));
    ASSERT_DOUBLE_EQ(6,   yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(5));
}

TEST_F(SimulatorYamlParserTest, can_parse_rigid_body_inertia_matrix_with_old_key_name)
{
    ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.frame, old_yaml.bodies.at(0).dynamics.rigid_body_inertia.frame);
    ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.hdb_filename, old_yaml.bodies.at(0).dynamics.rigid_body_inertia.hdb_filename);
    ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.read_from_file, old_yaml.bodies.at(0).dynamics.rigid_body_inertia.read_from_file);
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(i));
    }
}

TEST_F(SimulatorYamlParserTest, can_parse_added_mass_inertia_matrix)
{
    ASSERT_EQ("body 1", yaml.bodies.at(0).dynamics.added_mass.frame);
    ASSERT_EQ("", yaml.bodies.at(0).dynamics.added_mass.hdb_filename);
    ASSERT_FALSE(yaml.bodies.at(0).dynamics.added_mass.read_from_file);
    ASSERT_EQ(6, yaml.bodies.at(0).dynamics.added_mass.row_1.size());
    ASSERT_DOUBLE_EQ(1,   yaml.bodies.at(0).dynamics.added_mass.row_1.at(0));
    ASSERT_DOUBLE_EQ(2,   yaml.bodies.at(0).dynamics.added_mass.row_1.at(1));
    ASSERT_DOUBLE_EQ(3,   yaml.bodies.at(0).dynamics.added_mass.row_1.at(2));
    ASSERT_DOUBLE_EQ(4,   yaml.bodies.at(0).dynamics.added_mass.row_1.at(3));
    ASSERT_DOUBLE_EQ(5,   yaml.bodies.at(0).dynamics.added_mass.row_1.at(4));
    ASSERT_DOUBLE_EQ(6,   yaml.bodies.at(0).dynamics.added_mass.row_1.at(5));
    ASSERT_DOUBLE_EQ(-7,   yaml.bodies.at(0).dynamics.added_mass.row_2.at(0));
    ASSERT_DOUBLE_EQ(2,   yaml.bodies.at(0).dynamics.added_mass.row_2.at(1));
    ASSERT_DOUBLE_EQ(1,   yaml.bodies.at(0).dynamics.added_mass.row_2.at(2));
    ASSERT_DOUBLE_EQ(2,   yaml.bodies.at(0).dynamics.added_mass.row_2.at(3));
    ASSERT_DOUBLE_EQ(11,  yaml.bodies.at(0).dynamics.added_mass.row_2.at(4));
    ASSERT_DOUBLE_EQ(13,  yaml.bodies.at(0).dynamics.added_mass.row_2.at(5));
    ASSERT_DOUBLE_EQ(-20,  yaml.bodies.at(0).dynamics.added_mass.row_3.at(0));
    ASSERT_DOUBLE_EQ(30,  yaml.bodies.at(0).dynamics.added_mass.row_3.at(1));
    ASSERT_DOUBLE_EQ(3,   yaml.bodies.at(0).dynamics.added_mass.row_3.at(2));
    ASSERT_DOUBLE_EQ(80,  yaml.bodies.at(0).dynamics.added_mass.row_3.at(3));
    ASSERT_DOUBLE_EQ(0.9, yaml.bodies.at(0).dynamics.added_mass.row_3.at(4));
    ASSERT_DOUBLE_EQ(0.1, yaml.bodies.at(0).dynamics.added_mass.row_3.at(5));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_4.at(0));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_4.at(1));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_4.at(2));
    ASSERT_DOUBLE_EQ(4.1,   yaml.bodies.at(0).dynamics.added_mass.row_4.at(3));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_4.at(4));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_4.at(5));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_5.at(0));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_5.at(1));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_5.at(2));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_5.at(3));
    ASSERT_DOUBLE_EQ(5.2,   yaml.bodies.at(0).dynamics.added_mass.row_5.at(4));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_5.at(5));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_6.at(0));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_6.at(1));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_6.at(2));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_6.at(3));
    ASSERT_DOUBLE_EQ(0,   yaml.bodies.at(0).dynamics.added_mass.row_6.at(4));
    ASSERT_DOUBLE_EQ(6.3,   yaml.bodies.at(0).dynamics.added_mass.row_6.at(5));
}

TEST_F(SimulatorYamlParserTest, can_parse_added_mass_matrix_with_old_key_name)
{
    ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.frame, old_yaml.bodies.at(0).dynamics.rigid_body_inertia.frame);
    ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.hdb_filename, old_yaml.bodies.at(0).dynamics.rigid_body_inertia.hdb_filename);
    ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.read_from_file, old_yaml.bodies.at(0).dynamics.rigid_body_inertia.read_from_file);
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_1.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_2.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_3.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_4.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_5.at(i));
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        ASSERT_EQ(yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(i), old_yaml.bodies.at(0).dynamics.rigid_body_inertia.row_6.at(i));
    }
}

TEST_F(SimulatorYamlParserTest, can_parse_controllers_and_commands_for_controlled_forces)
{
    const YamlSimulatorInput yaml = SimulatorYamlParser(
        test_data::falling_ball_example() +
        test_data::setpoints() +
        test_data::controllers() +
        test_data::grpc_controller() +
        test_data::controlled_forces()
    ).parse();

    ASSERT_EQ(2, yaml.setpoints.size());
    ASSERT_EQ("", yaml.setpoints[0].name);
    ASSERT_EQ("", yaml.setpoints[1].name);

    ASSERT_EQ(3, yaml.controllers.size());
    ASSERT_EQ("pId", yaml.controllers[0].type);
    ASSERT_EQ(0.1, yaml.controllers[0].dt);
    ASSERT_EQ("gRPC", yaml.controllers[2].type);
    ASSERT_EQ(0.01, yaml.controllers[2].dt);
    ASSERT_EQ("PID", yaml.controllers[1].type);
    ASSERT_EQ(0.5, yaml.controllers[1].dt);

    ASSERT_EQ(1, yaml.commands.size());
    ASSERT_EQ("propeller", yaml.commands[0].name);
}


TEST_F(SimulatorYamlParserTest, can_parse_filtered_states)
{
    const YamlSimulatorInput yaml = SimulatorYamlParser(test_data::tutorial_10_gRPC_force_model()).parse();
    ASSERT_TRUE(yaml.bodies.front().filtered_states.x.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.y.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.z.empty());
    ASSERT_EQ("duration in seconds: 2.3\ntype of filter: moving average", yaml.bodies.front().filtered_states.u);
    ASSERT_EQ("duration in seconds: 3\ntype of filter: moving average", yaml.bodies.front().filtered_states.v);
    ASSERT_TRUE(yaml.bodies.front().filtered_states.w.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.p.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.q.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.r.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.phi.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.theta.empty());
    ASSERT_TRUE(yaml.bodies.front().filtered_states.psi.empty());
}
