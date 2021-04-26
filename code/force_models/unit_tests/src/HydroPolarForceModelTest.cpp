#include <cmath>

#include "HydroPolarForceModelTest.hpp"
#include "HydroPolarForceModel.hpp"

HydroPolarForceModelTest::HydroPolarForceModelTest()
{
}

TEST_F(HydroPolarForceModelTest, can_parse)
{
    std::stringstream ss;
    ss << "name: test\n" <<
          "position of calculation frame:\n" <<
          "    frame: body\n" <<
          "    x: {value: 1, unit: m}\n" <<
          "    y: {value: 2, unit: m}\n" <<
          "    z: {value: 3, unit: m}\n" <<
          "    phi: {value: 10, unit: deg}\n" <<
          "    theta: {value: 20, unit: deg}\n" <<
          "    psi: {value: 30, unit: deg}\n" <<
          "reference area: {value: 1000, unit: m^2}\n" <<
          "angle of attack: {unit: deg, values: [0,7,9,12,28,60,90,120,150,180]}\n" <<
          "lift coefficient: [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]\n" <<
          "drag coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]\n" <<
          "take waves orbital velocity into account: false";
    const HydroPolarForceModel::Input input = HydroPolarForceModel::parse(ss.str());
    ASSERT_EQ(input.name, "test");
    ASSERT_DOUBLE_EQ(input.reference_area, 1000);
    ASSERT_DOUBLE_EQ(input.internal_frame.coordinates.x, 1);
    ASSERT_DOUBLE_EQ(input.internal_frame.coordinates.y, 2);
    ASSERT_DOUBLE_EQ(input.internal_frame.coordinates.z, 3);
    ASSERT_DOUBLE_EQ(input.internal_frame.angle.phi, 10*M_PI/180);
    ASSERT_DOUBLE_EQ(input.internal_frame.angle.theta, 20*M_PI/180);
    ASSERT_DOUBLE_EQ(input.internal_frame.angle.psi, 30*M_PI/180);
    const std::vector<double> AoA = {0,7,9,12,28,60,90,120,150,180};
    const std::vector<double> Cl = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    const std::vector<double> Cd = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    ASSERT_EQ(input.angle_of_attack.size(), 10);
    ASSERT_EQ(input.lift_coefficient.size(), 10);
    ASSERT_EQ(input.drag_coefficient.size(), 10);
    for (size_t i=0 ; i<10 ; i++)
    {
        ASSERT_DOUBLE_EQ(input.angle_of_attack.at(i), M_PI*AoA.at(i)/180);
        ASSERT_DOUBLE_EQ(input.lift_coefficient.at(i), Cl.at(i));
        ASSERT_DOUBLE_EQ(input.drag_coefficient.at(i), Cd.at(i));
    }
    ASSERT_FALSE(input.moment_coefficient.is_initialized());
    ASSERT_FALSE(input.cord_length.is_initialized());
}

TEST_F(HydroPolarForceModelTest, can_parse_optional_output)
{
    std::stringstream ss;
    ss << "name: test\n" <<
          "position of calculation frame:\n" <<
          "    frame: body\n" <<
          "    x: {value: 1, unit: m}\n" <<
          "    y: {value: 2, unit: m}\n" <<
          "    z: {value: 3, unit: m}\n" <<
          "    phi: {value: 10, unit: deg}\n" <<
          "    theta: {value: 20, unit: deg}\n" <<
          "    psi: {value: 30, unit: deg}\n" <<
          "reference area: {value: 1000, unit: m^2}\n" <<
          "angle of attack: {unit: deg, values: [0,7,9,12,28,60,90,120,150,180]}\n" <<
          "lift coefficient: [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]\n" <<
          "drag coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]\n" <<
          "moment coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]\n" <<
          "cord length: {value: 5, unit: m}\n" <<
          "take waves orbital velocity into account: false";
    const HydroPolarForceModel::Input input = HydroPolarForceModel::parse(ss.str());
    ASSERT_TRUE(input.moment_coefficient.is_initialized());
    ASSERT_TRUE(input.cord_length.is_initialized());
    const std::vector<double> Cm = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    ASSERT_EQ(input.moment_coefficient.get().size(), 10);
    for (size_t i=0 ; i<10 ; i++)
    {
        ASSERT_DOUBLE_EQ(input.moment_coefficient.get().at(i), Cm.at(i));
    }
}

BodyStates get_states(const double u = 0., const double v = 0.)
{
    BodyStates states(0);
    states.convention = YamlRotation("angle", {"z","y'","x''"});
    states.x.record(0, 0);
    states.y.record(0, 0);
    states.z.record(0, 0);
    states.u.record(0, u);
    states.v.record(0, v);
    states.w.record(0, 0);
    states.p.record(0, 0);
    states.q.record(0, 0);
    states.r.record(0, 0);
    states.qr.record(0, 1);
    states.qi.record(0, 0);
    states.qj.record(0, 0);
    states.qr.record(0, 0);
    return states;
}

TEST_F(HydroPolarForceModelTest, orientation_test)
{
    HydroPolarForceModel::Input input;
    input.name = "test";
    // Internal frame is placed under water, 5m under the body frame's origin
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,5), YamlAngle(0,0,0), "body");
    input.reference_area = 100;
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,0.};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    input.use_waves_velocity = false;
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    const HydroPolarForceModel force_model(input, "body", env);
    BodyStates states;
    // u=10, v=0 -> AoA=0°
    states = get_states(10, 0);
    auto F = force_model.get_force(states, 0, env, {});
    ASSERT_TRUE(F.X() < 0);
    ASSERT_NEAR(F.Y(), 0, 1e-9);
    // u=0, v=10 -> AoA=-90°
    states = get_states(0, 10);
    F = force_model.get_force(states, 0, env, {});
    ASSERT_TRUE(F.X() > 0);
    ASSERT_TRUE(F.Y() < 0);
    // u=0, v=-10 -> AoA=90°
    states = get_states(0, -10);
    F = force_model.get_force(states, 0, env, {});
    ASSERT_TRUE(F.X() > 0);
    ASSERT_TRUE(F.Y() > 0);
    // u=-10, v=0 -> AoA=+/-180°
    states = get_states(-10, 0);
    F = force_model.get_force(states, 0, env, {});
    ASSERT_TRUE(F.X() > 0);
    ASSERT_NEAR(F.Y(), 0, 1e-9);
}

TEST_F(HydroPolarForceModelTest, should_throw_for_invalid_polar_input)
{
    HydroPolarForceModel::Input input;
    input.name = "test";
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,5), YamlAngle(0,0,0), "body");
    input.reference_area = 1000;
    input.use_waves_velocity = false;
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});

    // The vectors do not have the same size
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888};
    ASSERT_THROW(HydroPolarForceModel(input, "body", env), InvalidInputException);

    // The maximum value for AoA is below 180°
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578};
    ASSERT_THROW(HydroPolarForceModel(input, "body", env), InvalidInputException);
}

TEST_F(HydroPolarForceModelTest, should_print_warning_for_polar_input_with_unexpected_bounds)
{
    HydroPolarForceModel::Input input;
    input.name = "test";
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,5), YamlAngle(0,0,0), "body");
    input.reference_area = 1000;
    input.use_waves_velocity = false;
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    std::cout << "a" << std::endl;

    std::stringstream debug;
    std::cout << "b" << std::endl;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig = std::cerr.rdbuf(debug.rdbuf());
    std::cout << "c" << std::endl;
    ASSERT_TRUE(debug.str().empty());
    std::cout << "d" << std::endl;

    // The maximum value for AoA is over 180°
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI,3*M_PI/2};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483,1.34483};
    HydroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());

    // Reset the stderr stringstream
    debug.str("");
    std::cout << "e" << std::endl;
    debug.clear();
    std::cout << "f" << std::endl;
    ASSERT_TRUE(debug.str().empty());
    std::cout << "g" << std::endl;

    // The minimum value for AoA is between -180° and 0°
    input.angle_of_attack = {-M_PI/2,0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388, M_PI};
    input.lift_coefficient = {0.00000,0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    HydroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());
    std::cout << "h" << std::endl;

    // Reset the stderr stringstream
    debug.str("");
    std::cout << "i" << std::endl;
    debug.clear();
    std::cout << "j" << std::endl;
    ASSERT_TRUE(debug.str().empty());
    std::cout << "k" << std::endl;

    // The minimum value for AWA is under -180°
    input.angle_of_attack = {-3*M_PI/2,-M_PI/2,0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388, M_PI};
    input.lift_coefficient = {0.00000,0.00000,0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.03448,0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    HydroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());
    std::cout << "l" << std::endl;

    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
    std::cout << "m" << std::endl;
}
