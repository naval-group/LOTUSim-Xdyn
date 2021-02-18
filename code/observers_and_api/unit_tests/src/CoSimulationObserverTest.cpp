#include "yaml_data.hpp"
#include "parse_output.hpp"
#include "simulator_api.hpp"

#include "CoSimulationObserver.hpp"
#include "CoSimulationObserverTest.hpp"

CoSimulationObserverTest::CoSimulationObserverTest() : a(ssc::random_data_generator::DataGenerator(7926843))
{
}

CoSimulationObserverTest::~CoSimulationObserverTest()
{
}

TEST_F(CoSimulationObserverTest, RequestedAdditionalVariablesAreAvailable)
{
    const double dt = 1;
    const double tend = 10;
    auto sys = get_system(test_data::falling_ball_example(), 0);

    CoSimulationObserver observer({"Fz(gravity,ball,ball)"}, sys.get_bodies().at(0)->get_name());
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
    auto results = observer.get();

    for(const auto& res:results)
    {
        ASSERT_TRUE(res.extra_observations.find("Fz(gravity,ball,ball)") != res.extra_observations.end());
    }

}
