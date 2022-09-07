#include "Wrench.hpp"
#include "WrenchTest.hpp"

#include <cmath>
#include "yaml2eigen.hpp"
#include "YamlPosition.hpp"
#include "YamlRotation.hpp"
#include "yaml2eigen.hpp"
#include "InternalErrorException.hpp"

WrenchTest::WrenchTest(): a(ssc::random_data_generator::DataGenerator(267953))
{
}

WrenchTest::~WrenchTest()
{
}

TEST_F(WrenchTest, can_get_members)
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

TEST_F(WrenchTest, can_change_frame)
{

    // Python code to reproduce the test:
    /*
    import numpy as np

    phi = np.radians(12)
    theta = np.radians(47)
    psi = np.radians(31)

    Rz = np.array([[np.cos(psi), -np.sin(psi), 0],
                   [np.sin(psi),  np.cos(psi), 0],
                   [0          ,  0          , 1]])

    Ry = np.array([[ np.cos(theta), 0, np.sin(theta)],
                   [ 0            , 1, 0            ],
                   [-np.sin(theta), 0, np.cos(theta)]])

    Rx = np.array([[1, 0          ,  0          ],
                   [0, np.cos(phi), -np.sin(phi)],
                   [0, np.sin(phi),  np.cos(phi)]])

    force = np.array([1000, 2000, 3000])
    torque = np.array([4000, 5000, 6000])

    R_force = force.dot(Rz).dot(Ry).dot(Rx)
    R_torque = torque.dot(Rz).dot(Ry).dot(Rx)
    */
    const ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics);
    const YamlAngle angles(12 * M_PI / 180, 47 * M_PI / 180, 31 * M_PI / 180);
    const YamlPosition position(YamlCoordinates(a.random<double>(), a.random<double>(), a.random<double>()), angles, "frame2");
    YamlRotation rotation_convention("angle", { "z", "y'", "x''" });
    k->add(make_transform(position, "frame1", rotation_convention));
    const ssc::kinematics::Point P("frame1", 1, 2, 3);
    Eigen::Vector3d force;
    force << 1000, 2000, 3000;
    Eigen::Vector3d torque;
    torque << 4000, 5000, 6000;
    Wrench wrench(P, "frame2", force, torque);
    wrench.change_frame("frame1", k);
    ASSERT_EQ("frame1", wrench.get_frame());
    ASSERT_EQ(P.get_frame(), wrench.get_point().get_frame());
    ASSERT_TRUE(P.v.isApprox(wrench.get_point().v));
    const double epsilon = 1e-8;
    ASSERT_NEAR(-906.96416656, wrench.X(), epsilon);
    ASSERT_NEAR(1885.44386495, wrench.Y(), epsilon);
    ASSERT_NEAR(3102.01828375, wrench.Z(), epsilon);
    ASSERT_NEAR(-293.49982391, wrench.K(), epsilon);
    ASSERT_NEAR(3940.74904089, wrench.M(), epsilon);
    ASSERT_NEAR(7834.81683577, wrench.N(), epsilon);
}

TEST_F(WrenchTest, can_change_point_in_same_frame)
{
    // Python code to reproduce the test:
    /*
    import numpy as np

    force = np.array([1000, 2000, 3000])
    torque = np.array([4000, 5000, 6000])

    A = np.array([1, 2, 3])
    B = np.array([6, 5, 4])

    torque_B =  torque + np.cross(A - B, force)
     */
    const ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics);
    const ssc::kinematics::Point A("frame1", 1, 2, 3);
    const ssc::kinematics::Point B("frame1", 6, 5, 4);
    Eigen::Vector3d force;
    force << 1000, 2000, 3000;
    Eigen::Vector3d torque;
    torque << 4000, 5000, 6000;
    Wrench wrench(A, "frame1", force, torque);
    wrench.transport_to(B, k);
    ASSERT_EQ("frame1", wrench.get_frame());
    ASSERT_EQ(B.get_frame(), wrench.get_point().get_frame());
    ASSERT_TRUE(B.v.isApprox(wrench.get_point().v));
    ASSERT_TRUE(force.isApprox(wrench.get_force()));
    ASSERT_DOUBLE_EQ(-3000, wrench.K());
    ASSERT_DOUBLE_EQ(19000, wrench.M());
    ASSERT_DOUBLE_EQ(-1000, wrench.N());
}

TEST_F(WrenchTest, can_change_point_in_different_frame)
{
    const ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics);
    const YamlPosition position(YamlCoordinates(a.random<double>(),a.random<double>(),a.random<double>()),
                                YamlAngle(a.random<double>(), a.random<double>(), a.random<double>()),
                                "frame1");
    YamlRotation rotation_convention("angle", { "z", "y'", "x''" });
    auto T_from_1_to_2 = make_transform(position, "frame2", rotation_convention);
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

TEST_F(WrenchTest, addition_operator_works)
{
    const ssc::kinematics::Point A("frame1", 1, 2, 3);
    ssc::kinematics::Vector6d force1;
    force1 << a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>();
    const Wrench wrench1(A, "frame1", force1);
    ssc::kinematics::Vector6d force2;
    force2 << a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>();
    const Wrench wrench2(A, "frame1", force2);
    const Wrench wrench3 = wrench1 + wrench2;
    ASSERT_EQ("frame1", wrench3.get_frame());
    ASSERT_EQ(A.get_frame(), wrench3.get_point().get_frame());
    ASSERT_TRUE(A.v.isApprox(wrench3.get_point().v));
    ASSERT_TRUE((force1+force2).isApprox(wrench3.to_vector()));
}

TEST_F(WrenchTest, addition_operator_throws_if_different_point_or_frame)
{
    const ssc::kinematics::Point A("frame1", 1, 2, 3);
    ssc::kinematics::Vector6d force1;
    force1 << a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>();
    const Wrench wrench1(A, "frame1", force1);
    // Test: different point in same frame
    const ssc::kinematics::Point B("frame1", 4, 5, 6);
    ssc::kinematics::Vector6d force2;
    force2 << a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>();
    const Wrench wrench2(B, "frame1", force2);
    ASSERT_THROW(wrench1 + wrench2, InternalErrorException);
    // Test: point with same coordinates in different frame
    const ssc::kinematics::Point C("frame2", 1, 2, 3);
    const Wrench wrench3(C, "frame1", force2);
    ASSERT_THROW(wrench1 + wrench3, InternalErrorException);
    // Test: same point but different frame
    const Wrench wrench4(A, "frame2", force2);
    ASSERT_THROW(wrench1 + wrench3, InternalErrorException);
}

TEST_F(WrenchTest, can_add_wrenches_in_different_frames_and_point_using_method_add_instead_of_plus_operator)
{
    const ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics);
    const YamlPosition position(YamlCoordinates(10,0,0), YamlAngle(0, 0, 90*M_PI/180), "frame1");
    YamlRotation rotation_convention("angle", { "z", "y'", "x''" });
    auto T_from_1_to_2 = make_transform(position, "frame2", rotation_convention);
    k->add(T_from_1_to_2);
    const ssc::kinematics::Point A("frame1", 1, 2, 3);
    ssc::kinematics::Vector6d force1;
    force1 << a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>();
    const Wrench wrench1(A, "frame1", force1);

    const ssc::kinematics::Point B("frame2", 4, 5, 6);
    ssc::kinematics::Vector6d force2;
    force2 << a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>(), a.random<double>();
    const Wrench wrench2(B, "frame2", force2);

    const Wrench wrench3 = wrench1.add(wrench2, k);
    ASSERT_EQ(wrench3.get_frame(), wrench1.get_frame());
    ASSERT_EQ(wrench3.get_point().get_frame(), wrench1.get_point().get_frame());
    ASSERT_TRUE(wrench3.get_point().v.isApprox(wrench1.get_point().v));
    ASSERT_TRUE(wrench3.to_vector().isApprox((wrench1 + wrench2.change_point_and_frame(A, "frame1", k)).to_vector()));

}
