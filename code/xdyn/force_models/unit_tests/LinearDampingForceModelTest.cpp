/*
 * LinearDampingForceModelTest.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */
#include "xdyn/core/unit_tests/generate_body_for_tests.hpp"
#include "LinearDampingForceModelTest.hpp"
#include "LinearDampingForceModel.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"

#define BODY "body 1"

LinearDampingForceModelTest::LinearDampingForceModelTest() : a(ssc::random_data_generator::DataGenerator(82512))
{
}

LinearDampingForceModelTest::~LinearDampingForceModelTest()
{
}

void LinearDampingForceModelTest::SetUp()
{
}

void LinearDampingForceModelTest::TearDown()
{
}

TEST_F(LinearDampingForceModelTest, parser)
{
    const std::string yaml = "        damping matrix at the center of gravity projected in the body frame:\n"
                             "            row 1: [  2,   3,   5,   7,  11,  13]\n"
                             "            row 2: [ 17,  19,  23,  29,  31,  37]\n"
                             "            row 3: [ 41,  43,  47,  53,  59,  61]\n"
                             "            row 4: [ 67,  71,  73,  79,  83,  89]\n"
                             "            row 5: [ 97, 101, 103, 107, 109, 113]\n"
                             "            row 6: [127, 131, 137, 139, 149, 151]\n";
    const Eigen::Matrix<double,6,6> D = LinearDampingForceModel::parse(yaml);
    ASSERT_DOUBLE_EQ(2., D(0,0));
    ASSERT_DOUBLE_EQ(3., D(0,1));
    ASSERT_DOUBLE_EQ(5., D(0,2));
    ASSERT_DOUBLE_EQ(7., D(0,3));
    ASSERT_DOUBLE_EQ(11., D(0,4));
    ASSERT_DOUBLE_EQ(13., D(0,5));
    ASSERT_DOUBLE_EQ(17., D(1,0));
    ASSERT_DOUBLE_EQ(19., D(1,1));
    ASSERT_DOUBLE_EQ(23., D(1,2));
    ASSERT_DOUBLE_EQ(29., D(1,3));
    ASSERT_DOUBLE_EQ(31., D(1,4));
    ASSERT_DOUBLE_EQ(37., D(1,5));
    ASSERT_DOUBLE_EQ(41., D(2,0));
    ASSERT_DOUBLE_EQ(43., D(2,1));
    ASSERT_DOUBLE_EQ(47., D(2,2));
    ASSERT_DOUBLE_EQ(53., D(2,3));
    ASSERT_DOUBLE_EQ(59., D(2,4));
    ASSERT_DOUBLE_EQ(61., D(2,5));
    ASSERT_DOUBLE_EQ(67., D(3,0));
    ASSERT_DOUBLE_EQ(71., D(3,1));
    ASSERT_DOUBLE_EQ(73., D(3,2));
    ASSERT_DOUBLE_EQ(79., D(3,3));
    ASSERT_DOUBLE_EQ(83., D(3,4));
    ASSERT_DOUBLE_EQ(89., D(3,5));
    ASSERT_DOUBLE_EQ(97., D(4,0));
    ASSERT_DOUBLE_EQ(101., D(4,1));
    ASSERT_DOUBLE_EQ(103., D(4,2));
    ASSERT_DOUBLE_EQ(107., D(4,3));
    ASSERT_DOUBLE_EQ(109., D(4,4));
    ASSERT_DOUBLE_EQ(113., D(4,5));
    ASSERT_DOUBLE_EQ(127., D(5,0));
    ASSERT_DOUBLE_EQ(131., D(5,1));
    ASSERT_DOUBLE_EQ(137., D(5,2));
    ASSERT_DOUBLE_EQ(139., D(5,3));
    ASSERT_DOUBLE_EQ(149., D(5,4));
    ASSERT_DOUBLE_EQ(151., D(5,5));
}

TEST_F(LinearDampingForceModelTest, example)
{
    //! [LinearDampingForceModelTest example]
    const double EPS = 1e-9;
    Eigen::Matrix<double,6,6> D;
    double u,v,w,p,q,r;
    EnvironmentAndFrames env;
    BodyStates states = get_body(BODY)->get_states();
    D <<  2,   3,   5,   7,  11,  13,
         17,  19,  23,  29,  31,  37,
         41,  43,  47,  53,  59,  61,
         67,  71,  73,  79,  83,  89,
         97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151;
    LinearDampingForceModel F(D,BODY, env);
    ASSERT_EQ("linear damping",F.model_name());
    for (size_t i=0;i<100;++i)
    {
        states.u.record(0, u = a.random<double>().between(-10.0,+10.0));
        states.v.record(0, v = a.random<double>().between(-10.0,+10.0));
        states.w.record(0, w = a.random<double>().between(-10.0,+10.0));
        states.p.record(0, p = a.random<double>().between(-10.0,+10.0));
        states.q.record(0, q = a.random<double>().between(-10.0,+10.0));
        states.r.record(0, r = a.random<double>().between(-10.0,+10.0));
        const ssc::kinematics::Wrench f = F(states, a.random<double>(), env);
        ASSERT_EQ(BODY, f.get_frame());
        for (int j=0;j<3;++j)
        {
            const int k = j+3;
            ASSERT_NEAR(D(j,0)*states.u()+D(j,1)*states.v()+D(j,2)*states.w()+D(j,3)*states.p()+D(j,4)*states.q()+D(j,5)*states.r(),-f.force[j],EPS)<<" row: "<<i << ", col:"<<j;
            ASSERT_NEAR(D(k,0)*states.u()+D(k,1)*states.v()+D(k,2)*states.w()+D(k,3)*states.p()+D(k,4)*states.q()+D(k,5)*states.r(),-f.torque[j],EPS)<<" row: "<<i << ", col:"<<k;
        }
    }
    //! [LinearDampingForceModelTest example]
}
