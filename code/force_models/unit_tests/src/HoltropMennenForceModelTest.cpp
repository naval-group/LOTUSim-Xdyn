/*
 * HoltropMennenForceModelTest.cpp
 *
 *  Created on: 5 janv. 2021
 *      Author: mcharlou2016
 */

#include <string>
#include <sstream>
#include <vector>

#include "HoltropMennenForceModel.hpp"
#include "EnvironmentAndFrames.hpp"
#include "BodyStates.hpp"

#include "HoltropMennenForceModelTest.hpp"

HoltropMennenForceModelTest::HoltropMennenForceModelTest() : a(ssc::random_data_generator::DataGenerator(45454))
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

std::string get_yaml_input()
{
    std::stringstream ss;
    ss  << "Lwl: {value: 325.5,unit: m}\n" << "Lpp: {value: 320,unit: m}\n" << "B: {value: 58,unit: m}\n" << "Ta: {value: 20.8,unit: m}\n"
        << "Tf: {value: 20.8,unit: m}\n" << "Vol: {value: 312622,unit: m^3}\n" << "lcb: 3.48\n"
        << "Abt: {value: 25,unit: m^2}\n" << "hb: {value: 2.5,unit: m}\n" << "Cm: 0.998\n" << "Cwp: 0.83\n" << "At: {value: 0,unit: m^2}\n"
        << "Sapp: {value: 273.3,unit: m^2}\n" << "Cstern: 0\n" << "1+k2: 2\n" << "apply on ship speed direction: false\n";
    return ss.str();
}

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
    input.form_coeff_app = 1.5;
    return input;
}

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
    input.form_coeff_app = 3;
    input.iE = 25;
    return input;
}

BodyStates get_steady_forward_speed_states(double u = 0.)
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
    auto angle = states.convert(ssc::kinematics::EulerAngles(0, 0, 0), rotations);
    states.qr.record(0, std::get<0>(angle));
    states.qi.record(0, std::get<1>(angle));
    states.qj.record(0, std::get<2>(angle));
    states.qk.record(0, std::get<3>(angle));
    return states;
}

TEST_F(HoltropMennenForceModelTest, can_parse)
{
    auto input = HoltropMennenForceModel::parse(get_yaml_input());
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
    ASSERT_DOUBLE_EQ(input.form_coeff_app, 2);
    ASSERT_FALSE(input.apply_on_ship_speed_direction);
    ASSERT_FALSE(input.S.is_initialized());
    ASSERT_FALSE(input.iE.is_initialized());
    ASSERT_FALSE(input.form_coeff_hull.is_initialized());
}

TEST_F(HoltropMennenForceModelTest, no_resistance_at_zero_speed)
{
    auto input = get_Holtrop_Mennen_1982_input();
    auto force_model = HoltropMennenForceModel(input, "body", env);
    auto states = get_steady_forward_speed_states(0);
    ASSERT_DOUBLE_EQ(0,force_model(states, 0).X());
}

TEST_F(HoltropMennenForceModelTest, numerical_example_1982)
{
    // Example from Holtrop & Mennen's original 1984 paper
    auto input = get_Holtrop_Mennen_1982_input();
    auto force_model = HoltropMennenForceModel(input, "body", env);
    BodyStates states = get_steady_forward_speed_states(25. * 1852./3600.);
    // Constant intermediate values
    EXPECT_NEAR(7381.45,force_model.get_S(),0.01);
    EXPECT_NEAR(12.08,force_model.get_iE(),0.01);
    EXPECT_NEAR(1.156,force_model.get_hull_form_coeff(),0.05); // Large margin because the formulae were revised in 1984
    // Resulting forces
    EXPECT_NEAR(869630,force_model.Rf(states),200000); // Large margin because the formulae were revised in 1984
    EXPECT_NEAR(8830,force_model.Rapp(states),10);
    EXPECT_NEAR(557110,force_model.Rw(states),5000); // Large margin because the formulae were revised in 1984
    EXPECT_NEAR(49,force_model.Rb(states),1);
    EXPECT_NEAR(0.,force_model.Rtr(states),1);
    EXPECT_NEAR(221980,force_model.Ra(states),1500); // Somewhat large margin because of what appears to be a rounding error in the reference paper
    EXPECT_NEAR(1793260,-force_model(states,0).X(),200000); // large margin to account for all the margins
}

TEST_F(HoltropMennenForceModelTest, numerical_example_1984)
{
    // Example from Holtrop's 1984 paper (revision of the Holtrop-Mennen method)
    auto input = get_Holtrop_1984_input();
    auto force_model = HoltropMennenForceModel(input, "body", env);
    BodyStates states;
    // Constant intermediate values
    EXPECT_NEAR(1.297,force_model.get_hull_form_coeff(),0.001);
    EXPECT_NEAR(584.9,force_model.get_S(),0.1);
    // Numerical values given in Holtrop (1982)
    const std::vector<double> speed_kts({25,27,29,31,33,35});
    const std::vector<double> Rw({475000,512000,539000,564000,590000,618000});
    const std::vector<double> Rapp({21000,24000,28000,31000,35000,39000});
    const std::vector<double> Rtr({25000,16000,2000,0,0,0});
    const std::vector<double> R({662000,715000,756000,807000,864000,925000});
    for(size_t i = 0 ; i<speed_kts.size() ; i++)
    {
        states = get_steady_forward_speed_states(speed_kts.at(i) * 1852/3600);
        EXPECT_NEAR(force_model.Rw(states),Rw.at(i),1000);
        EXPECT_NEAR(force_model.Rapp(states),Rapp.at(i),1000);
        EXPECT_NEAR(force_model.Rtr(states),Rtr.at(i),1000);
        EXPECT_NEAR(-force_model(states,0).X(),R.at(i),R.at(i)/100);
    }
}
