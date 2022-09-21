#include "MMGManeuveringForceModelTest.hpp"
#include "MMGManeuveringForceModel.hpp"

MMGManeuveringForceModelTest::MMGManeuveringForceModelTest()
{
}

MMGManeuveringForceModelTest::~MMGManeuveringForceModelTest()
{
}

std::string get_input();
std::string get_input()
{
    std::stringstream ss;
    ss << "calculation point in body frame:\n"
        <<"    x: {value: -11.1, unit: m}\n"
        <<"    y: {value: 0, unit: m}\n"
        <<"    z: {value: 0, unit: m}\n"
        <<"Lpp: {value: 320, unit: m}\n"
        <<"T: {value: 20.8, unit: m}\n"
        <<"Xvv: -0.04\n"
        <<"Xrr: 0.011\n"
        <<"Xvr: 0.002\n"
        <<"Xvvvv: 0.771\n"
        <<"Yv: -0.315\n"
        <<"Yr: 0.083\n"
        <<"Yvvv: -1.607\n"
        <<"Yrvv: 0.379\n"
        <<"Yvrr: -0.391\n"
        <<"Yrrr: 0.008\n"
        <<"Nv: -0.137\n"
        <<"Nr: -0.049\n"
        <<"Nvvv: -0.03\n"
        <<"Nrvv: -0.294\n"
        <<"Nvrr: 0.055\n"
        <<"Nrrr: -0.013\n";
    return ss.str();
}

TEST_F(MMGManeuveringForceModelTest, can_parse)
{
    const auto data = MMGManeuveringForceModel::parse(get_input());
    ASSERT_DOUBLE_EQ(data.application_point.x, -11.1);
    ASSERT_DOUBLE_EQ(data.application_point.y, 0);
    ASSERT_DOUBLE_EQ(data.application_point.z, 0);
    ASSERT_DOUBLE_EQ(data.Lpp, 320);
    ASSERT_DOUBLE_EQ(data.T, 20.8);
    ASSERT_DOUBLE_EQ(data.Xvv, -0.04);
    ASSERT_DOUBLE_EQ(data.Xrr, 0.011);
    ASSERT_DOUBLE_EQ(data.Xvr, 0.002);
    ASSERT_DOUBLE_EQ(data.Xvvvv, 0.771);
    ASSERT_DOUBLE_EQ(data.Yv, -0.315);
    ASSERT_DOUBLE_EQ(data.Yr, 0.083);
    ASSERT_DOUBLE_EQ(data.Yvvv, -1.607);
    ASSERT_DOUBLE_EQ(data.Yrvv, 0.379);
    ASSERT_DOUBLE_EQ(data.Yvrr, -0.391);
    ASSERT_DOUBLE_EQ(data.Yrrr, 0.008);
    ASSERT_DOUBLE_EQ(data.Nv, -0.137);
    ASSERT_DOUBLE_EQ(data.Nr, -0.049);
    ASSERT_DOUBLE_EQ(data.Nvvv, -0.03);
    ASSERT_DOUBLE_EQ(data.Nrvv, -0.294);
    ASSERT_DOUBLE_EQ(data.Nvrr, 0.055);
    ASSERT_DOUBLE_EQ(data.Nrrr, -0.013);
}

BodyStates get_states();
BodyStates get_states()
{
    BodyStates states(0);
    states.convention = YamlRotation("angle", {"z","y'","x''"});
    states.x.record(0, 0);
    states.y.record(0, 0);
    states.z.record(0, 0);
    states.u.record(0, 1);
    states.v.record(0, 2);
    states.w.record(0, 0);
    states.p.record(0, 0);
    states.q.record(0, 0);
    states.r.record(0, 3);
    states.qr.record(0, 1);
    states.qi.record(0, 0);
    states.qj.record(0, 0);
    states.qr.record(0, 0);
    return states;
}

TEST_F(MMGManeuveringForceModelTest, example)
{
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    auto force_model = MMGManeuveringForceModel(MMGManeuveringForceModel::parse(get_input()), "body", env);
    auto states = get_states();
    auto F = force_model.get_force(states, 0, env, {});
    ASSERT_DOUBLE_EQ(F.X(), 35918728522.330902);
    ASSERT_DOUBLE_EQ(F.Y(), 2003246596640.8945);
    ASSERT_DOUBLE_EQ(F.Z(), 0);
    ASSERT_DOUBLE_EQ(F.K(), 0);
    ASSERT_DOUBLE_EQ(F.M(), 0);
    ASSERT_DOUBLE_EQ(F.N(), -455883858001617.88);
}
