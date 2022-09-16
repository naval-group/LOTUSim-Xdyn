/*
 * MapObserverTest.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: cady
 */


#include "MapObserver.hpp"
#include "MapObserverTest.hpp"
#include "test_data_generator/yaml_data.hpp"
#include "parse_output.hpp"
#include "ListOfObservers.hpp"
#include "observers_and_api/simulator_api.hpp"
#include <ssc/solver/steppers.hpp>
#include "test_data_generator/stl_data.hpp"

#define EPS 1E-8
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

MapObserverTest::MapObserverTest() : a(ssc::random_data_generator::DataGenerator(8725200))
{
}

MapObserverTest::~MapObserverTest()
{
}

void MapObserverTest::SetUp()
{
}

void MapObserverTest::TearDown()
{
}

ListOfObservers observe(const std::vector<std::string>& stuff_to_watch);
ListOfObservers observe(const std::vector<std::string>& stuff_to_watch)
{
    YamlOutput out;
    out.data = stuff_to_watch;
    out.format = "map";
    return ListOfObservers({out});
}

std::map<std::string,std::vector<double> > get_map(const ListOfObservers& observers)
{
    if (observers.empty())
        return std::map<std::string,std::vector<double> >();
    const auto map_observer = static_cast<MapObserver*>(observers.get().front().get());
    return map_observer->get();
}

TEST_F(MapObserverTest, example)
{
//! [MapObserverTest example]
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::falling_ball_example(), 0);

    ssc::solver::Scheduler scheduler(0, tend, dt);
    auto observers = observe({"x(ball)"});
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observers);
    const auto obs = observers.get();
//! [MapObserverTest example]
//! [MapObserverTest expected output]
    ASSERT_EQ(1, obs.size());
    const auto m = get_map(observers);
    ASSERT_EQ(1, m.size());
    ASSERT_TRUE(m.find("x(ball)") != m.end());
//! [MapObserverTest expected output]
}

TEST_F(MapObserverTest, GM)
{
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::GM_cube(), test_data::cube_for_gm_test(), 0);
    ssc::solver::Scheduler scheduler(0, tend, dt);
    auto observers = observe({"Fz(GM,cube,NED)","GM(cube)"});
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observers);
    const auto m = get_map(observers);
    ASSERT_TRUE(m.find("Fz(GM,cube,NED)") != m.end());
    ASSERT_TRUE(m.find("GM(cube)") != m.end());
    const double max_relative_error = 1E-2;
    const double expected_fz = -1000*9.81*0.5;
    ASSERT_NEAR(expected_fz, m.find("Fz(GM,cube,NED)")->second.back(), std::abs(max_relative_error*expected_fz));
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
    ASSERT_NEAR(expected_gm, m.find("GM(cube)")->second.back(), std::abs(max_relative_error*expected_gm));
}

TEST_F(MapObserverTest, blocked_state_force_residuals_should_appear)
{
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::full_example_with_diagonal_inertia(), test_data::cube(), 0);
    ssc::solver::Scheduler scheduler(0, tend, dt);
    auto observers = observe({"Fx(blocked states,body 1,body 1)","Fy(blocked states,body 1,body 1)","Fz(blocked states,body 1,body 1)","Mx(blocked states,body 1,body 1)","My(blocked states,body 1,body 1)","Mz(blocked states,body 1,body 1)"});
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, scheduler, observers);
    auto m = get_map(observers);
    ASSERT_EQ(6, m.size());
    ASSERT_TRUE(m.find("Fx(blocked states,body 1,body 1)") != m.end());
    ASSERT_TRUE(m.find("Fy(blocked states,body 1,body 1)") != m.end());
    ASSERT_TRUE(m.find("Fz(blocked states,body 1,body 1)") != m.end());
    ASSERT_TRUE(m.find("Mx(blocked states,body 1,body 1)") != m.end());
    ASSERT_TRUE(m.find("My(blocked states,body 1,body 1)") != m.end());
    ASSERT_TRUE(m.find("Mz(blocked states,body 1,body 1)") != m.end());
    ASSERT_SMALL_RELATIVE_ERROR(-1.3937392887475170E16, m["Fx(blocked states,body 1,body 1)"].back(), EPS);
    ASSERT_NEAR(0, m["Fy(blocked states,body 1,body 1)"].back(), 1E-6);
    ASSERT_NEAR(-8, m["Fz(blocked states,body 1,body 1)"].back(), 1E-6);
    ASSERT_NEAR(0, m["Mx(blocked states,body 1,body 1)"].back(), 1E-6);
    ASSERT_NEAR(0, m["My(blocked states,body 1,body 1)"].back(), 1E-6);
    ASSERT_NEAR(0, m["Mz(blocked states,body 1,body 1)"].back(), 1E-6);
}
