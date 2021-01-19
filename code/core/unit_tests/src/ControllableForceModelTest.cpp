/*
 * ControllableForceModelTest.cpp
 *
 *  Created on: May 11, 2015
 *      Author: cady
 */

#include <ssc/data_source.hpp>

#include "ControllableForceModelTest.hpp"
#include "ControllableForceModel.hpp"
#include "random_kinematics.hpp"

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

class RandomControllableForce : public ControllableForceModel
{
    public:

        RandomControllableForce(ssc::random_data_generator::DataGenerator& a_)
             : ControllableForceModel("mock", std::vector<std::string>(), YamlPosition(), "body", make_env(a_)), a(a_)
        {
        }

        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const
        {
            ssc::kinematics::Vector6d ret;
            ret(0) = a.random<double>().between(2,3);
            ret(1) = a.random<double>().between(-3,-2);
            ret(2) = a.random<double>().between(20,30);
            ret(3) = a.random<double>().between(-30,-20);
            ret(4) = a.random<double>().between(200,300);
            ret(5) = a.random<double>().between(-300,-200);
            return ret;
        }

        EnvironmentAndFrames get_env() const
        {
            return env;
        }

    private:
        ssc::random_data_generator::DataGenerator a;
};

ControllableForceModelTest::ControllableForceModelTest() : a(ssc::random_data_generator::DataGenerator(545121))
{
}

ControllableForceModelTest::~ControllableForceModelTest()
{
}

void ControllableForceModelTest::SetUp()
{
}

void ControllableForceModelTest::TearDown()
{
}

TEST_F(ControllableForceModelTest, bug_2838)
{
//! [ControllableForceModelTest example]
    ssc::data_source::DataSource command_listener;
    RandomControllableForce F(a);
    BodyStates states;
    states.G = ssc::kinematics::Point("body", 1, 2, 3);
    const double t = a.random<double>();


    auto w = F(states, t, F.get_env(), command_listener);
//! [ControllableForceModelTest example]
//! [ControllableForceModelTest expected output]
    ASSERT_EQ("body", w.get_frame());
    ASSERT_NEAR((states.G - w.get_point()).norm(), 0, 1E-10);
//! [ControllableForceModelTest expected output]
}

