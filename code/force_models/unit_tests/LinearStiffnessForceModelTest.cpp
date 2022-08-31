#include <cmath>

#include "LinearStiffnessForceModel.hpp"
#include "EnvironmentAndFrames.hpp"

#include "LinearStiffnessForceModelTest.hpp"

LinearStiffnessForceModelTest::LinearStiffnessForceModelTest() : a(ssc::random_data_generator::DataGenerator(16752))
{
}

TEST_F(LinearStiffnessForceModelTest, can_parse)
{
    const std::string yaml = "        name: test\n"
                             "        stiffness matrix:\n"
                             "            row 1: [  2,   3,   5,   7,  11,  13]\n"
                             "            row 2: [ 17,  19,  23,  29,  31,  37]\n"
                             "            row 3: [ 41,  43,  47,  53,  59,  61]\n"
                             "            row 4: [ 67,  71,  73,  79,  83,  89]\n"
                             "            row 5: [ 97, 101, 103, 107, 109, 113]\n"
                             "            row 6: [127, 131, 137, 139, 149, 151]\n";
    const LinearStiffnessForceModel::Input input = LinearStiffnessForceModel::parse(yaml);
    ASSERT_EQ(input.name, "test");
    const Eigen::Matrix<double,6,6>& K = input.K;
    ASSERT_DOUBLE_EQ(2., K(0,0));
    ASSERT_DOUBLE_EQ(3., K(0,1));
    ASSERT_DOUBLE_EQ(5., K(0,2));
    ASSERT_DOUBLE_EQ(7., K(0,3));
    ASSERT_DOUBLE_EQ(11., K(0,4));
    ASSERT_DOUBLE_EQ(13., K(0,5));
    ASSERT_DOUBLE_EQ(17., K(1,0));
    ASSERT_DOUBLE_EQ(19., K(1,1));
    ASSERT_DOUBLE_EQ(23., K(1,2));
    ASSERT_DOUBLE_EQ(29., K(1,3));
    ASSERT_DOUBLE_EQ(31., K(1,4));
    ASSERT_DOUBLE_EQ(37., K(1,5));
    ASSERT_DOUBLE_EQ(41., K(2,0));
    ASSERT_DOUBLE_EQ(43., K(2,1));
    ASSERT_DOUBLE_EQ(47., K(2,2));
    ASSERT_DOUBLE_EQ(53., K(2,3));
    ASSERT_DOUBLE_EQ(59., K(2,4));
    ASSERT_DOUBLE_EQ(61., K(2,5));
    ASSERT_DOUBLE_EQ(67., K(3,0));
    ASSERT_DOUBLE_EQ(71., K(3,1));
    ASSERT_DOUBLE_EQ(73., K(3,2));
    ASSERT_DOUBLE_EQ(79., K(3,3));
    ASSERT_DOUBLE_EQ(83., K(3,4));
    ASSERT_DOUBLE_EQ(89., K(3,5));
    ASSERT_DOUBLE_EQ(97., K(4,0));
    ASSERT_DOUBLE_EQ(101., K(4,1));
    ASSERT_DOUBLE_EQ(103., K(4,2));
    ASSERT_DOUBLE_EQ(107., K(4,3));
    ASSERT_DOUBLE_EQ(109., K(4,4));
    ASSERT_DOUBLE_EQ(113., K(4,5));
    ASSERT_DOUBLE_EQ(127., K(5,0));
    ASSERT_DOUBLE_EQ(131., K(5,1));
    ASSERT_DOUBLE_EQ(137., K(5,2));
    ASSERT_DOUBLE_EQ(139., K(5,3));
    ASSERT_DOUBLE_EQ(149., K(5,4));
    ASSERT_DOUBLE_EQ(151., K(5,5));
}

TEST_F(LinearStiffnessForceModelTest, can_parse_optional)
{
    const std::string yaml = "        name: test\n"
                             "        stiffness matrix:\n"
                             "            row 1: [  2,   3,   5,   7,  11,  13]\n"
                             "            row 2: [ 17,  19,  23,  29,  31,  37]\n"
                             "            row 3: [ 41,  43,  47,  53,  59,  61]\n"
                             "            row 4: [ 67,  71,  73,  79,  83,  89]\n"
                             "            row 5: [ 97, 101, 103, 107, 109, 113]\n"
                             "            row 6: [127, 131, 137, 139, 149, 151]\n"
                             "        equilibrium position:\n"
                             "            frame: body\n"
                             "            x: {value: 1, unit: m}\n"
                             "            y: {value: 2, unit: m}\n"
                             "            z: {value: 3, unit: m}\n"
                             "            phi: {value: 4, unit: rad}\n"
                             "            theta: {value: 5, unit: rad}\n"
                             "            psi: {value: 6, unit: rad}\n";
    const LinearStiffnessForceModel::Input input = LinearStiffnessForceModel::parse(yaml);
    ASSERT_TRUE(input.equilibrium_position);
    ASSERT_EQ(input.equilibrium_position.get().frame, "body");
    ASSERT_DOUBLE_EQ(input.equilibrium_position.get().coordinates.x, 1);
    ASSERT_DOUBLE_EQ(input.equilibrium_position.get().coordinates.y, 2);
    ASSERT_DOUBLE_EQ(input.equilibrium_position.get().coordinates.z, 3);
    ASSERT_DOUBLE_EQ(input.equilibrium_position.get().angle.phi, 4);
    ASSERT_DOUBLE_EQ(input.equilibrium_position.get().angle.theta, 5);
    ASSERT_DOUBLE_EQ(input.equilibrium_position.get().angle.psi, 6);
}

BodyStates get_position_states(const double x, const double y, const double z, const double phi, const double theta, const double psi);
BodyStates get_position_states(const double x, const double y, const double z, const double phi, const double theta, const double psi)
{
    BodyStates states(0);
    states.convention = YamlRotation("angle", {"z","y'","x''"});
    states.x.record(0, x);
    states.y.record(0, y);
    states.z.record(0, z);
    states.u.record(0, 0);
    states.v.record(0, 0);
    states.w.record(0, 0);
    states.p.record(0, 0);
    states.q.record(0, 0);
    states.r.record(0, 0);
    ssc::kinematics::EulerAngles angles(phi, theta, psi);
    auto quat = states.convert(angles, states.convention);
    states.qr.record(0, std::get<0>(quat));
    states.qi.record(0, std::get<1>(quat));
    states.qj.record(0, std::get<2>(quat));
    states.qk.record(0, std::get<3>(quat));
    angles = states.get_angles();
    return states;
}

TEST_F(LinearStiffnessForceModelTest, example)
{
    const double EPS = 1e-9;
    double x,y,z,phi,theta,psi;
    EnvironmentAndFrames env;
    BodyStates states;
    LinearStiffnessForceModel::Input input;
    input.name = "test";
    input.K <<  2,   3,   5,   7,  11,  13,
                17,  19,  23,  29,  31,  37,
                41,  43,  47,  53,  59,  61,
                67,  71,  73,  79,  83,  89,
                97, 101, 103, 107, 109, 113,
                127, 131, 137, 139, 149, 151;
    const Eigen::Matrix<double,6,6>& K = input.K;
    LinearStiffnessForceModel F(input, "body", env);
    for (size_t i=0;i<100;++i)
    {
        x = a.random<double>().between(-10.0,+10.0);
        y = a.random<double>().between(-10.0,+10.0);
        z = a.random<double>().between(-10.0,+10.0);
        phi = a.random<double>().between(-M_PI/3,M_PI/3); // Restricting the range of angles to avoid gimbal lock
        theta = a.random<double>().between(-M_PI/3,M_PI/3);
        psi = a.random<double>().between(-M_PI/3,M_PI/3);
        states = get_position_states(x,y,z,phi,theta,psi);
        const ssc::kinematics::RotationMatrix rotation = states.get_rot_from_ned_to_body().transpose();
        const Wrench f = F.get_force(states, a.random<double>(), env, {});
        ASSERT_EQ("NED", f.get_frame());
        for (int j=0;j<3;++j)
        {
            const int k = j+3;
            ASSERT_NEAR(K(j,0)*x + K(j,1)*y + K(j,2)*z + K(j,3)*phi + K(j,4)*theta + K(j,5)*psi, -f.force[j],  EPS)<<" row: "<<i << ", col:"<<j;
            ASSERT_NEAR(K(k,0)*x + K(k,1)*y + K(k,2)*z + K(k,3)*phi + K(k,4)*theta + K(k,5)*psi, -(rotation*f.torque)[j], EPS)<<" row: "<<i << ", col:"<<k;
        }
    }
}

TEST_F(LinearStiffnessForceModelTest, example_with_equilibrium_input)
{
    const double EPS = 1e-9;
    double x,y,z,phi,theta,psi;
    EnvironmentAndFrames env;
    BodyStates states;
    LinearStiffnessForceModel::Input input;
    input.name = "test";
    input.K <<  2,   3,   5,   7,  11,  13,
                17,  19,  23,  29,  31,  37,
                41,  43,  47,  53,  59,  61,
                67,  71,  73,  79,  83,  89,
                97, 101, 103, 107, 109, 113,
                127, 131, 137, 139, 149, 151;
    const double x0 = 1;
    const double y0 = 2;
    const double z0 = 3;
    const double phi0 = M_PI/10.;
    const double theta0 = M_PI/8.;
    const double psi0 = M_PI/12.;
    input.equilibrium_position = YamlPosition(YamlCoordinates(x0,y0,z0), YamlAngle(phi0,theta0,psi0), "body");
    const Eigen::Matrix<double,6,6>& K = input.K;
    LinearStiffnessForceModel F(input, "body", env);
    for (size_t i=0;i<10;++i)
    {
        x = a.random<double>().between(-10.0,+10.0);
        y = a.random<double>().between(-10.0,+10.0);
        z = a.random<double>().between(-10.0,+10.0);
        phi = a.random<double>().between(-M_PI/3,M_PI/3); // Restricting the range of angles to avoid gimbal lock
        theta = a.random<double>().between(-M_PI/3,M_PI/3);
        psi = a.random<double>().between(-M_PI/3,M_PI/3);
        states = get_position_states(x,y,z,phi,theta,psi);
        const ssc::kinematics::RotationMatrix rotation = states.get_rot_from_ned_to_body().transpose();
        const Wrench f = F.get_force(states, a.random<double>(), env, {});
        ASSERT_EQ("NED", f.get_frame());
        for (int j=0;j<3;++j)
        {
            const int k = j+3;
            ASSERT_NEAR(K(j,0)*(x-x0) + K(j,1)*(y-y0) + K(j,2)*(z-z0) + K(j,3)*(phi-phi0) + K(j,4)*(theta-theta0) + K(j,5)*(psi-psi0), -f.force[j],  EPS)<<" row: "<<i << ", col:"<<j;
            ASSERT_NEAR(K(k,0)*(x-x0) + K(k,1)*(y-y0) + K(k,2)*(z-z0) + K(k,3)*(phi-phi0) + K(k,4)*(theta-theta0) + K(k,5)*(psi-psi0), -(rotation*f.torque)[j], EPS)<<" row: "<<i << ", col:"<<k;
        }
    }
}