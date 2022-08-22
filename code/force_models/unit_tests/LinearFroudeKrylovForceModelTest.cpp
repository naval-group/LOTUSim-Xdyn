/*
 * LinearFroudeKrylovForceModelTest.cpp
 *
 *  Created on: July 16, 2021
 *      Author: cady
 */

#include "LinearFroudeKrylovForceModelTest.hpp"
#include "DiffractionForceModelTest.hpp"
#include "LinearFroudeKrylovForceModel.hpp"
#include "hdb_data.hpp"
#include "precal_test_data.hpp"

#include <fstream>

#define BODY_NAME "TestShip"

LinearFroudeKrylovForceModelTest::LinearFroudeKrylovForceModelTest() : a(ssc::random_data_generator::DataGenerator(545454))
{
}

LinearFroudeKrylovForceModelTest::~LinearFroudeKrylovForceModelTest()
{
}

void LinearFroudeKrylovForceModelTest::SetUp()
{
}

void LinearFroudeKrylovForceModelTest::TearDown()
{
}

TEST_F(LinearFroudeKrylovForceModelTest, encounter_frequency_example)
{
    EnvironmentAndFrames env = DiffractionForceModelTest::get_waves_env(64., 0.);
    BodyStates states;
    std::ofstream hdb_file("data.hdb");
    hdb_file << test_data::big_hdb();
    YamlRAO input;
    input.calculation_point = YamlCoordinates(0.696, 0, 1.418);
    input.hdb_filename = "data.hdb";
    input.mirror = true;
    input.use_encounter_period = true;
    input.type_of_rao = YamlRAO::TypeOfRao::FROUDE_KRYLOV_RAO;
    const LinearFroudeKrylovForceModel force_model(input, BODY_NAME, env);
    double T = 64;

    // Ship heading is always 0 (due North), encounter period varies with U and wave heading
    // Values from the force model are compared to values from the HDB data (amplitude and phase)

    // Wave direction=0° (due North)
    env = DiffractionForceModelTest::get_waves_env(T, 0.);

    // U=48.763, wave direction=0°, wave period = 64s -> encounter period = 125s, incidence = 0°
    states = DiffractionForceModelTest::get_states_with_forward_speed(48.762833661758);
    auto F = force_model.get_force(states, 0., env, {});
    const double kx = 0.000683817;
    ASSERT_NEAR( F.X(), 2.164523E+04 * sin(-(1.570774E+00 - kx)), 10);
    ASSERT_NEAR(-F.Y(), 0.000000E+00 * sin(-(-3.141593E+00 - kx)), 10);
    ASSERT_NEAR(-F.Z(), 2.106490E+07 * sin(-(1.577854E-03 - kx)), 10);
    ASSERT_NEAR( F.K(), 0.000000E+00 * sin(-(-3.141593E+00 - kx)), 10);
    ASSERT_NEAR(-F.M(), 1.291984E+08 * sin(-(5.123568E-02 - kx)), 110);
    ASSERT_NEAR(-F.N(), 0.000000E+00 * sin(-(-3.141593E+00 - kx)), 10);
}

double LinearFroudeKrylovForceModelTest::small_relative_error(const double val) const
{
    const double eps = 1E-4;
    return std::max(eps, std::abs(val * eps));
}

TEST_F(LinearFroudeKrylovForceModelTest, precal_r_example)
{
    EnvironmentAndFrames env = DiffractionForceModelTest::get_waves_env(10.47198, 0.);
    BodyStates states;
    std::ofstream precalr_file("data.raodb.ini");
    precalr_file << test_data::precal();
    YamlRAO input;
    input.calculation_point = YamlCoordinates(0, 0, 0);
    input.precal_filename = "data.raodb.ini";
    input.mirror = true;
    input.use_encounter_period = true;
    input.type_of_rao = YamlRAO::TypeOfRao::FROUDE_KRYLOV_RAO;
    const LinearFroudeKrylovForceModel force_model(input, BODY_NAME, env);
    double T = 10.47198;

    // Ship heading is always 0 (due North), encounter period varies with U and wave heading
    // Values from the force model are compared to values from the PRECAL_R data (amplitude and phase)

    // First battery of tests: wave direction=-180° (due South)
    env = DiffractionForceModelTest::get_waves_env(T, -M_PI);

    // U=0, wave direction=-180° -> encounter period = wave period = 10.47198s (frequency = 0.6 rad/s => third line), incidence = 180°
    states = DiffractionForceModelTest::get_states_with_forward_speed(0.);
    auto F = force_model.get_force(states, 0., env, {});

    ASSERT_NEAR( F.X(), 0.137233E+04 * 1e3 * sin(- (-91.060349) * M_PI / 180.), small_relative_error(F.X()));
    ASSERT_NEAR(-F.Y(), 0.185689E-02 * 1e3 * sin(- ( 99.457497) * M_PI / 180.), small_relative_error(F.Y()));
    ASSERT_NEAR(-F.Z(), 0.672921E+04 * 1e3 * sin(- (-16.163645) * M_PI / 180.), small_relative_error(F.Z()));
    ASSERT_NEAR( F.K(), 0.122369E-01 * 1e3 * sin(- (129.387329) * M_PI / 180.), small_relative_error(F.K()));
    ASSERT_NEAR(-F.M(), 0.459008E+06 * 1e3 * sin(- (-97.283081) * M_PI / 180.), small_relative_error(F.M()));
    ASSERT_NEAR(-F.N(), 0.120006E+00 * 1e3 * sin(- (-96.079956) * M_PI / 180.), small_relative_error(F.N()));
}
