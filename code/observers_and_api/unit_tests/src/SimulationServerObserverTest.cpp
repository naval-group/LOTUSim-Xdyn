#include "SimulationServerObserverTest.hpp"

#include "yaml_data.hpp"
#include "parse_output.hpp"
#include "simulator_api.hpp"

#include "SimulationServerObserver.hpp"

SimulationServerObserverTest::SimulationServerObserverTest() : a(ssc::random_data_generator::DataGenerator(7926843))
{
}

SimulationServerObserverTest::~SimulationServerObserverTest()
{
}

TEST_F(SimulationServerObserverTest, requested_additional_variables_are_available)
{
    const double dt = 1;
    const double tend = 10;
    auto sys = get_system(test_data::falling_ball_example(), 0);

    SimulationServerObserver observer({"Fz(gravity,ball,ball)"});
    ssc::solver::Scheduler scheduler(0, tend, dt);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observer);
    auto results = observer.get();

    for(const auto& res:results)
    {
        ASSERT_TRUE(res.extra_observations.find("Fz(gravity,ball,ball)") != res.extra_observations.end());
    }

}
