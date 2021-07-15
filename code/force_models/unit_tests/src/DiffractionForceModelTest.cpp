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
#include "precal_test_data.hpp"
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

TEST_F(DiffractionForceModelTest, parser_hdb)
{
    const YamlDiffraction r = DiffractionForceModel::parse(test_data::diffraction());
    ASSERT_EQ("test_ship.hdb", r.hdb_filename);
    ASSERT_TRUE(r.precal_filename.empty());
    ASSERT_EQ(0.696, r.calculation_point.x);
    ASSERT_EQ(0, r.calculation_point.y);
    ASSERT_EQ(1.418, r.calculation_point.z);
    ASSERT_TRUE(r.mirror);
    ASSERT_TRUE(r.use_encounter_period);
}

TEST_F(DiffractionForceModelTest, parser_precalr)
{
    const YamlDiffraction r = DiffractionForceModel::parse(test_data::diffraction_precalr());
    ASSERT_EQ("test_ship.raodb.ini", r.precal_filename);
    ASSERT_TRUE(r.hdb_filename.empty());
    ASSERT_EQ(0, r.calculation_point.x);
    ASSERT_EQ(0, r.calculation_point.y);
    ASSERT_EQ(0, r.calculation_point.z);
    ASSERT_TRUE(r.mirror);
    ASSERT_TRUE(r.use_encounter_period);
}

TEST_F(DiffractionForceModelTest, parser_precalr_ignores_calculation_point)
{
    const YamlDiffraction r = DiffractionForceModel::parse(test_data::diffraction_precalr() +
       "calculation point in body frame:\n"
       "    x: {value: 0.696, unit: m}\n"
       "    y: {value: 0, unit: m}\n"
       "    z: {value: 1.418, unit: m}\n");
    ASSERT_EQ(0, r.calculation_point.x);
    ASSERT_EQ(0, r.calculation_point.y);
    ASSERT_EQ(0, r.calculation_point.z);
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

TEST_F(DiffractionForceModelTest, precal_r_example)
{
    EnvironmentAndFrames env = get_waves_env(10.47198, 0.);
    BodyStates states;
    std::ofstream precalr_file("data.raodb.ini");
    precalr_file << test_data::precal();
    YamlDiffraction input;
    input.calculation_point = YamlCoordinates(0, 0, 0);
    input.precal_filename = "data.raodb.ini";
    input.mirror = true;
    input.use_encounter_period = true;
    const DiffractionForceModel force_model(input, BODY_NAME, env);
    double T = 10.47198;

    // Ship heading is always 0 (due North), encounter period varies with U and wave heading
    // Values from the force model are compared to values from the PRECAL_R data (amplitude and phase)

    // First battery of tests: wave direction=-180° (due South)
    env = get_waves_env(T, -M_PI);

    // U=0, wave direction=-180° -> encounter period = wave period = 10.47198s (frequency = 0.6 rad/s => third line), incidence = 180°
    states = get_states_with_forward_speed(0.);
    auto F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(),  0.447085E+03 * 1e3 * sin( -28.844746 * M_PI / 180.), std::abs(F.X() * 1e-4));
    ASSERT_NEAR(F.Y(), -0.112772E-02 * 1e3 * sin(-170.478134 * M_PI / 180.), std::abs(F.Y() * 1e-4));
    ASSERT_NEAR(F.Z(), -0.309840E+04 * 1e3 * sin(-116.540771 * M_PI / 180.), std::abs(F.Z() * 1e-4));
    ASSERT_NEAR(F.K(),  0.148446E-02 * 1e3 * sin(-102.782616 * M_PI / 180.), std::abs(F.K() * 1e-4));
    ASSERT_NEAR(F.M(), -0.145031E+06 * 1e3 * sin( -39.633919 * M_PI / 180.), std::abs(F.M() * 1e-4));
    ASSERT_NEAR(F.N(), -0.808052E-01 * 1e3 * sin( -21.082087 * M_PI / 180.), std::abs(F.N() * 1e-4));

    // U=-5.45, wave direction=-180°, wave period = 10.47198s -> encounter frequency = 0.4 rad/s => first line, incidence = 180°
    states = get_states_with_forward_speed(-5.45);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(),  0.614690E+02 * 1e3 * sin( -78.508087 * M_PI / 180.), std::abs(F.X() * 1e-4));
    ASSERT_NEAR(F.Y(), -0.121771E-02 * 1e3 * sin( -81.328613 * M_PI / 180.), std::abs(F.Y() * 1e-4));
    ASSERT_NEAR(F.Z(), -0.444548E+04 * 1e3 * sin(-131.685837 * M_PI / 180.), std::abs(F.Z() * 1e-4));
    ASSERT_NEAR(F.K(),  0.927719E-04 * 1e3 * sin(-127.574928 * M_PI / 180.), std::abs(F.K() * 1e-4));
    ASSERT_NEAR(F.M(), -0.481271E+05 * 1e3 * sin( -97.924873 * M_PI / 180.), std::abs(F.M() * 1e-4));
    ASSERT_NEAR(F.N(), -0.151559E-01 * 1e3 * sin( -68.464912 * M_PI / 180.), std::abs(F.N() * 1e-4));

    // U=10.9, wave direction=-180°, wave period = 10.47198s -> encounter frequency = 1 rad/s => seventh line, incidence = 180°
    states = get_states_with_forward_speed(10.9);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(),  0.188522E+03 * 1e3 * sin( 155.721146 * M_PI / 180.), std::abs(F.X() * 1e-4));
    ASSERT_NEAR(F.Y(), -0.390895E-03 * 1e3 * sin( -84.802406 * M_PI / 180.), std::abs(F.Y() * 1e-4));
    ASSERT_NEAR(F.Z(), -0.832559E+03 * 1e3 * sin( 112.416695 * M_PI / 180.), std::abs(F.Z() * 1e-4));
    ASSERT_NEAR(F.K(),  0.112438E-02 * 1e3 * sin(-149.386047 * M_PI / 180.), std::abs(F.K() * 1e-4));
    ASSERT_NEAR(F.M(), -0.553710E+05 * 1e3 * sin( 179.790375 * M_PI / 180.), std::abs(F.M() * 5e-3));
    ASSERT_NEAR(F.N(), -0.967943E-02 * 1e3 * sin( 163.360352 * M_PI / 180.), std::abs(F.N() * 1e-4));

    // Second battery of tests: wave direction=-90° (North-West)
    env = get_waves_env(T, -90.*M_PI/180);

    // U=0, wave direction=-90° -> encounter period = wave period = 10.47198s (frequency = 0.6 rad/s => third line), incidence = 90°
    states = get_states_with_forward_speed(0.);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(),  0.213802E+03 * 1e3 * sin( 146.941406 * M_PI / 180.), std::abs(F.X() * 1e-4));
    ASSERT_NEAR(F.Y(), -0.253451E+04 * 1e3 * sin( -75.123482 * M_PI / 180.), std::abs(F.Y() * 1e-4));
    ASSERT_NEAR(F.Z(), -0.841796E+04 * 1e3 * sin(-131.750656 * M_PI / 180.), std::abs(F.Z() * 1e-4));
    ASSERT_NEAR(F.K(),  0.126249E+04 * 1e3 * sin(  46.827698 * M_PI / 180.), std::abs(F.K() * 1e-4));
    ASSERT_NEAR(F.M(), -0.668399E+05 * 1e3 * sin( 172.587128 * M_PI / 180.), std::abs(F.M() * 1e-4));
    ASSERT_NEAR(F.N(), -0.496977E+05 * 1e3 * sin( -47.521080 * M_PI / 180.), std::abs(F.N() * 1e-4));

    // U=10, wave direction=-90° -> encounter period = wave period = 15.70796s (frequency = 0.4 rad/s => first line), incidence = 90°
    env = get_waves_env(15.70796, -90.*M_PI/180);
    states = get_states_with_forward_speed(10);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(),  0.127820E+03 * 1e3 * sin( 116.837296 * M_PI / 180.), std::abs(F.X() * 1e-4));
    ASSERT_NEAR(F.Y(), -0.117869E+04 * 1e3 * sin( -82.125160 * M_PI / 180.), std::abs(F.Y() * 1e-4));
    ASSERT_NEAR(F.Z(), -0.514178E+04 * 1e3 * sin(-147.132416 * M_PI / 180.), std::abs(F.Z() * 1e-4));
    ASSERT_NEAR(F.K(),  0.111813E+04 * 1e3 * sin(  30.492355 * M_PI / 180.), std::abs(F.K() * 1e-4));
    ASSERT_NEAR(F.M(), -0.381031E+05 * 1e3 * sin( 148.914230 * M_PI / 180.), std::abs(F.M() * 1e-4));
    ASSERT_NEAR(F.N(), -0.271359E+05 * 1e3 * sin( -37.329960 * M_PI / 180.), std::abs(F.N() * 1e-4));

    // U=-10, wave direction=-90° -> encounter period = wave period = 6.28319s (frequency = 1 rad/s => seventh line), incidence = 90°
    env = get_waves_env(6.28319, -90.*M_PI/180);
    states = get_states_with_forward_speed(-12.586);
    F = force_model.get_force(states, 0., env, {});
    ASSERT_NEAR(F.X(),  0.231034E+03 * 1e3 * sin( 162.160065 * M_PI / 180.), std::abs(F.X() * 1e-4));
    ASSERT_NEAR(F.Y(), -0.275595E+04 * 1e3 * sin( -50.503010 * M_PI / 180.), std::abs(F.Y() * 1e-4));
    ASSERT_NEAR(F.Z(), -0.117092E+05 * 1e3 * sin(-128.845367 * M_PI / 180.), std::abs(F.Z() * 1e-4));
    ASSERT_NEAR(F.K(),  0.571309E+04 * 1e3 * sin( -57.190521 * M_PI / 180.), std::abs(F.K() * 1e-4));
    ASSERT_NEAR(F.M(), -0.888754E+05 * 1e3 * sin(-163.850571 * M_PI / 180.), std::abs(F.M() * 1e-4));
    ASSERT_NEAR(F.N(), -0.117203E+06 * 1e3 * sin( -47.279522 * M_PI / 180.), std::abs(F.N() * 1e-4));
}
