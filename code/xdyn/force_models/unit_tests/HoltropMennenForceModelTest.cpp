#include "HoltropMennenForceModelTest.hpp"
#include "HoltropMennenForceModel.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include "xdyn/core/BodyStates.hpp"

#include <string>
#include <sstream>
#include <vector>

HoltropMennenForceModelTest::HoltropMennenForceModelTest() : a(ssc::random_data_generator::DataGenerator(45454)), env()
{
    env.rho = 1025;
    env.nu = 1.18e-6;
    env.g = 9.81;
}

HoltropMennenForceModelTest::~HoltropMennenForceModelTest()
{
}

void HoltropMennenForceModelTest::SetUp()
{
}

void HoltropMennenForceModelTest::TearDown()
{
}

std::string get_yaml_input_with_optional();
std::string get_yaml_input_with_optional()
{
    std::stringstream ss;
    ss  << "Lwl: {value: 325.5,unit: m}\n" << "Lpp: {value: 320,unit: m}\n" << "B: {value: 58,unit: m}\n" << "Ta: {value: 20.8,unit: m}\n"
        << "Tf: {value: 20.8,unit: m}\n" << "Vol: {value: 312622,unit: m^3}\n" << "S: {value: 27194, unit: m^2}\n" << "lcb: 3.48\n"
        << "Abt: {value: 25,unit: m^2}\n" << "hb: {value: 2.5,unit: m}\n" << "Cm: 0.998\n" << "Cwp: 0.83\n" << "At: {value: 0,unit: m^2}\n"
        << "Sapp: {value: 273.3,unit: m^2}\n" << "Cstern: 0\n" << "1+k2: 2\n" << "apply on ship speed direction: true\n"
        << "iE: {value: 25, unit: deg}\n" << "1+k1: 1.5\n";
    return ss.str();
}

std::string get_yaml_input_without_optional();
std::string get_yaml_input_without_optional()
{
    std::stringstream ss;
    ss  << "Lwl: {value: 325.5,unit: m}\n" << "Lpp: {value: 320,unit: m}\n" << "B: {value: 58,unit: m}\n" << "Ta: {value: 20.8,unit: m}\n"
        << "Tf: {value: 20.8,unit: m}\n" << "Vol: {value: 312622,unit: m^3}\n" << "lcb: 3.48\n"
        << "Abt: {value: 25,unit: m^2}\n" << "hb: {value: 2.5,unit: m}\n" << "Cm: 0.998\n" << "Cwp: 0.83\n" << "At: {value: 0,unit: m^2}\n"
        << "Sapp: {value: 273.3,unit: m^2}\n" << "Cstern: 0\n" << "1+k2: 2\n";
    return ss.str();
}

HoltropMennenForceModel::Input get_Holtrop_Mennen_1982_input();
HoltropMennenForceModel::Input get_Holtrop_Mennen_1982_input()
{
    HoltropMennenForceModel::Input input;
    input.Lwl = 205;
    input.Lpp = 200;
    input.B = 32;
    input.Ta = 10;
    input.Tf = 10;
    input.Vol = 37500;
    input.lcb = -0.75;
    input.Abt = 20;
    input.hb = 4;
    input.Cm = 0.98;
    input.Cwp = 0.75;
    input.At = 16;
    input.Sapp = 50;
    input.Cstern = 10;
    input.app_form_coeff = 1.5;
    return input;
}

HoltropMennenForceModel::Input get_Holtrop_1984_input();
HoltropMennenForceModel::Input get_Holtrop_1984_input()
{
    HoltropMennenForceModel::Input input;
    input.Lwl = 50;
    input.Lpp = 50;
    input.B = 12;
    input.Ta = 3.3;
    input.Tf = 3.1;
    input.Vol = 900;
    input.lcb = -4.5;
    input.S = 584.9;
    input.Abt = 0;
    input.hb = 2.5;
    input.Cm = 0.78;
    input.Cwp = 0.8;
    input.At = 10;
    input.Sapp = 50;
    input.Cstern = 0;
    input.app_form_coeff = 3;
    input.iE = 25;
    return input;
}

BodyStates get_steady_forward_speed_states(double u = 0., const std::string& body_name = "body");
BodyStates get_steady_forward_speed_states(double u, const std::string& body_name)
{
    BodyStates states;
    states.x.record(0, 0);
    states.y.record(0, 0);
    states.z.record(0, 0);
    states.u.record(0, u);
    states.v.record(0, 0);
    states.w.record(0, 0);
    states.p.record(0, 0);
    states.q.record(0, 0);
    states.r.record(0, 0);
    YamlRotation rotations;
    rotations.order_by = "angle";
    rotations.convention.push_back("z");
    rotations.convention.push_back("y'");
    rotations.convention.push_back("x''");
    const auto angle = states.convert(ssc::kinematics::EulerAngles(0, 0, 0), rotations);
    states.qr.record(0, std::get<0>(angle));
    states.qi.record(0, std::get<1>(angle));
    states.qj.record(0, std::get<2>(angle));
    states.qk.record(0, std::get<3>(angle));
    states.G = ssc::kinematics::Point(body_name);
    states.hydrodynamic_forces_calculation_point = ssc::kinematics::Point(body_name);
    return states;
}

TEST_F(HoltropMennenForceModelTest, can_parse)
{
    const HoltropMennenForceModel::Input input = HoltropMennenForceModel::parse(get_yaml_input_with_optional());
    ASSERT_DOUBLE_EQ(input.Lwl, 325.5);
    ASSERT_DOUBLE_EQ(input.Lpp, 320);
    ASSERT_DOUBLE_EQ(input.B, 58);
    ASSERT_DOUBLE_EQ(input.Ta, 20.8);
    ASSERT_DOUBLE_EQ(input.Tf, 20.8);
    ASSERT_DOUBLE_EQ(input.Vol, 312622);
    ASSERT_DOUBLE_EQ(input.lcb, 3.48);
    ASSERT_DOUBLE_EQ(input.Abt, 25);
    ASSERT_DOUBLE_EQ(input.hb, 2.5);
    ASSERT_DOUBLE_EQ(input.Cm, 0.998);
    ASSERT_DOUBLE_EQ(input.Cwp, 0.83);
    ASSERT_DOUBLE_EQ(input.At, 0);
    ASSERT_DOUBLE_EQ(input.Sapp, 273.3);
    ASSERT_DOUBLE_EQ(input.Cstern, 0);
    ASSERT_DOUBLE_EQ(input.app_form_coeff, 2);
    ASSERT_TRUE(input.apply_on_ship_speed_direction);
    ASSERT_TRUE(input.S.is_initialized());
    ASSERT_DOUBLE_EQ(input.S.get(), 27194);
    ASSERT_TRUE(input.iE.is_initialized());
    ASSERT_DOUBLE_EQ(input.iE.get(), 25);
    ASSERT_TRUE(input.hull_form_coeff.is_initialized());
    ASSERT_DOUBLE_EQ(input.hull_form_coeff.get(), 1.5);
}

TEST_F(HoltropMennenForceModelTest, can_ignore_optional_inputs)
{
    const HoltropMennenForceModel::Input input = HoltropMennenForceModel::parse(get_yaml_input_without_optional());
    ASSERT_FALSE(input.apply_on_ship_speed_direction);
    ASSERT_FALSE(input.S.is_initialized());
    ASSERT_FALSE(input.iE.is_initialized());
    ASSERT_FALSE(input.hull_form_coeff.is_initialized());
}

TEST_F(HoltropMennenForceModelTest, no_resistance_at_zero_speed)
{
    const auto input = get_Holtrop_Mennen_1982_input();
    auto force_model = HoltropMennenForceModel(input, "body", env);
    auto states = get_steady_forward_speed_states(0, "body");
    ASSERT_DOUBLE_EQ(0, force_model(states, 0, env).X());
}

TEST_F(HoltropMennenForceModelTest, numerical_example_1982)
{
    // Example from Holtrop & Mennen's original 1982 paper (tolerance is based on the precision given in the paper)
    /* Note:
     * Holtrop & Mennen's original 1982 method was revised by Holtrop in 1984. A number of formulae changed, in particular for the friction and wave components of the force.
     * In consequence, some of the checks in this test have rather large margins because the values in the 1982 paper used the early version of the model, but the 1984 version is used in xdyn.
     * All the values that have large margins in this test are properly tested with the 1984 test.
     */
    const auto input = get_Holtrop_Mennen_1982_input();
    const HoltropMennenForceModel::DerivedData derived_data(input);
    // Checking derived input values
    EXPECT_NEAR(0.1561,derived_data.c7,0.0001);
    EXPECT_NEAR(0.04,derived_data.c4,0.01);
    EXPECT_NEAR(10,derived_data.T,0.0001);
    EXPECT_NEAR(0.57164634146341464,derived_data.Cb,0.0001);
    EXPECT_NEAR(0.58331259333001484,derived_data.Cp,0.0001);
    EXPECT_NEAR(7381.45,derived_data.S,0.01);
    EXPECT_NEAR(-1.9412639420954685,derived_data.m3,0.0001);
    EXPECT_NEAR(12.08,derived_data.iE,0.01);
    EXPECT_NEAR(-1.69385,derived_data.c15,0.00001); // Note: there is a sign error for the value of c15 in the original 1982 paper
    EXPECT_NEAR(0.6261,derived_data.Pb,0.0001);
    EXPECT_NEAR(0.02119,derived_data.c3,0.00001);
    EXPECT_NEAR(0.7595,derived_data.c2,0.0001);
    EXPECT_NEAR(0.000352,derived_data.Ca,0.000001);
    EXPECT_NEAR(1.0289534685942132,derived_data.c17,0.0001);
    EXPECT_NEAR(1.380877247519813,derived_data.c16,0.0001);
    EXPECT_NEAR(-2.1274,derived_data.m1,0.0001);
    EXPECT_NEAR(81.385,derived_data.Lr,0.001);
    EXPECT_NEAR(0.6513,derived_data.lambda,0.0001);
    EXPECT_NEAR(1.398,derived_data.c1,0.001);
    EXPECT_NEAR(0.9592,derived_data.c5,0.0001);
    EXPECT_NEAR(1.11,derived_data.c14,0.0001);

    auto force_model = HoltropMennenForceModel(input, "body", env);
    BodyStates states = get_steady_forward_speed_states(25. * 1852./3600., "body");
    // Constant intermediate values
    EXPECT_NEAR(1.156,derived_data.hull_form_coeff,0.05); // Large margin because the formulae were revised in 1984
    // Resulting forces
    EXPECT_NEAR(869630,force_model.Rf(states, env),200000); // Large margin because the formulae were revised in 1984
    EXPECT_NEAR(8830,force_model.Rapp(states, env),10);
    EXPECT_NEAR(557110,force_model.Rw(states, env),5000); // Large margin because the formulae were revised in 1984
    EXPECT_NEAR(49,force_model.Rb(states, env),1);
    EXPECT_NEAR(0.,force_model.Rtr(states, env),1);
    EXPECT_NEAR(220572,force_model.Ra(states, env),1); // Note: there seems to be a rounding error in the original paper (Ra = 221980 N)
    EXPECT_NEAR(1793260,-force_model(states, 0, env).X(),200000); // large margin to account for all the margins
}

TEST_F(HoltropMennenForceModelTest, numerical_example_1984)
{
    // Example from Holtrop's 1984 paper (revision of the Holtrop-Mennen method)
    const auto input = get_Holtrop_1984_input();
    const HoltropMennenForceModel::DerivedData derived_data(input);
    // Checking derived input values
    EXPECT_NEAR(0.60096,derived_data.Cp,0.00001);
    EXPECT_NEAR(0.46875,derived_data.Cb,0.00001);
    EXPECT_NEAR(14.1728,derived_data.Lr,0.0001);
    EXPECT_NEAR(0.00064,derived_data.Ca,0.00001);
    EXPECT_NEAR(1.4133,derived_data.c17,0.0001);
    EXPECT_NEAR(0.7329,derived_data.c5,0.0001);
    EXPECT_NEAR(-2.0298,derived_data.m3,0.0001);
    EXPECT_NEAR(0.7440,derived_data.lambda,0.0001);
    EXPECT_NEAR(1.0,derived_data.c2,0.1);
    EXPECT_NEAR(-1.69385,derived_data.c15,0.00001);
    auto force_model = HoltropMennenForceModel(input, "body", env);
    BodyStates states;
    // Constant intermediate values
    EXPECT_NEAR(1.297,derived_data.hull_form_coeff,0.001);
    EXPECT_NEAR(584.9,derived_data.S,0.1);
    // Numerical values given in Holtrop (1982)
    const std::vector<double> speed_kts({25,27,29,31,33,35});
    const std::vector<double> Rw({475000,512000,539000,564000,590000,618000});
    const std::vector<double> Rapp({21000,24000,28000,31000,35000,39000});
    const std::vector<double> Rtr({25000,16000,2000,0,0,0});
    const std::vector<double> R({662000,715000,756000,807000,864000,925000});
    for(size_t i = 0 ; i<speed_kts.size() ; i++)
    {
        states = get_steady_forward_speed_states(speed_kts.at(i) * 1852/3600, "body");
        EXPECT_NEAR(force_model.Rw(states, env),Rw.at(i),1000);
        EXPECT_NEAR(force_model.Rapp(states, env),Rapp.at(i),1000);
        EXPECT_NEAR(force_model.Rtr(states, env),Rtr.at(i),1000);
        EXPECT_NEAR(-force_model(states, 0, env).X(),R.at(i),R.at(i)/100);
    }
}
