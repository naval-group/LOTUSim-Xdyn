/*
 * RadiationDampingForceModelTest.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#include <fstream>
#include <ssc/integrate.hpp>

#include "BodyStates.hpp"
#include "hdb_data.hpp"
#include "hdb_test.hpp"
#include "RadiationDampingBuilder.hpp"
#include "RadiationDampingForceModel.hpp"
#include "RadiationDampingForceModelTest.hpp"
#include "EnvironmentAndFrames.hpp"
#include "yaml_data.hpp"
#include "precal_test_data.hpp"

#define EPS 5E-2

RadiationDampingForceModelTest::RadiationDampingForceModelTest() : a(ssc::random_data_generator::DataGenerator(2121))
{
}

RadiationDampingForceModelTest::~RadiationDampingForceModelTest()
{
}

void RadiationDampingForceModelTest::SetUp()
{
}

void RadiationDampingForceModelTest::TearDown()
{
}

double compute_Ma_from_K(const double omega, const double A, std::function<double(double)> K, const double tau_min, const double tau_max);
double compute_Ma_from_K(const double omega, const double A, std::function<double(double)> K, const double tau_min, const double tau_max)
{
    const std::function<double(double)> g = [omega, K](double tau){return K(tau)*sin(omega*tau);};
    return A - (1/omega)*ssc::integrate::Simpson(g).integrate_f(tau_min, tau_max, 1e-2);
}

TR1(shared_ptr)<HDBParser> RadiationDampingForceModelTest::get_hdb_data(const YamlRadiationDamping& yaml, const bool only_diagonal_terms) const
{
    std::vector<double> Br;
    std::vector<double> Ma;
    const double omega_min = 0.01;
    const double omega_max = 40;
    const size_t N = 460;
    const double A = 1000;
    const auto omegas = RadiationDampingBuilder(TypeOfQuadrature::FILON,TypeOfQuadrature::GAUSS_KRONROD).build_exponential_intervals(omega_min, omega_max, N);
    for (auto omega:omegas) Br.push_back(test_data::analytical_Br(omega));
    for (auto omega:omegas) Ma.push_back(compute_Ma_from_K(omega, A, test_data::analytical_K, yaml.tau_min, yaml.tau_max));
    return TR1(shared_ptr)<HDBParser>(new HDBParserForTests(omegas, Ma, Br, only_diagonal_terms));
}

YamlRadiationDamping RadiationDampingForceModelTest::get_yaml_data(const bool show_debug) const
{
    YamlRadiationDamping ret;
    ret.nb_of_points_for_retardation_function_discretization = 50;
    ret.omega_min = 0;
    ret.omega_max = 30;
    ret.output_Br_and_K = show_debug;
    ret.tau_min = 0.2094395;
    ret.tau_max = 10;
    ret.type_of_quadrature_for_convolution = TypeOfQuadrature::SIMPSON;
    ret.type_of_quadrature_for_cos_transform = TypeOfQuadrature::SIMPSON;
    return ret;
}

TEST_F(RadiationDampingForceModelTest, parser)
{
    const YamlRadiationDamping r = RadiationDampingForceModel::parse(test_data::radiation_damping(),false).yaml;
    ASSERT_EQ("test_ship.hdb", r.hdb_filename);
    ASSERT_EQ(50, r.nb_of_points_for_retardation_function_discretization);
    ASSERT_DOUBLE_EQ(30,r.omega_max);
    ASSERT_EQ(0,r.omega_min);
    ASSERT_TRUE(r.output_Br_and_K);
    ASSERT_DOUBLE_EQ(10,r.tau_max);
    ASSERT_DOUBLE_EQ(0.2094395,r.tau_min);
    ASSERT_EQ(TypeOfQuadrature::CLENSHAW_CURTIS, r.type_of_quadrature_for_convolution);
    ASSERT_EQ(TypeOfQuadrature::SIMPSON, r.type_of_quadrature_for_cos_transform);
    ASSERT_DOUBLE_EQ(0.696, r.calculation_point_in_body_frame.x);
    ASSERT_DOUBLE_EQ(0, r.calculation_point_in_body_frame.y);
    ASSERT_DOUBLE_EQ(1.418, r.calculation_point_in_body_frame.z);
    ASSERT_TRUE(r.forward_speed_correction);
}

void record(BodyStates& states, const double t, const double value);
void record(BodyStates& states, const double t, const double value)
{
    states.u.record(t, value);
    states.v.record(t, value);
    states.w.record(t, value);
    states.p.record(t, value);
    states.q.record(t, value);
    states.r.record(t, value);
}

void record(BodyStates& states, const double t, const double u, const double v, const double w, const double p, const double q, const double r);
void record(BodyStates& states, const double t, const double u, const double v, const double w, const double p, const double q, const double r)
{
    states.u.record(t, u);
    states.v.record(t, v);
    states.w.record(t, w);
    states.p.record(t, p);
    states.q.record(t, q);
    states.r.record(t, r);
}

double record_sine(BodyStates& states, const double tstart, const double T, const unsigned int Nperiods, const unsigned int Nsteps);
double record_sine(BodyStates& states, const double tstart, const double T, const unsigned int Nperiods, const unsigned int Nsteps)
{
    const double tend = tstart + T*Nperiods;
    const double dt = (tend-tstart)/Nsteps;
    for (unsigned int i = 0 ; i <= Nsteps ; i++)
    {
        double t = tstart + i*dt;
        record(states, t, sin(2*PI*(t-tstart)/T));
    }
    return tend;
}


TEST_F(RadiationDampingForceModelTest, example)
{
//! [RadiationDampingForceModelTest example]
    const auto yaml = get_yaml_data(false);
    RadiationDampingForceModel::Input input;
    input.parser = get_hdb_data(yaml);
    input.yaml = yaml;
    const EnvironmentAndFrames env;
    const std::string body_name = a.random<std::string>();
    RadiationDampingForceModel F(input, body_name, env);
    ASSERT_EQ("radiation damping", F.model_name());
    BodyStates states(100);
    states.name = body_name;
//! [RadiationDampingForceModelTest example]
//! [RadiationDampingForceModelTest expected output]
    const auto T = 10;
    const auto t = record_sine(states, 0, T, 10, 100);

    const auto K = test_data::analytical_K;
    std::function<double(double)> g = [K, T](double t){return K(t)*sin(-2*PI*t/T);};
    const double Fexpected = -ssc::integrate::ClenshawCurtisCosine(g,0).integrate_f(yaml.tau_min,yaml.tau_max);

    const auto Frad = F.get_force(states, 100, env, {});
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Y());
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Z());
    ASSERT_DOUBLE_EQ(Frad.K(),Frad.M());
    ASSERT_DOUBLE_EQ(Frad.M(),Frad.N());
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.X(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.Y(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.Z(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.K(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.M(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.N(), EPS);
//! [RadiationDampingForceModelTest expected output]
}

TEST_F(RadiationDampingForceModelTest, results_are_zero_for_constant_velocity)
{
    const auto yaml = get_yaml_data(false);
    RadiationDampingForceModel::Input input;
    input.parser = get_hdb_data(yaml);
    input.yaml = yaml;
    input.yaml.remove_constant_speed = true;
    const EnvironmentAndFrames env;
    const std::string body_name = a.random<std::string>();
    RadiationDampingForceModel F(input, body_name, env);
    ASSERT_EQ("radiation damping", F.model_name());
    BodyStates states(100);
    states.name = body_name;
    const double velocity = a.random<double>();
    record(states, 0, velocity);
    record(states, 100, velocity);

    const auto Frad = F.get_force(states, 100, env, {});
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Y());
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Z());
    ASSERT_DOUBLE_EQ(Frad.K(),Frad.M());
    ASSERT_DOUBLE_EQ(Frad.M(),Frad.N());
    ASSERT_SMALL_RELATIVE_ERROR(0, Frad.X(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0, Frad.Y(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0, Frad.Z(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0, Frad.K(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0, Frad.M(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0, Frad.N(), EPS);
}

double record_offset_sine(BodyStates& states, const double tstart, const double T, const unsigned int Nperiods, const unsigned int Nsteps, const double offset);
double record_offset_sine(BodyStates& states, const double tstart, const double T, const unsigned int Nperiods, const unsigned int Nsteps, const double offset)
{
    const double tend = tstart + T*Nperiods;
    const double dt = (tend-tstart)/Nsteps;
    for (unsigned int i = 0 ; i <= Nsteps ; i++)
    {
        double t = tstart + i*dt;
        record(states, t, offset + sin(2*PI*(t-tstart)/T));
    }
    return tend;
}

TEST_F(RadiationDampingForceModelTest, velocity_offset_should_not_change_the_result)
{
    const auto yaml = get_yaml_data(false);
    RadiationDampingForceModel::Input input;
    input.parser = get_hdb_data(yaml);
    input.yaml = yaml;
    input.yaml.remove_constant_speed = true;
    const EnvironmentAndFrames env;
    const std::string body_name = a.random<std::string>();
    RadiationDampingForceModel F(input, body_name, env);
    ASSERT_EQ("radiation damping", F.model_name());
    BodyStates states1(100);
    states1.name = body_name;
    const double T_sine = 10;
    record_sine(states1, 0, T_sine, 10, 100);

    BodyStates states2(100);
    states2.name = body_name;
    const double offset = a.random<double>();
    record_offset_sine(states2, 0, T_sine, 10, 100, offset);

    const auto Frad1 = F.get_force(states1, 100, env, {});
    const auto Frad2 = F.get_force(states2, 100, env, {});
    ASSERT_SMALL_RELATIVE_ERROR(Frad1.X(), Frad2.X(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Frad1.Y(), Frad2.Y(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Frad1.Z(), Frad2.Z(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Frad1.K(), Frad2.K(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Frad1.M(), Frad2.M(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Frad1.N(), Frad2.N(), EPS);
}

TEST_F(RadiationDampingForceModelTest, should_print_debugging_information_if_required_by_yaml_data)
{
    std::stringstream debug;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(debug.rdbuf());
    ASSERT_TRUE(debug.str().empty());
    // Call the radiation damping model
    RadiationDampingForceModel::Input input;
    input.yaml = get_yaml_data(true);
    input.parser = get_hdb_data(input.yaml);
    RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    ASSERT_FALSE(debug.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(RadiationDampingForceModelTest, should_not_print_debugging_information_if_not_required_by_yaml_data)
{
    std::stringstream debug;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(debug.rdbuf());
    // Call the radiation damping model
    RadiationDampingForceModel::Input input;
    input.yaml = get_yaml_data(false);
    input.parser = get_hdb_data(input.yaml);
    RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    ASSERT_TRUE(debug.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(RadiationDampingForceModelTest, force_model_knows_history_length)
{
    RadiationDampingForceModel::Input input;
    input.yaml = get_yaml_data(false);
    input.parser = get_hdb_data(input.yaml);
    const RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    ASSERT_DOUBLE_EQ(input.yaml.tau_max, F.get_Tmax());
}


TEST_F(RadiationDampingForceModelTest, matrix_product_should_be_done_properly)
{
    RadiationDampingForceModel::Input input;
    input.yaml = get_yaml_data(false);
    const bool only_diagonal_terms = true;
    input.parser = get_hdb_data(input.yaml, not(only_diagonal_terms));
    input.yaml.type_of_quadrature_for_convolution = TypeOfQuadrature::RECTANGLE;
    EnvironmentAndFrames env;
    RadiationDampingForceModel F(input, "", env);
    BodyStates states(100);
    const double tmin = 0.20943950000000000067;
    const double tmax = 10;
    const double eps = 1E-6;
    const double t0 = 5.1047197500000001114;

    record(states, 0, 0);
    record(states, tmax-t0-eps, 0);

    const double u0 = 1;
    const double v0 = 2;
    const double w0 = 3;
    const double p0 = 4;
    const double q0 = 5;
    const double r0 = 6;

    record(states, tmax-t0-eps/2, u0, v0, w0, p0, q0, r0);
    record(states, tmax-t0+eps/2, u0, v0, w0, p0, q0, r0);
    record(states, tmax-t0+eps, 0);
    record(states, tmax, 0);

    const auto Frad = F.get_force(states, 0, env, {});
    // This is the result of the discrete Fourier transform: it does not
    // exactly match the value given by the (analytical) continuous Fourier
    // transform.
    const double k = -0.50135576185179109299;
    ASSERT_NEAR(test_data::analytical_K(t0), k, 3E-3);
    const double conv = -(tmax-tmin )/100*k;

    ASSERT_NEAR(conv * (11*u0 + 12*v0 + 13*w0 + 14*p0 + 15*q0 + 16*r0), Frad.X(), EPS);
    ASSERT_NEAR(conv * (21*u0 + 22*v0 + 23*w0 + 24*p0 + 25*q0 + 26*r0), Frad.Y(), EPS);
    ASSERT_NEAR(conv * (31*u0 + 32*v0 + 33*w0 + 34*p0 + 35*q0 + 36*r0), Frad.Z(), EPS);
    ASSERT_NEAR(conv * (41*u0 + 42*v0 + 43*w0 + 44*p0 + 45*q0 + 46*r0), Frad.K(), 10*EPS);
    ASSERT_NEAR(conv * (51*u0 + 52*v0 + 53*w0 + 54*p0 + 55*q0 + 56*r0), Frad.M(), 10*EPS);
    ASSERT_NEAR(conv * (61*u0 + 62*v0 + 63*w0 + 64*p0 + 65*q0 + 66*r0), Frad.N(), 10*EPS);
}

TEST_F(RadiationDampingForceModelTest, cannot_specify_both_hdb_and_precal_r_files)
{
    const std::string invalid_yaml = "model: radiation damping\n"
                                     "hdb: test_ship.hdb\n"
                                     "raodb: test_ship.ini\n"
                                     "type of quadrature for cos transform: simpson\n"
                                     "type of quadrature for convolution: clenshaw-curtis\n"
                                     "nb of points for retardation function discretization: 50\n"
                                     "omega min: {value: 0, unit: rad/s}\n"
                                     "omega max: {value: 30, unit: rad/s}\n"
                                     "tau min: {value: 0.2094395, unit: s}\n"
                                     "tau max: {value: 10, unit: s}\n"
                                     "output Br and K: true\n"
                                     "forward speed correction: true\n"
                                     "calculation point in body frame:\n"
                                     "    x: {value: 0.696, unit: m}\n"
                                     "    y: {value: 0, unit: m}\n"
                                     "    z: {value: 1.418, unit: m}\n";
    ASSERT_THROW(RadiationDampingForceModel::parse(invalid_yaml,false), InvalidInputException);
}

TEST_F(RadiationDampingForceModelTest, should_be_able_to_use_precal_r_files_only)
{
    const std::string valid_yaml = "model: radiation damping\n"
                                   "raodb: test_ship.ini\n"
                                   "type of quadrature for cos transform: simpson\n"
                                   "type of quadrature for convolution: clenshaw-curtis\n"
                                   "nb of points for retardation function discretization: 50\n"
                                   "omega min: {value: 0, unit: rad/s}\n"
                                   "omega max: {value: 30, unit: rad/s}\n"
                                   "tau min: {value: 0.2094395, unit: s}\n"
                                   "tau max: {value: 10, unit: s}\n"
                                   "output Br and K: true\n"
                                   "forward speed correction: true\n"
                                   "calculation point in body frame:\n"
                                   "    x: {value: 0.696, unit: m}\n"
                                   "    y: {value: 0, unit: m}\n"
                                   "    z: {value: 1.418, unit: m}\n";
    ASSERT_NO_THROW(RadiationDampingForceModel::parse(valid_yaml,false));
}

TEST_F(RadiationDampingForceModelTest, need_one_of_precal_r_or_hdb)
{
    const std::string invalid_yaml = "model: radiation damping\n"
                                     "type of quadrature for cos transform: simpson\n"
                                     "type of quadrature for convolution: clenshaw-curtis\n"
                                     "nb of points for retardation function discretization: 50\n"
                                     "omega min: {value: 0, unit: rad/s}\n"
                                     "omega max: {value: 30, unit: rad/s}\n"
                                     "tau min: {value: 0.2094395, unit: s}\n"
                                     "tau max: {value: 10, unit: s}\n"
                                     "output Br and K: true\n"
                                     "forward speed correction: true\n"
                                     "calculation point in body frame:\n"
                                     "    x: {value: 0.696, unit: m}\n"
                                     "    y: {value: 0, unit: m}\n"
                                     "    z: {value: 1.418, unit: m}\n";
    ASSERT_THROW(RadiationDampingForceModel::parse(invalid_yaml,false), InvalidInputException);
}

TEST_F(RadiationDampingForceModelTest, precal_r_filename_is_read_properly)
{
    const std::string valid_yaml = "model: radiation damping\n"
                                   "raodb: test_ship.ini\n"
                                   "type of quadrature for cos transform: simpson\n"
                                   "type of quadrature for convolution: clenshaw-curtis\n"
                                   "nb of points for retardation function discretization: 50\n"
                                   "omega min: {value: 0, unit: rad/s}\n"
                                   "omega max: {value: 30, unit: rad/s}\n"
                                   "tau min: {value: 0.2094395, unit: s}\n"
                                   "tau max: {value: 10, unit: s}\n"
                                   "output Br and K: true\n"
                                   "forward speed correction: true\n"
                                   "calculation point in body frame:\n"
                                   "    x: {value: 0.696, unit: m}\n"
                                   "    y: {value: 0, unit: m}\n"
                                   "    z: {value: 1.418, unit: m}\n";
    ASSERT_EQ("test_ship.ini", RadiationDampingForceModel::parse(valid_yaml,false).yaml.precal_r_filename);
}

TEST_F(RadiationDampingForceModelTest, can_use_data_from_precal_r)
{
    const std::string yaml = "model: radiation damping\n"
                             "raodb: data.raodb.ini\n"
                             "type of quadrature for cos transform: simpson\n"
                             "type of quadrature for convolution: trapezoidal\n"
                             "nb of points for retardation function discretization: 50\n"
                             "omega min: {value: 0, unit: rad/s}\n"
                             "omega max: {value: 30, unit: rad/s}\n"
                             "tau min: {value: 0.2094395, unit: s}\n"
                             "tau max: {value: 10, unit: s}\n"
                             "output Br and K: false\n"
                             "forward speed correction: false\n"
                             "calculation point in body frame:\n"
                             "    x: {value: 0.696, unit: m}\n"
                             "    y: {value: 0, unit: m}\n"
                             "    z: {value: 1.418, unit: m}\n";
    std::ofstream precalr_file("data.raodb.ini");
    precalr_file << test_data::precal();
    RadiationDampingForceModel::Input input = RadiationDampingForceModel::parse(yaml);
    const EnvironmentAndFrames env;
    const std::string body_name = a.random<std::string>();
    RadiationDampingForceModel F(input, body_name, env);
    BodyStates states(100);
    states.name = body_name;
    const auto T = 10;
    const auto t = record_sine(states, 0, T, 10, 100);
    const auto Frad = F.get_force(states, 0, env, {});
    ASSERT_NE(0, Frad.X());
}