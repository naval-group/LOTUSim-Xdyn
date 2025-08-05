#include "HydroPolarForceModelTest.hpp"
#include "HydroPolarForceModel.hpp"
#include "xdyn/core/yaml2eigen.hpp"
#include <cmath>

HydroPolarForceModelTest::HydroPolarForceModelTest()
{
}

std::string get_yaml_input();
std::string get_yaml_input()
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
    return ss.str();
}

TEST_F(HydroPolarForceModelTest, can_parse)
{
    const HydroPolarForceModel::Input input = HydroPolarForceModel::parse(get_yaml_input());
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
    ASSERT_FALSE(input.chord_length.is_initialized());
}

TEST_F(HydroPolarForceModelTest, can_parse_optional_output)
{
    std::stringstream ss;
    ss << "\n" <<
          "moment coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]\n" <<
          "chord length: {value: 5, unit: m}\n" <<
          "angle command: beta";
    const HydroPolarForceModel::Input input = HydroPolarForceModel::parse(get_yaml_input()+ss.str());
    ASSERT_TRUE(input.moment_coefficient.is_initialized());
    ASSERT_TRUE(input.chord_length.is_initialized());
    const std::vector<double> Cm = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    ASSERT_EQ(input.moment_coefficient.get().size(), 10);
    for (size_t i=0 ; i<10 ; i++)
    {
        ASSERT_DOUBLE_EQ(input.moment_coefficient.get().at(i), Cm.at(i));
    }
    ASSERT_TRUE(input.angle_command);
    ASSERT_EQ(input.angle_command.get(), "beta");
}


namespace {
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
    ASSERT_LT(F.X(), 0);
    ASSERT_NEAR(F.Y(), 0, 1e-9);
    // u=0, v=10 -> AoA=-90°
    states = get_states(0, 10);
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    ASSERT_LT(F.Y(), 0);
    // u=0, v=-10 -> AoA=90°
    states = get_states(0, -10);
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    ASSERT_GT(F.Y(), 0);
    // u=-10, v=0 -> AoA=+/-180°
    states = get_states(-10, 0);
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
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

    std::stringstream debug;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig = std::cerr.rdbuf(debug.rdbuf());
    ASSERT_TRUE(debug.str().empty());

    // The maximum value for AoA is over 180°
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI,3*M_PI/2};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483,1.34483};
    HydroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());

    // Reset the stderr stringstream
    debug.str("");
    debug.clear();
    ASSERT_TRUE(debug.str().empty());

    // The minimum value for AoA is between -180° and 0°
    input.angle_of_attack = {-M_PI/2,0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388, M_PI};
    input.lift_coefficient = {0.00000,0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    HydroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());

    // Reset the stderr stringstream
    debug.str("");
    debug.clear();
    ASSERT_TRUE(debug.str().empty());

    // The minimum value for AWA is under -180°
    input.angle_of_attack = {-3*M_PI/2,-M_PI/2,0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388, M_PI};
    input.lift_coefficient = {0.00000,0.00000,0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.03448,0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    HydroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());

    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(HydroPolarForceModelTest, should_throw_if_empty_angle_vector)
{
    HydroPolarForceModel::Input input;
    input.name = "test";
    // Internal frame is placed under water, 5m under the body frame's origin
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,5), YamlAngle(0,0,0), "body");
    input.reference_area = 100;
    input.angle_of_attack = {};
    input.lift_coefficient = {};
    input.drag_coefficient = {};
    input.use_waves_velocity = true;
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    ASSERT_THROW(HydroPolarForceModel(input, "body", env), InvalidInputException);
}

TEST_F(HydroPolarForceModelTest, should_throw_if_wave_model_expected_but_not_defined)
{
    HydroPolarForceModel::Input input;
    input.name = "test";
    // Internal frame is placed under water, 5m under the body frame's origin
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,5), YamlAngle(0,0,0), "body");
    input.reference_area = 100;
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,0.};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    input.use_waves_velocity = true;
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    ASSERT_THROW(HydroPolarForceModel(input, "body", env), InvalidInputException);
}

TEST_F(HydroPolarForceModelTest, should_print_warning_and_return_zero_force_if_calculation_point_is_outside_the_water)
{
    HydroPolarForceModel::Input input;
    input.name = "test";
    // Internal frame is placed above water, 5m above the body frame's origin
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,-5), YamlAngle(0,0,0), "body");
    input.reference_area = 100;
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,0.};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    input.use_waves_velocity = false;
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    const HydroPolarForceModel force_model(input, "body", env);
    BodyStates states = get_states(10, 0);

    std::stringstream debug;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig = std::cerr.rdbuf(debug.rdbuf());
    ASSERT_TRUE(debug.str().empty());

    auto F = force_model.get_force(states, 0, env, {});
    ASSERT_FALSE(debug.str().empty());
    ASSERT_DOUBLE_EQ(F.X(), 0);
    ASSERT_DOUBLE_EQ(F.Y(), 0);
    ASSERT_DOUBLE_EQ(F.Z(), 0);
    ASSERT_DOUBLE_EQ(F.K(), 0);
    ASSERT_DOUBLE_EQ(F.M(), 0);
    ASSERT_DOUBLE_EQ(F.N(), 0);

    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(HydroPolarForceModelTest, symmetrical_behavior)
{
    HydroPolarForceModel::Input input;
    // Internal frame is placed under water, 5m under the body frame's origin
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,5), YamlAngle(0,0,0), "body");
    input.name = "test";
    input.reference_area = 100;
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,0.};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    input.use_waves_velocity = false;
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    BodyStates states = get_states(10, 0);
    HydroPolarForceModel force_model(input, "body", env);

    const double eps = 1E-8;

    std::vector<double> angles = {0,30,60,90,120,150,180};
    for(double angle: angles)
    {
        input.internal_frame.angle.psi = angle*M_PI/180;
        env.k->add(make_transform(input.internal_frame, input.name, env.rot));
        Wrench F = force_model.get_force(states, 0, env, {});
        input.internal_frame.angle.psi = -angle*M_PI/180;
        env.k->add(make_transform(input.internal_frame, input.name, env.rot));
        Wrench F_sym = force_model.get_force(states, 0, env, {});
        ASSERT_NEAR(F.X(), F_sym.X(), eps) << "angle: " << angle << "deg";
        ASSERT_NEAR(F.Y(), -F_sym.Y(), eps) << "angle: " << angle << "deg";;
    }
}

TEST_F(HydroPolarForceModelTest, angle_can_be_controlled)
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
    input.angle_command = "beta";
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    const HydroPolarForceModel force_model(input, "body", env);
    BodyStates states;
    const double eps = 1E-10;
    // u=10, v=0, beta=0 -> AoA=0°
    states = get_states(10, 0);
    auto F = force_model.get_force(states, 0, env, {{"beta", 0}});
    ASSERT_NEAR(F.X(), -172399.99999999997, eps);
    ASSERT_NEAR(F.Y(), 0., eps);
    // u=10, v=0, beta=30° -> AoA=30°
    states = get_states(10, 0);
    F = force_model.get_force(states, 0, env, {{"beta", M_PI/6}});
    ASSERT_NEAR(F.X(), -141862.59529221783, eps);
    ASSERT_NEAR(F.Y(), 7168844.2065911861, eps);
    // u=10, v=0, beta=45° -> AoA=45°
    states = get_states(10, 0);
    F = force_model.get_force(states, 0, env, {{"beta", M_PI/4}});
    ASSERT_NEAR(F.X(), -292675.63145633071, eps);
    ASSERT_NEAR(F.Y(), 7159116.1098209573, eps);
    // u=10, v=0, beta=60° -> AoA=60°
    states = get_states(10, 0);
    F = force_model.get_force(states, 0, env, {{"beta", M_PI/3}});
    ASSERT_NEAR(F.X(), -565100.00156966201, eps);
    ASSERT_NEAR(F.Y(), 6915949.99887837, eps);

    // u=0, v=10, beta=90  -> AoA=0°
    states = get_states(0, 10);
    F = force_model.get_force(states, 0, env, {{"beta", M_PI/2}});
    ASSERT_NEAR(F.X(), 0., eps);
    ASSERT_NEAR(F.Y(), -172399.99999999997, eps);
    // u=0, v=-10, beta=-90  -> AoA=0°
    states = get_states(0, -10);
    F = force_model.get_force(states, 0, env, {{"beta", -M_PI/2}});
    ASSERT_NEAR(F.X(), 0., eps);
    ASSERT_NEAR(F.Y(), 172399.99999999997, eps);
    // u=-10, v=0, beta=180  -> AoA=0°
    states = get_states(-10, 0);
    F = force_model.get_force(states, 0, env, {{"beta", M_PI}});
    ASSERT_NEAR(F.X(), 172399.99999999997, eps);
    ASSERT_NEAR(F.Y(), 0., eps);
}

TEST_F(HydroPolarForceModelTest, angle_control_is_equivalent_to_frame_rotation)
{
    HydroPolarForceModel::Input input;
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
    BodyStates states = get_states(10, 0);

    HydroPolarForceModel::Input variable_frame_input = input;
    variable_frame_input.name = "variable_frame";
    HydroPolarForceModel variable_frame_force_model(variable_frame_input, "body", env);

    HydroPolarForceModel::Input controlled_angle_input = input;
    controlled_angle_input.name = "controlled_angle";
    controlled_angle_input.angle_command = "beta";
    const HydroPolarForceModel controlled_angle_force_model(controlled_angle_input, "body", env);

    const double eps = 1E-8;

    std::vector<double> angles = {-180,-150,-120,-90,-60,-30,0,30,60,90,120,150,180};
    for(double angle: angles)
    {
        variable_frame_input.internal_frame.angle.psi = angle*M_PI/180;
        env.k->add(make_transform(variable_frame_input.internal_frame, variable_frame_input.name, env.rot));
        Wrench F_controlled_angle = controlled_angle_force_model.get_force(states, 0, env, {{"beta", angle*M_PI/180}});
        Wrench F_variable_frame = variable_frame_force_model.get_force(states, 0, env, {});
        F_controlled_angle.change_frame("body", env.k);
        F_variable_frame.change_frame("body", env.k);
        ASSERT_NEAR(F_controlled_angle.X(), F_variable_frame.X(), eps) << "angle: " << angle << "deg";
        ASSERT_NEAR(F_controlled_angle.Y(), F_variable_frame.Y(), eps) << "angle: " << angle << "deg";;
    }
}

TEST_F(HydroPolarForceModelTest, symmetrical_behavior_with_controlled_angle)
{
    HydroPolarForceModel::Input input;
    // Internal frame is placed under water, 5m under the body frame's origin
    input.internal_frame = YamlPosition(YamlCoordinates(0,0,5), YamlAngle(0,0,0), "body");
    input.name = "test";
    input.reference_area = 100;
    input.angle_of_attack = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,0.};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    input.use_waves_velocity = false;
    input.angle_command = "beta";
    EnvironmentAndFrames env;
    env.rho = 1000;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    BodyStates states = get_states(10, 0);
    HydroPolarForceModel force_model(input, "body", env);

    const double eps = 1E-8;

    std::vector<double> angles = {0,30,60,90,120,150,180};
    for(double angle: angles)
    {
        Wrench F = force_model.get_force(states, 0, env, {{"beta", angle*M_PI/180}});
        Wrench F_sym = force_model.get_force(states, 0, env, {{"beta", -angle*M_PI/180}});
        ASSERT_NEAR(F.X(), F_sym.X(), eps) << "angle: " << angle << "deg";
        ASSERT_NEAR(F.Y(), -F_sym.Y(), eps) << "angle: " << angle << "deg";;
    }
}
