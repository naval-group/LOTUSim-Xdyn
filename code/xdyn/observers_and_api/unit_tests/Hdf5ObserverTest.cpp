
#include "xdyn/test_data_generator/yaml_data.hpp"
#include "Hdf5Observer.hpp"
#include "Hdf5ObserverTest.hpp"
#include "ListOfObservers.hpp"
#include "xdyn/observers_and_api/simulator_api.hpp"
#include "xdyn/yaml_parser/parse_output.hpp"
#include <ssc/solver/steppers.hpp>

Hdf5ObserverTest::Hdf5ObserverTest() : a(ssc::random_data_generator::DataGenerator(546545))
{
}

TEST_F(Hdf5ObserverTest, should_be_able_to_create_an_observer)
{
    const double dt = 1E-1;
    const double tend = 10;
    auto sys = get_system(test_data::falling_ball_example(), 0);
    const auto yaml = parse_output(test_data::falling_ball_example());
    {
        ssc::solver::Scheduler scheduler(0, tend, dt);
        ListOfObservers observers(yaml);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observers);
    }
    for (auto output:yaml)
    {
        if (output.format == "hdf5")
        {
            EXPECT_EQ(0,remove(output.filename.c_str()));
        }
    }
}

TEST_F(Hdf5ObserverTest, LONG_should_be_able_to_create_an_observer_with_wave_output)
{
    const double dt = 1E-1;
    const double tend = 10;
    auto sys = get_system(test_data::simple_waves(), 0);
    const auto yaml = parse_output(test_data::simple_waves());
    {
        ssc::solver::Scheduler scheduler(0, tend, dt);
        ListOfObservers observers(yaml);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observers);
    }
    for (auto output:yaml)
    {
        if (output.format == "hdf5")
        {
            //std::cout<<output.filename;
            //EXPECT_EQ(0,remove(output.filename.c_str()));
        }
    }
}
