//const auto directional_spectra = env.w->get_and_check_wave_height
//
//.environment
//const auto yaml = SimulatorYamlParser(test_data::full_example()).parse();
//EnvironmentAndFrames SimulatorBuilder::build_environment_and_frames() const
//YamlSimulatorInput
//SimulatorBuilder(const YamlSimulatorInput& input, const double t0, const ssc::data_source::DataSource& command_listener = ssc::data_source::DataSource());

#include "EnvironmentTest.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SimulatorYamlParser.hpp"
#include "InvalidInputException.hpp"
#include "stl_data.hpp"
#include "yaml_data.hpp"
#include "simulator_api.hpp"

#include <fstream>
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

EnvironmentTest::EnvironmentTest(){}

TEST_F(EnvironmentTest, can_get_wave_elevation_with_test_data_full_example)
{
    const std::string stl_filename("test_ship.stl");
    std::ofstream stl(stl_filename);
    stl << test_data::cube();
    stl.close();
    Sim sim = get_system(test_data::full_example());
    auto waves = sim.get_env().w;
    std::vector<double> res = waves->get_and_check_wave_height(std::vector<double>{0.0}, std::vector<double>{0.0}, 0);
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0], 12);
    EXPECT_EQ(0, remove(stl_filename.c_str()));
}

TEST_F(EnvironmentTest, can_get_wave_elevation_with_simple_waves)
{
    Sim sim = get_system(test_data::simple_waves());
    auto waves = sim.get_env().w;
    std::vector<double> res = waves->get_and_check_wave_height(std::vector<double>{0.0}, std::vector<double>{0.0}, 0);
    ASSERT_EQ(res.size(), 1);
    ASSERT_NE(res[0], 0.0);
}

TEST_F(EnvironmentTest, can_get_wave_elevation_with_waves_from_a_list_of_rays)
{
    Sim sim = get_system(test_data::waves_from_a_list_of_rays());
    auto waves = sim.get_env().w;
    std::vector<double> res = waves->get_and_check_wave_height(std::vector<double>{0.0}, std::vector<double>{0.0}, 0);
    ASSERT_EQ(res.size(), 1);
}

TEST_F(EnvironmentTest, check_that_system_can_be_generated_without_any_body)
{
    StateType ret(0,0);
    ASSERT_EQ(ret.size(), 0);
}
