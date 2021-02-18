#include "yaml_data.hpp"
#include <ssc/macros.hpp>

#include "TriMeshTestData.hpp"
#include "XdynForCS.hpp"
#include "XdynForCSTest.hpp"
#include "JSONSerializer.hpp"
#define EPS 1E-8

XdynForCSTest::XdynForCSTest() : a(ssc::random_data_generator::DataGenerator(123456789))
{
}

XdynForCSTest::~XdynForCSTest()
{
}

void XdynForCSTest::SetUp()
{
}

void XdynForCSTest::TearDown()
{
}

TEST_F(XdynForCSTest, test_falling_ball_with_yaml)
{
//! [SimServerTest example]

    const double g = 9.81;
    const double dt = 1.0;
    const double t_start = 1.87;
    const double Dt = 10;
    const double t_end = t_start+Dt;
    const std::string yaml_model = test_data::falling_ball_example();
    const std::string solver = "euler";
    XdynForCS sim_server(yaml_model, solver, dt);
    const auto JSON_request = test_data::complete_yaml_message_for_falling_ball();
    const std::vector<YamlState> outputs = sim_server.handle(deserialize(JSON_request));

//! [SimServerTest example]
//! [SimServerTest expected output]
    ASSERT_EQ(11, outputs.size());
    ASSERT_NEAR(t_end, outputs.back().t, EPS);
    ASSERT_NEAR(4.0+1.0*10.0,              outputs.back().x,  EPS);
    ASSERT_NEAR(8.0,                       outputs.back().y,  EPS);
    ASSERT_NEAR(12.0+g*Dt*(Dt-1.)/2.,      outputs.back().z,  EPS);
    ASSERT_NEAR(1.0,                       outputs.back().u,  EPS);
    ASSERT_NEAR(0.0,                       outputs.back().v,  EPS);
    ASSERT_NEAR(0.0+g*Dt,                  outputs.back().w,  EPS);
    ASSERT_NEAR(0,                         outputs.back().p,  EPS);
    ASSERT_NEAR(0,                         outputs.back().q,  EPS);
    ASSERT_NEAR(0,                         outputs.back().r,  EPS);
    ASSERT_NEAR(1,                         outputs.back().qr, EPS);
    ASSERT_NEAR(0,                         outputs.back().qi, EPS);
    ASSERT_NEAR(0,                         outputs.back().qj, EPS);
    ASSERT_NEAR(0,                         outputs.back().qk, EPS);
//! [SimServerTest expected output]
}

TEST_F(XdynForCSTest, should_throw_if_Dt_is_not_strictly_positive)
{
    const double dt = 1.0;
    const std::string yaml_model = test_data::falling_ball_example();
    const std::string solver = "euler";
    XdynForCS sim_server(yaml_model, solver, dt);
    auto JSON_request = test_data::invalid_json_for_cs();
    ASSERT_THROW(sim_server.handle(deserialize(JSON_request)), InvalidInputException);
}

TEST_F(XdynForCSTest, can_get_extra_observations)
{
    const double dt = 1.0;
    const std::string yaml_model = test_data::GM_cube();
    const std::string solver = "euler";
    XdynForCS sim_server(yaml_model, unit_cube(), solver, dt);
    const auto JSON_request = test_data::JSON_server_request_GM_cube_with_output();
    const std::vector<YamlState> outputs = sim_server.handle(deserialize(JSON_request));
    ASSERT_EQ(11, outputs.size());
    ASSERT_FALSE(outputs.front().extra_observations. empty());
    for (const auto output:outputs)
    {
        ASSERT_NE(output.extra_observations.find("GM(cube)"), output.extra_observations.end());
        ASSERT_NE(output.extra_observations.find("GZ(cube)"), output.extra_observations.end());
    }
}

TEST_F(XdynForCSTest, can_compute_one_step_with_euler_solver)
{
//! [SimStepperTest can_compute_one_step_with_euler_solver]
    const double g = 9.81;
    const std::string solver = "euler";
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;
    const double t_end = t_start+Dt;

    const std::string input_yaml(test_data::falling_ball_example());
    XdynForCS simstepper(input_yaml, solver, dt); // SimStepper's creator controls the stability of the numerical integration
    const double x0 = 4;
    const double y0 = 8;
    const double z0 = 12;
    const double u0 = 1;
    const double v0 = 0;
    const double w0 = 0;

    YamlSimServerInputs y;
    y.Dt = Dt;
    y.states = std::vector<YamlState>(1, YamlState(t_start, x0, y0 ,z0 ,u0 ,v0 ,w0 ,0 ,0 ,0 ,1 ,0 ,0 ,0));

    SimServerInputs infos(y, Dt);

    const std::vector<YamlState> res = simstepper.play_one_step(infos);
//! [SimStepperTest example]

//! [SimStepperTest expected output]
    ASSERT_EQ(11, res.size());
    ASSERT_NEAR(t_end,                    res.back().t, EPS);
    ASSERT_NEAR(x0+u0*t_end,              res.back().x, EPS);
    ASSERT_NEAR(y0,                       res.back().y, EPS);
    ASSERT_NEAR(z0+g*t_end*(t_end-1.)/2., res.back().z, EPS);
    ASSERT_NEAR(u0,                       res.back().u, EPS);
    ASSERT_NEAR(v0,                       res.back().v, EPS);
    ASSERT_NEAR(w0+g*t_end,               res.back().w, EPS);
    ASSERT_NEAR(0,                        res.back().p, EPS);
    ASSERT_NEAR(0,                        res.back().q, EPS);
    ASSERT_NEAR(0,                        res.back().r, EPS);
    ASSERT_NEAR(1,                        res.back().qr, EPS);
    ASSERT_NEAR(0,                        res.back().qi, EPS);
    ASSERT_NEAR(0,                        res.back().qj, EPS);
    ASSERT_NEAR(0,                        res.back().qk, EPS);
//! [SimStepperTest expected output]
}

TEST_F(XdynForCSTest, can_compute_same_step_several_times)
{
//! [SimStepperTest can_compute_one_step_with_euler_solver]
    const double g = 9.81;
    const std::string solver = "euler";
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;
    const double t_end = t_start+Dt;

    std::string input_yaml(test_data::falling_ball_example());
    XdynForCS simstepper(input_yaml, solver, dt); // SimStepper's creator controls the stability of the numerical integration
    const double x0 = 4;
    const double y0 = 8;
    const double z0 = 12;
    const double u0 = 1;
    const double v0 = 0;
    const double w0 = 0;

    YamlSimServerInputs y;
    y.Dt = Dt;
    y.states = std::vector<YamlState>(1, YamlState(t_start, x0, y0 ,z0 ,u0 ,v0 ,w0 ,0 ,0 ,0 ,1 ,0 ,0 ,0));
    const SimServerInputs infos(y, Dt);
    std::vector<YamlState> res = simstepper.play_one_step(infos);
    res = simstepper.play_one_step(infos);
//! [SimStepperTest can_compute_one_step_with_euler_solver]

//! [SimStepperTest can_compute_one_step_with_euler_solver output]
    ASSERT_EQ(11, res.size());
    ASSERT_NEAR(t_end,                    res.back().t, EPS);
    ASSERT_NEAR(x0+u0*t_end,              res.back().x, EPS);
    ASSERT_NEAR(y0,                       res.back().y, EPS);
    ASSERT_NEAR(z0+g*t_end*(t_end-1.)/2., res.back().z, EPS);
    ASSERT_NEAR(u0,                       res.back().u, EPS);
    ASSERT_NEAR(v0,                       res.back().v, EPS);
    ASSERT_NEAR(w0+g*t_end,               res.back().w, EPS);
    ASSERT_NEAR(0,                        res.back().p, EPS);
    ASSERT_NEAR(0,                        res.back().q, EPS);
    ASSERT_NEAR(0,                        res.back().r, EPS);
    ASSERT_NEAR(1,                        res.back().qr, EPS);
    ASSERT_NEAR(0,                        res.back().qi, EPS);
    ASSERT_NEAR(0,                        res.back().qj, EPS);
    ASSERT_NEAR(0,                        res.back().qk, EPS);

//! [SimStepperTest expected output]

}
TEST_F(XdynForCSTest, wrong_solver_must_raise_exception)
{
//! [SimStepperTest wrong_solver_must_raise_exception]
    const std::string solver = "wrong solver";
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;

    std::string input_yaml(test_data::falling_ball_example());
    XdynForCS simstepper(input_yaml, solver, dt); // SimStepper's creator controls the stability of the numerical integration
    const double x0 = 4;
    const double y0 = 8;
    const double z0 = 12;
    const double u0 = 1;
    const double v0 = 0;
    const double w0 = 0;

    YamlSimServerInputs y;
    y.Dt = Dt;
    y.states = std::vector<YamlState>(1, YamlState(t_start, x0, y0 ,z0 ,u0 ,v0 ,w0 ,0 ,0 ,0 ,1 ,0 ,0 ,0));

//! [SimStepperTest wrong_solver_must_raise_exception]

//! [SimStepperTest wrong_solver_must_raise_exception output]
    ASSERT_THROW(simstepper.play_one_step(SimServerInputs(y, Dt)), InvalidInputException);
//! [SimStepperTest expected output]
}

TEST_F(XdynForCSTest, sim_update)
{
    ConfBuilder builder(test_data::falling_ball_example());
    Sim sim(builder.sim);
    const StateType x1 = {100.0, 200.0 ,300.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0};
    const StateType x2 = {400.0, 500.0 ,600.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0};

    sim.set_bodystates(State(AbstractStates<double>(4.0, 8.0 ,12.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0) ,0.0));
    sim.get_bodies().front()->update_body_states(x1, 10.0);
    sim.set_bodystates(State(AbstractStates<double>(4.0, 8.0 ,12.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0) ,0.0));
    sim.get_bodies().front()->update_body_states(x2, 10.0);
    sim.set_bodystates(State(AbstractStates<double>(5.0, 200.0 ,300.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0) ,0.0));
    ASSERT_EQ(sim.get_bodies().front()->get_states().x(), 5.0);
}
