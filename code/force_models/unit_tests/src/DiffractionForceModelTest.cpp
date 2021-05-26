/*
 * DiffractionForceModelTest.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */

#include <fstream>

#include "DiffractionForceModelTest.hpp"
#include "DiffractionForceModel.hpp"
#include "yaml_data.hpp"
#include "hdb_data.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "YamlWaveModelInput.hpp"
#include "Stretching.hpp"
#include "discretize.hpp"
#include "Airy.hpp"
#include "SurfaceElevationFromWaves.hpp"

#define BODY_NAME "TestShip"

DiffractionForceModelTest::DiffractionForceModelTest() : a(ssc::random_data_generator::DataGenerator(545454))
{
}

DiffractionForceModelTest::~DiffractionForceModelTest()
{
}

void DiffractionForceModelTest::SetUp()
{
}

void DiffractionForceModelTest::TearDown()
{
}

TEST_F(DiffractionForceModelTest, parser)
{
    const YamlDiffraction r = DiffractionForceModel::parse(test_data::diffraction());
    ASSERT_EQ("test_ship.hdb", r.hdb_filename);
    ASSERT_EQ(0.696, r.calculation_point.x);
    ASSERT_EQ(0, r.calculation_point.y);
    ASSERT_EQ(1.418, r.calculation_point.z);
    ASSERT_TRUE(r.mirror);
}

TR1(shared_ptr)<WaveModel> get_wave_model(const double period, const double direction=0., const double height=1.)
{
    const double omega = 2*M_PI/period;
    YamlStretching ys;
    ys.h = 0;
    ys.delta = 1;
    const Stretching ss(ys);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega, 2*height), DiracDirectionalSpreading(direction), omega, omega, 1, ss, false);
    return TR1(shared_ptr)<WaveModel>(new Airy(A, 0.));
}

EnvironmentAndFrames get_waves_env(const double period, const double direction=0., const double amplitude = 1.)
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY_NAME));
    env.w = SurfaceElevationPtr(new SurfaceElevationFromWaves(get_wave_model(period, direction, amplitude)));
    return env;
}

BodyStates get_states_with_forward_speed(const double u = 0)
{
    BodyStates states;
    states.name = BODY_NAME;
    states.convention = YamlRotation("angle", {"z","y'","x''"});
    states.x.record(0, 0);
    states.y.record(0, 0);
    states.z.record(0, 0);
    states.u.record(0, u);
    states.v.record(0, 0);
    states.w.record(0, 0);
    states.p.record(0, 0);
    states.q.record(0, 0);
    states.r.record(0, 0);
    states.qr.record(0, 1);
    states.qi.record(0, 0);
    states.qj.record(0, 0);
    states.qr.record(0, 0);
    return states;
}

TEST_F(DiffractionForceModelTest, encounter_frequency_example)
{
    EnvironmentAndFrames env = get_waves_env(64., 0.);
    BodyStates states;
    std::ofstream hdb_file("data.hdb");
    hdb_file << test_data::bug_3210();
    YamlDiffraction input;
    input.calculation_point = YamlCoordinates(0.696, 0, 1.418);
    input.hdb_filename = "data.hdb";
    input.mirror = true;
    input.use_encounter_period = true;
    const DiffractionForceModel force_model(input, BODY_NAME, env);
    double T = 64;

    // Ship heading is always 0 (due North), encounter period varies with U and wave heading
    // Values from the force model are compared to values from the HDB data (amplitude and phase)

    // First battery of tests: wave direction=0° (due North)
    env = get_waves_env(T, 0.);

    // U=0, wave direction=0° -> encounter period = wave period = 64s, incidence = 0°
    states = get_states_with_forward_speed(0.);
    auto F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(), 3.378373e3*sin(-2.088816), 10);
    ASSERT_NEAR(F.Y(), 0, 10);
    ASSERT_NEAR(F.Z(), -3.282703e5*sin(-1.636404), 20);
    ASSERT_NEAR(F.K(), 0, 10);
    ASSERT_NEAR(F.M(), -2.267054e6*sin(-1.789263), 400);
    ASSERT_NEAR(F.N(), 0, 10);

    // U=48.763, wave direction=0°, wave period = 64s -> encounter period = 125s, incidence = 0°
    states = get_states_with_forward_speed(48.762833661758);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(), 9.472648e2*sin(-2.189364), 10);
    ASSERT_NEAR(F.Y(), 0, 10);
    ASSERT_NEAR(F.Z(), -8.336677e4*sin(-1.596137), 10);
    ASSERT_NEAR(F.K(), 0, 10);
    ASSERT_NEAR(F.M(), -5.817311e5*sin(-1.825686), 110);
    ASSERT_NEAR(F.N(), 0, 10);

    // U=-1498.86, wave direction=0°, wave period = 64s -> encounter period = 4s, incidence = 0°
    states = get_states_with_forward_speed(-1498.8575920622336);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(), 5.084407e4*sin(1.135123), 20);
    ASSERT_NEAR(F.Y(), 0, 10);
    ASSERT_NEAR(F.Z(), -3.997774e5*sin(8.389206e-1), 200);
    ASSERT_NEAR(F.K(), 0, 10);
    ASSERT_NEAR(F.M(), -3.209051e7*sin(8.356066e-1), 15000);// Still a small relative error
    ASSERT_NEAR(F.N(), 0, 10);

    // Second battery of tests: wave direction=-30° (North-West)
    env = get_waves_env(T, -30.*M_PI/180);

    // U=0, wave direction=-30° -> encounter period = wave period = 64s, incidence = 30°
    states = get_states_with_forward_speed(0.);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(), 3.275670e3*sin(-2.028876), 10);
    ASSERT_NEAR(F.Y(), -3.409648e4*sin(3.130008), 30);
    ASSERT_NEAR(F.Z(), -3.283568e5*sin(-1.636047), 20);
    ASSERT_NEAR(F.K(), 1.685674e4*sin(1.350179e-1), 10);
    ASSERT_NEAR(F.M(), -2.255748e6*sin(-1.768062), 300);
    ASSERT_NEAR(F.N(), -4.594584e5*sin(3.053413), 300);

    // U=56.31, wave direction=-30°, wave period = 64s -> encounter period = 125s, incidence = 30°
    states = get_states_with_forward_speed(56.306470282129844);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(), 9.067188e2*sin(-2.123063), 10);
    ASSERT_NEAR(F.Y(), -8.953133e3*sin(3.138567), 10);
    ASSERT_NEAR(F.Z(), -8.339660e4*sin(-1.595086), 10);
    ASSERT_NEAR(F.K(), 4.420171e3*sin(3.533363e-2), 10);
    ASSERT_NEAR(F.M(), -5.772251e5*sin(-1.794767), 100);
    ASSERT_NEAR(F.N(), -1.199319e5*sin(3.118377), 100);

    // U=-1730.73, wave direction=-30°, wave period = 64s -> encounter period = 4s, incidence = 30°
    states = get_states_with_forward_speed(-1730.7316685080896);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(), 8.567009e4*sin(-2.077326), 30);
    ASSERT_NEAR(F.Y(), -3.284304e5*sin(5.459499e-1), 200);
    ASSERT_NEAR(F.Z(), -5.820626e5*sin(-1.525810), 20);
    ASSERT_NEAR(F.K(), 1.460688e6*sin(6.670656e-1), 700);
    ASSERT_NEAR(F.M(), -3.140660e7*sin(-1.375271), 4000);// Still a small relative error
    ASSERT_NEAR(F.N(), -2.301511e7*sin(5.846877e-1), 12000);// Still a small relative error
}


