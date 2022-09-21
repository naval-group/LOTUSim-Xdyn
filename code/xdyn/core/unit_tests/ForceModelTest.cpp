/*
 * ForceModelTest.cpp
 *
 *  Created on: May 11, 2015
 *      Author: cady
 */
#include "ForceModelTest.hpp"
#include "ForceModel.hpp"
#include "random_kinematics.hpp"
#include <ssc/data_source.hpp>

EnvironmentAndFrames make_env(ssc::random_data_generator::DataGenerator& a);
EnvironmentAndFrames make_env(ssc::random_data_generator::DataGenerator& a)
{
    EnvironmentAndFrames env;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    const auto bTa = random_transform(a, "body", "mock");
    env.k->add(bTa);
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    return env;
}

class RandomForce : public ForceModel
{
    public:

        RandomForce(ssc::random_data_generator::DataGenerator& a_, const EnvironmentAndFrames& env)
             : ForceModel("mock", std::vector<std::string>(), YamlPosition(), "body", env), a(a_)
        {
        }

        Wrench get_force(
            const BodyStates& /*states*/,
            const double /*t*/,
            const EnvironmentAndFrames& /*env*/,
            const std::map<std::string,double>& /*commands*/) const
        {
            ssc::kinematics::Vector6d ret;
            ret(0) = a.random<double>().between(+2, +3);
            ret(1) = a.random<double>().between(-3, -2);
            ret(2) = a.random<double>().between(+20, +30);
            ret(3) = a.random<double>().between(-30, -20);
            ret(4) = a.random<double>().between(+200, +300);
            ret(5) = a.random<double>().between(-300, -200);
            return Wrench(ssc::kinematics::Point(name, 0, 0, 0), name, ret);
        }

    private:
        ssc::random_data_generator::DataGenerator a;
};

ForceModelTest::ForceModelTest() : a(ssc::random_data_generator::DataGenerator(545121))
{
}

ForceModelTest::~ForceModelTest()
{
}

void ForceModelTest::SetUp()
{
}

void ForceModelTest::TearDown()
{
}

TEST_F(ForceModelTest, bug_2838)
{
//! [ForceModelTest example]
    ssc::data_source::DataSource command_listener;
    EnvironmentAndFrames env = make_env(a);
    RandomForce F(a, env);
    BodyStates states;
    states.G = ssc::kinematics::Point("body", 1, 2, 3);
    const double t = a.random<double>();


    auto w = F(states, t, env, command_listener);
//! [ForceModelTest example]
//! [ForceModelTest expected output]
    ASSERT_EQ("body", w.get_frame());
    ASSERT_NEAR((states.G - w.get_point()).norm(), 0, 1E-10);
//! [ForceModelTest expected output]
}
