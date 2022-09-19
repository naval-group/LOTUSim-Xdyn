/*
 * EverythingObserverTest.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: cady
 */

#include "EverythingObserverTest.hpp"
#include "EverythingObserver.hpp"
#include "ListOfObservers.hpp"
#include "xdyn/observers_and_api/simulator_api.hpp"
#include "xdyn/test_data_generator/stl_data.hpp"
#include "xdyn/test_data_generator/yaml_data.hpp"
#include "xdyn/yaml_parser/parse_output.hpp"
#include <ssc/solver/steppers.hpp>

#define EPS 1E-8
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

EverythingObserverTest::EverythingObserverTest() : a(ssc::random_data_generator::DataGenerator(8725200))
{
}

EverythingObserverTest::~EverythingObserverTest()
{
}

void EverythingObserverTest::SetUp()
{
}

void EverythingObserverTest::TearDown()
{
}

ListOfObservers observers();
ListOfObservers observers()
{
    return ListOfObservers({ObserverPtr(new EverythingObserver())});
}

std::vector<Res> get_results(const ListOfObservers& observers);
std::vector<Res> get_results(const ListOfObservers& observers)
{
    if (observers.empty())
        return std::vector<Res>();
    const auto observer = static_cast<EverythingObserver*>(observers.get().front().get());
    return observer->get();
}

TEST_F(EverythingObserverTest, example)
{
//! [MapObserverTest example]
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::GM_cube(), test_data::cube(), 0);
    auto list_of_observers = observers();

    ssc::solver::Scheduler scheduler(0, tend, dt);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, list_of_observers);
    const auto obs = list_of_observers.get();
//! [MapObserverTest example]
//! [MapObserverTest expected output]
    ASSERT_EQ(1, obs.size());
    const auto results = get_results(list_of_observers);
    ASSERT_EQ(2, results.size());
    ASSERT_TRUE(results.front().extra_observations.find("GM(cube)") != results.front().extra_observations.end());
//! [MapObserverTest expected output]
}

TEST_F(EverythingObserverTest, GM)
{
    const double max_relative_error = 1E-2;
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::GM_cube(), test_data::cube_for_gm_test(), 0);
    ssc::solver::Scheduler scheduler(0, tend, dt);
    auto list_of_observers = observers();
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, list_of_observers);
    const auto obs = list_of_observers.get();
//! [MapObserverTest example]
//! [MapObserverTest expected output]
    ASSERT_EQ(1, obs.size());
    const auto results = get_results(list_of_observers);
    ASSERT_EQ(2, results.size());
    ASSERT_TRUE(results.back().extra_observations.find("Fz(GM,cube,NED)") != results.back().extra_observations.end());
    ASSERT_TRUE(results.back().extra_observations.find("GM(cube)") != results.back().extra_observations.end());
    const double expected_fz = -1000*9.81*0.5;
    ASSERT_NEAR(expected_fz, results.back().extra_observations.at("Fz(GM,cube,NED)"), std::abs(max_relative_error*expected_fz));
    /*
    Cf. discussion here: https://gitlab.sirehna.com/sirehna/xdyn/-/merge_requests/166#note_115660

    Cube with 1 m edges, half-immersed:

- G is the centre of gravity,
- M the metacentre,
- B the centre of buoyancy,
- V the immersed volume,
- I The cube's inertia (along the non-vertical x axis, perpendicular to one of the cube's faces)

```math
\overline{GM} = \overline{BM} - \overline{BG}
   = I/V - 0.25 = \frac{ab^3}{12\cdot V} - 0.25 = \frac{1}{6}-\frac{1}{4} = -\frac{1}{12}
```
   So GM = -1/12
    */
    const double expected_gm = -1.0/12.0;
    ASSERT_NEAR(expected_gm, results.back().extra_observations.at("GM(cube)"), std::abs(max_relative_error*expected_gm));
}
