#include <cmath>

#include "AeroPolarForceModelTest.hpp"
#include "AeroPolarForceModel.hpp"
#include "xdyn/environment_models/UniformWindVelocityProfile.hpp"

AeroPolarForceModelTest::AeroPolarForceModelTest()
{
}

TEST_F(AeroPolarForceModelTest, can_parse)
{
    std::stringstream ss;
    ss << "name: test\n" <<
          "calculation point in body frame:\n" <<
          "    x: {value: 1, unit: m}\n" <<
          "    y: {value: 2, unit: m}\n" <<
          "    z: {value: 3, unit: m}\n" <<
          "reference area: {value: 1000, unit: m^2}\n" <<
          "AWA: {unit: deg, values: [0,7,9,12,28,60,90,120,150,180]}\n" <<
          "lift coefficient: [0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207]\n" <<
          "drag coefficient: [0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483]\n" <<
          "angle command: beta";
    const AeroPolarForceModel::Input input = AeroPolarForceModel::parse(ss.str());
    ASSERT_EQ(input.name, "test");
    ASSERT_DOUBLE_EQ(input.reference_area, 1000);
    ASSERT_DOUBLE_EQ(input.calculation_point_in_body_frame.x, 1);
    ASSERT_DOUBLE_EQ(input.calculation_point_in_body_frame.y, 2);
    ASSERT_DOUBLE_EQ(input.calculation_point_in_body_frame.z, 3);
    const std::vector<double> AWA = {0,7,9,12,28,60,90,120,150,180};
    const std::vector<double> Cl = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    const std::vector<double> Cd = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    ASSERT_EQ(input.apparent_wind_angle.size(), 10);
    ASSERT_EQ(input.lift_coefficient.size(), 10);
    ASSERT_EQ(input.drag_coefficient.size(), 10);
    for (size_t i=0 ; i<10 ; i++)
    {
        ASSERT_DOUBLE_EQ(input.apparent_wind_angle.at(i), M_PI*AWA.at(i)/180);
        ASSERT_DOUBLE_EQ(input.lift_coefficient.at(i), Cl.at(i));
        ASSERT_DOUBLE_EQ(input.drag_coefficient.at(i), Cd.at(i));
    }
    ASSERT_TRUE(input.angle_command);
    ASSERT_EQ(input.angle_command.get(), "beta");
}

namespace {
BodyStates get_states(const double forward_speed = 0.)
{
    BodyStates states(0);
    states.x.record(0, 0);
    states.y.record(0, 0);
    states.z.record(0, 0);
    states.u.record(0, forward_speed);
    states.v.record(0, 0);
    states.w.record(0, 0);
    states.p.record(0, 0);
    states.q.record(0, 0);
    states.r.record(0, 0);
    states.qr.record(0, 1);
    states.qi.record(0, 0);
    states.qj.record(0, 0);
    states.qk.record(0, 0);
    return states;
}
}

TEST_F(AeroPolarForceModelTest, several_values)
{
    AeroPolarForceModel::Input input;
    input.name = "test";
    input.calculation_point_in_body_frame = YamlCoordinates(0,0,0);
    input.reference_area = 1000;
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    EnvironmentAndFrames env;
    const AeroPolarForceModel force_model(input, "body", env);
    const BodyStates states = get_states();
    env.set_rho_air(1.2);
    UniformWindVelocityProfile::Input wind_input;
    wind_input.velocity = 10;

    // WARNING: The lift force is not continuous at AWA=180° because it changes side (gybing),
    // thus it is not a very good test point (side of lift might depend on implementation)

    wind_input.direction = 0*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    const double eps = 1E-10;
    ASSERT_NEAR(80689.800000000003, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(-6724.2000000000098, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 45*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(78937.416185313908, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(21889.902640217559, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 90*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(76034.400072671793, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(22949.999826340063, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 135*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(58259.36619649193, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(63226.236365071731, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 180*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(-2068.8000000000002, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(0, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 225*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(58259.366196491937, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(-63226.236365071782, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 270*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(76034.4000726718080, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(-22949.999826340088, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 315*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(78937.416185313894, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(-21889.902640217573, force_model.get_force(states, 0, env, {}).Y(), eps);
    wind_input.direction = 355*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(80930.439263099062, force_model.get_force(states, 0, env, {}).X(), eps);
    ASSERT_NEAR(-9736.6379043679153, force_model.get_force(states, 0, env, {}).Y(), eps);
}


TEST_F(AeroPolarForceModelTest, orientation_test_no_forward_speed)
{
    AeroPolarForceModel::Input input;
    input.name = "test";
    input.calculation_point_in_body_frame = YamlCoordinates(0,0,0);
    input.reference_area = 1000;
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    EnvironmentAndFrames env;
    const AeroPolarForceModel force_model(input, "body", env);
    const BodyStates states = get_states();
    env.set_rho_air(1.2);
    UniformWindVelocityProfile::Input wind_input;
    wind_input.velocity = 10;
    // Wind coming from North propagating to South (180°), heading North, Vs=0 --> AWA = 0°
    wind_input.direction = M_PI*180/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    auto F = force_model.get_force(states, 0, env, {});
    ASSERT_LT(F.X(), 0);
    ASSERT_NEAR(F.Y(), 0, 1e-10);
    // Wind coming from South propagating to North (0°), heading North, Vs=0 --> AWA = 180°
    wind_input.direction = 0;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    // Wind coming from East propagating to West (270°), heading North, Vs=0 --> AWA = 90°
    wind_input.direction = M_PI*270/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    ASSERT_LT(F.Y(), 0);
    // Wind coming from West propagating to East (90°), heading North, Vs=0 --> AWA = 270°
    wind_input.direction = M_PI*90/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    ASSERT_GT(F.Y(), 0);
}

TEST_F(AeroPolarForceModelTest, orientation_test_with_forward_speed)
{
    AeroPolarForceModel::Input input;
    input.name = "test";
    input.calculation_point_in_body_frame = YamlCoordinates(0,0,0);
    input.reference_area = 1000;
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    EnvironmentAndFrames env;
    const AeroPolarForceModel force_model(input, "body", env);
    const BodyStates states = get_states();
    env.set_rho_air(1.2);
    UniformWindVelocityProfile::Input wind_input;
    wind_input.velocity = 10;
    // North wind (180°), heading North, Vs=10m/s
    wind_input.direction = M_PI*180/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    auto F = force_model.get_force(states, 0, env, {});
    ASSERT_LT(F.X(), 0);
    ASSERT_NEAR(F.Y(), 0, 1e-10);
    // South wind (0°), heading North, Vs=10m/s
    wind_input.direction = 0;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    // North-East wind (225°), heading North, Vs=10m/s
    wind_input.direction = M_PI*225/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    ASSERT_LT(F.Y(), 0);
    // North-West wind (135°), heading North, Vs=10m/s
    wind_input.direction = M_PI*135/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    F = force_model.get_force(states, 0, env, {});
    ASSERT_GT(F.X(), 0);
    ASSERT_GT(F.Y(), 0);
}

TEST_F(AeroPolarForceModelTest, should_throw_for_invalid_polar_input)
{
    AeroPolarForceModel::Input input;
    input.name = "test";
    input.calculation_point_in_body_frame = YamlCoordinates(0,0,0);
    input.reference_area = 1000;
    EnvironmentAndFrames env;

    // The vectors do not have the same size
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888};
    ASSERT_THROW(AeroPolarForceModel(input, "body", env), InvalidInputException);

    // The maximum value for AWA is below 180°
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578};
    ASSERT_THROW(AeroPolarForceModel(input, "body", env), InvalidInputException);
}

TEST_F(AeroPolarForceModelTest, should_print_warning_for_polar_input_with_unexpected_bounds)
{
    AeroPolarForceModel::Input input;
    input.name = "test";
    input.calculation_point_in_body_frame = YamlCoordinates(0,0,0);
    input.reference_area = 1000;
    EnvironmentAndFrames env;

    std::stringstream debug;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig = std::cerr.rdbuf(debug.rdbuf());
    ASSERT_TRUE(debug.str().empty());

    // The maximum value for AWA is between 180° and 360°
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI,3*M_PI/2};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483,1.34483};
    AeroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());

    // Reset the stderr stringstream
    debug.str("");
    debug.clear();
    ASSERT_TRUE(debug.str().empty());

    // The maximum value for AWA is over 360°
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI,3*M_PI/2,5*M_PI/2};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207,-0.11207,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483,1.34483,1.34483};
    AeroPolarForceModel(input, "body", env);
    ASSERT_FALSE(debug.str().empty());

    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(AeroPolarForceModelTest, angle_can_be_controlled)
{
    AeroPolarForceModel::Input input;
    input.name = "test";
    input.calculation_point_in_body_frame = YamlCoordinates(0,0,0);
    input.reference_area = 1000;
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    input.angle_command = "beta";
    EnvironmentAndFrames env;
    const AeroPolarForceModel force_model(input, "body", env);
    const BodyStates states = get_states();
    env.set_rho_air(1.2);
    UniformWindVelocityProfile::Input wind_input;
    wind_input.velocity = 10;

    wind_input.direction = 0;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    const double eps = 1E-10;
    ASSERT_NEAR(22949.999826340067, force_model.get_force(states, 0, env, {{"beta", 90*M_PI/180}}).X(), eps);
    ASSERT_NEAR(76034.400072671793, force_model.get_force(states, 0, env, {{"beta", 90*M_PI/180}}).Y(), eps);

    ASSERT_THROW(force_model.get_force(states, 0, env, {}), std::out_of_range);

    wind_input.direction = 90*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(-76034.400072671793, force_model.get_force(states, 0, env, {{"beta", 180*M_PI/180}}).X(), eps);
    ASSERT_NEAR(22949.999826340067, force_model.get_force(states, 0, env, {{"beta", 180*M_PI/180}}).Y(), eps);

    wind_input.direction = -90*M_PI/180;
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    ASSERT_NEAR(76034.400072671793, force_model.get_force(states, 0, env, {{"beta", 0}}).X(), eps);
    ASSERT_NEAR(-22949.999826340067, force_model.get_force(states, 0, env, {{"beta", 0}}).Y(), eps);
}

TEST_F(AeroPolarForceModelTest, symmetrical_behavior)
{
    AeroPolarForceModel::Input input;
    input.name = "test";
    input.calculation_point_in_body_frame = YamlCoordinates(0,0,0);
    input.reference_area = 1000;
    input.apparent_wind_angle = {0.,0.12217305,0.15707963,0.20943951,0.48869219,1.04719755,1.57079633,2.0943951,2.61799388,M_PI};
    input.lift_coefficient = {0.00000,0.94828,1.13793,1.25000,1.42681,1.38319,1.26724,0.93103,0.38793,-0.11207};
    input.drag_coefficient = {0.03448,0.01724,0.01466,0.01466,0.02586,0.11302,0.38250,0.96888,1.31578,1.34483};
    input.angle_command = "beta";
    EnvironmentAndFrames env;
    const AeroPolarForceModel force_model(input, "body", env);
    const BodyStates states = get_states();
    env.set_rho_air(1.2);
    UniformWindVelocityProfile::Input wind_input;
    wind_input.velocity = 10;
    wind_input.direction = M_PI; // North wind
    env.wind.reset(new UniformWindVelocityProfile(wind_input));
    const double eps = 1E-10;

    std::vector<double> wind_directions = {5,30,60,90,120,150,175};
    for(double& wind_direction: wind_directions)
    {
        Wrench F = force_model.get_force(states, 0, env, {{"beta", wind_direction*M_PI/180}});
        Wrench F_sym = force_model.get_force(states, 0, env, {{"beta", -wind_direction*M_PI/180}});
        ASSERT_NEAR(F.X(), F_sym.X(), eps);
        ASSERT_NEAR(F.Y(), -F_sym.Y(), eps);
    }
}
