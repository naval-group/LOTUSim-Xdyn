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
#include "yaml_data.hpp"
#include "simulator_api.hpp"

EnvironmentTest::EnvironmentTest(){}

TEST_F(EnvironmentTest, can_get_wave_elevation_with_test_data_full_example)
{
    const double t0 = 0.0;
    Sim sim = get_system(test_data::full_example(), t0);
    auto waves = sim.get_env().w;
    std::vector<double> res = waves->get_and_check_wave_height(std::vector<double>{0.0}, std::vector<double>{0.0}, 0);
    ASSERT_EQ(res.size(), 1);
}
