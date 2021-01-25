#include "Wrench.hpp"
#include "WrenchTest.hpp"

#include <cmath>
#include "yaml2eigen.hpp"
#include "YamlPosition.hpp"
#include "YamlRotation.hpp"
#include "yaml2eigen.hpp"

WrenchTest::WrenchTest(): a(ssc::random_data_generator::DataGenerator(267953))
{
}

WrenchTest::~WrenchTest()
{
}

TEST_F(WrenchTest, CanGetMembers)
{
    const std::string frame("expression_frame");
    const ssc::kinematics::Point application_point("point_frame", a.random<double>(), a.random<double>(), a.random<double>());
    ssc::kinematics::Vector6d force;
    force << a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>();
    const Wrench wrench(application_point, frame, force);
    ASSERT_EQ(frame, wrench.get_frame());
    ASSERT_EQ(application_point.get_frame(), wrench.get_point().get_frame());
    ASSERT_TRUE(application_point.v.isApprox(wrench.get_point().v));
    ASSERT_TRUE(force.isApprox(wrench.to_vector()));
}

TEST_F(WrenchTest, CanChangeFrame)
{
    const ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics);
    const double psi = 30 * M_PI / 180;
    const YamlAngle angles(0,0,psi);
    const YamlPosition position(YamlCoordinates(a.random<double>(), a.random<double>(), a.random<double>()), angles, "frame2");
    YamlRotation rotation_convention("angle", { "z", "y'", "x''" });
    k->add(make_transform(position, "frame1", rotation_convention));
    const ssc::kinematics::Point P("frame1", 1, 2, 3);
    Eigen::Vector3d force;
    force << a.random<double>(), a.random<double>(), a.random<double>();
    Eigen::Vector3d torque;
    torque << a.random<double>(), a.random<double>(), a.random<double>();
    Wrench wrench(P, "frame2", force, torque);
    wrench.change_frame("frame1", k);
    ASSERT_EQ("frame1", wrench.get_frame());
    ASSERT_EQ(P.get_frame(), wrench.get_point().get_frame());
    ASSERT_TRUE(P.v.isApprox(wrench.get_point().v));
    const ssc::kinematics::RotationMatrix R = angle2matrix(angles, rotation_convention);
    auto Rforce = R.inverse()*force;
    auto Rtorque = R.inverse()*torque;
    ASSERT_DOUBLE_EQ(Rforce(0), wrench.X());
    ASSERT_DOUBLE_EQ(Rforce(1), wrench.Y());
    ASSERT_DOUBLE_EQ(Rforce(2), wrench.Z());
    ASSERT_DOUBLE_EQ(Rtorque(0), wrench.K());
    ASSERT_DOUBLE_EQ(Rtorque(1), wrench.M());
    ASSERT_DOUBLE_EQ(Rtorque(2), wrench.N());
}

TEST_F(WrenchTest, CanChangePointInSameFrame)
{
    const ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics);
    const ssc::kinematics::Point A("frame1", 1, 2, 3);
    const ssc::kinematics::Point B("frame1", 4, 5, 6);
    Eigen::Vector3d force;
    force << a.random<double>(), a.random<double>(), a.random<double>();
    Eigen::Vector3d torque;
    torque << a.random<double>(), a.random<double>(), a.random<double>();
    Wrench wrench(A, "frame1", force, torque);
    wrench.transport_to(B, k);
    ASSERT_EQ("frame1", wrench.get_frame());
    ASSERT_EQ(B.get_frame(), wrench.get_point().get_frame());
    ASSERT_TRUE(B.v.isApprox(wrench.get_point().v));
    ASSERT_TRUE(force.isApprox(wrench.get_force()));
    ASSERT_TRUE((torque + (A.v - B.v).cross(force)).isApprox(wrench.get_torque()));
}

TEST_F(WrenchTest, CanChangePointInDifferentFrame)
{
    const ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics);
    const double psi = 30 * M_PI / 180;
    const YamlPosition position(YamlCoordinates(10,0,0), YamlAngle(0, 0, 90*M_PI/180), "frame1");
    YamlRotation rotation_convention("angle", { "z", "y'", "x''" });
    auto T_from_1_to_2 = make_transform(position, "frame2", rotation_convention);
    std::cout << "T = " << T_from_1_to_2 << std::endl;
    k->add(T_from_1_to_2);
    const ssc::kinematics::Point A("frame1", 1, 2, 3);
    const ssc::kinematics::Point B("frame1", 4, 5, 6);
    Eigen::Vector3d force;
    force << a.random<double>(), a.random<double>(), a.random<double>();
    Eigen::Vector3d torque;
    torque << a.random<double>(), a.random<double>(), a.random<double>();
    Wrench wrench(A, "frame2", force, torque);
    wrench.transport_to(B, k);
    ASSERT_EQ("frame2", wrench.get_frame());
    ASSERT_EQ(B.get_frame(), wrench.get_point().get_frame());
    ASSERT_TRUE(B.v.isApprox(wrench.get_point().v));
    ASSERT_TRUE(force.isApprox(wrench.get_force()));
    ssc::kinematics::Point B_in_2("frame2",T_from_1_to_2.inverse().get_point().v+T_from_1_to_2.get_rot().inverse()*B.v);
    ssc::kinematics::Point A_in_2("frame2",T_from_1_to_2.inverse().get_point().v+T_from_1_to_2.get_rot().inverse()*A.v);
    Eigen::Vector3d BA_2 = A_in_2 - B_in_2;
    ASSERT_TRUE((torque + BA_2.cross(force)).isApprox(wrench.get_torque()));
}
