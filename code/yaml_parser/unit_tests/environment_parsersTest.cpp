/*
 * environment_parsersTest.cpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#include "environment_parsersTest.hpp"
#include "yaml_parser/environment_parsers.hpp"
#include "test_data_generator/yaml_data.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"

#include <boost/algorithm/string.hpp> // replace in string
#include <boost/regex.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#define DEG (180./PI)

#include <fstream>
#include <sstream>

const YamlWaveModel environment_parsersTest::yaml = parse_waves(test_data::waves_for_parser_validation_only());

environment_parsersTest::environment_parsersTest() : a(ssc::random_data_generator::DataGenerator(1212))
{
}

TEST_F(environment_parsersTest, default_wave_model)
{
//! [environment_parsersTest example]
    const YamlDefaultWaveModel input = parse_default_wave_model("constant sea elevation in NED frame: {value: 1234, unit: m}\n"
                                                                "output:\n"
                                                                "    frame of reference: NED\n"
                                                                "    mesh:\n"
                                                                "       xmin: {value: -10, unit: m}\n"
                                                                "       xmax: {value: 1, unit: km}\n"
                                                                "       nx: 10\n"
                                                                "       ymin: {value: -20, unit: m}\n"
                                                                "       ymax: {value: 3, unit: km}\n"
                                                                "       ny: 20\n");
//! [environment_parsersTest example]
//! [environment_parsersTest expected output]
    ASSERT_DOUBLE_EQ(1234,input.zwave);
    ASSERT_EQ("NED", input.output.frame_of_reference);
    ASSERT_DOUBLE_EQ(-10, input.output.xmin);
    ASSERT_DOUBLE_EQ(1000, input.output.xmax);
    ASSERT_EQ(10, input.output.nx);
    ASSERT_DOUBLE_EQ(-20, input.output.ymin);
    ASSERT_DOUBLE_EQ(3000, input.output.ymax);
    ASSERT_EQ(20, input.output.ny);
//! [environment_parsersTest expected output]
}

TEST_F(environment_parsersTest, cannot_have_both_n_and_nfreq)
{
    const char* yaml = "n: 128\n"
                       "nfreq: 128\n"
                       "omega min: {value: 0.1, unit: rad/s}\n"
                       "omega max: {value: 6, unit: deg/s}\n"
                       "energy fraction: 0.123\n"
                       "equal energy bins: true\n";
    ASSERT_THROW(parse_discretization(yaml), InvalidInputException);
}

TEST_F(environment_parsersTest, cannot_have_both_n_and_ndir)
{
    const char* yaml = "n: 128\n"
                       "ndir: 128\n"
                       "omega min: {value: 0.1, unit: rad/s}\n"
                       "omega max: {value: 6, unit: deg/s}\n"
                       "energy fraction: 0.123\n"
                       "equal energy bins: true\n";
    ASSERT_THROW(parse_discretization(yaml), InvalidInputException);
}

TEST_F(environment_parsersTest, can_parse_wave_discretization)
{
    ASSERT_EQ(128,yaml.discretization.nfreq);
    ASSERT_EQ(128,yaml.discretization.ndir);
    ASSERT_DOUBLE_EQ(0.1,yaml.discretization.omega_min);
    ASSERT_DOUBLE_EQ(6./DEG,yaml.discretization.omega_max);
    ASSERT_DOUBLE_EQ(0.123,yaml.discretization.energy_fraction);
}

TEST_F(environment_parsersTest, can_parse_wave_spreading_and_spectral_data)
{
    ASSERT_EQ(2,yaml.spectra.size());

    ASSERT_EQ("dirac",yaml.spectra.at(0).directional_spreading_type);
    ASSERT_EQ("type: dirac\nwaves propagating to:\n  unit: deg\n  value: 90",yaml.spectra.at(0).directional_spreading_yaml);
    ASSERT_EQ("airy",yaml.spectra.at(0).model);
    ASSERT_EQ("depth:\n  unit: km\n  value: 1.7\ndirectional spreading:\n  type: dirac\n  waves propagating to:\n    unit: deg\n    value: 90\nmodel: airy\nseed of the random data generator: 0\nspectral density:\n  Hs:\n    unit: m\n    value: 5\n  Tp:\n    unit: s\n    value: 15\n  gamma: 1.2\n  type: jonswap\nstretching:\n  delta: 0.5\n  h:\n    unit: m\n    value: 100",yaml.spectra.at(0).model_yaml);
    ASSERT_EQ("jonswap",yaml.spectra.at(0).spectral_density_type);
    ASSERT_EQ("Hs:\n  unit: m\n  value: 5\nTp:\n  unit: s\n  value: 15\ngamma: 1.2\ntype: jonswap",yaml.spectra.at(0).spectral_density_yaml);

    ASSERT_EQ("cos2s",yaml.spectra.at(1).directional_spreading_type);
    ASSERT_EQ("s: 2\ntype: cos2s\nwaves propagating to:\n  unit: deg\n  value: 90",yaml.spectra.at(1).directional_spreading_yaml);
    ASSERT_EQ("airy",yaml.spectra.at(1).model);
    ASSERT_EQ("depth:\n  unit: m\n  value: 12\ndirectional spreading:\n  s: 2\n  type: cos2s\n  waves propagating to:\n    unit: deg\n    value: 90\nmodel: airy\nseed of the random data generator: 1872\nspectral density:\n  Hs:\n    unit: m\n    value: 5\n  Tp:\n    unit: s\n    value: 15\n  type: dirac\nstretching:\n  delta: 0.6\n  h:\n    unit: m\n    value: 101",yaml.spectra.at(1).model_yaml);
    ASSERT_EQ("dirac",yaml.spectra.at(1).spectral_density_type);
    ASSERT_EQ("Hs:\n  unit: m\n  value: 5\nTp:\n  unit: s\n  value: 15\ntype: dirac",yaml.spectra.at(1).spectral_density_yaml);
}

TEST_F(environment_parsersTest, can_parse_wave_outputs)
{
    ASSERT_EQ("NED", yaml.output.frame_of_reference);
    ASSERT_DOUBLE_EQ(-10, yaml.output.xmin);
    ASSERT_DOUBLE_EQ(1000, yaml.output.xmax);
    ASSERT_EQ(10, yaml.output.nx);
    ASSERT_DOUBLE_EQ(-20, yaml.output.ymin);
    ASSERT_DOUBLE_EQ(3000, yaml.output.ymax);
    ASSERT_EQ(20, yaml.output.ny);
}


TEST_F(environment_parsersTest, can_parse_jonswap_spectrum)
{
    const YamlJonswap y = parse_jonswap("Hs: {value: 5, unit: m}\n"
                                        "Tp: {value: 15, unit: s}\n"
                                        "gamma: 1.2\n");
    ASSERT_DOUBLE_EQ(5, y.Hs);
    ASSERT_DOUBLE_EQ(15, y.Tp);
    ASSERT_DOUBLE_EQ(1.2, y.gamma);
}

TEST_F(environment_parsersTest, can_parse_dirac_spectrum)
{
    const YamlDiracSpectrum y = parse_wave_dirac_spectrum("Hs: {value: 5, unit: m}\n"
                                                          "omega0: {value: 15, unit: rad/s}\n");
    ASSERT_DOUBLE_EQ(5, y.Hs);
    ASSERT_DOUBLE_EQ(15, y.omega0);
}

TEST_F(environment_parsersTest, can_parse_pierson_moskowitz_spectrum)
{
    const YamlPiersonMoskowitz y = parse_pierson_moskowitz("Hs: {value: 5, unit: m}\n"
                                                           "Tp: {value: 15, unit: s}\n");
    ASSERT_DOUBLE_EQ(5, y.Hs);
    ASSERT_DOUBLE_EQ(15, y.Tp);
}

TEST_F(environment_parsersTest, can_parse_bretschneider_spectrum)
{
    const YamlBretschneider y = parse_bretschneider("Hs: {value: 5, unit: m}\n"
                                                    "Tp: {value: 15, unit: s}\n");
    ASSERT_DOUBLE_EQ(5, y.Hs);
    ASSERT_DOUBLE_EQ(15, y.Tp);
}

TEST_F(environment_parsersTest, can_parse_dirac_spreading)
{
    const YamlDiracDirection y = parse_wave_dirac_direction("waves propagating to: {value: 5, unit: deg}\n");
    ASSERT_DOUBLE_EQ(5*PI/180, y.psi0);
}

TEST_F(environment_parsersTest, can_parse_cos2s_spreading)
{
    const YamlCos2s y = parse_cos2s("s: 2\n"
                                    "waves propagating to: {value: 90, unit: deg}\n");
    ASSERT_DOUBLE_EQ(PI/2., y.psi0);
    ASSERT_DOUBLE_EQ(2, y.s);
}

TEST_F(environment_parsersTest, airy_wave_model)
{
    const boost::optional<int> seed = parse_seed_of_random_number_generator("seed of the random data generator: 1234\n");
    ASSERT_TRUE(seed);
    ASSERT_EQ(1234, seed);
}

TEST_F(environment_parsersTest, can_force_seed_to_zero)
{
    const boost::optional<int> seed = parse_seed_of_random_number_generator("seed of the random data generator: none\n");
    ASSERT_FALSE(seed);
}

TEST_F(environment_parsersTest, can_parse_depth_for_wave_models)
{
    ASSERT_EQ(0,yaml.spectra_from_rays.size());
    ASSERT_EQ(2,yaml.spectra.size());
    ASSERT_DOUBLE_EQ(1700, yaml.spectra[0].depth);
    ASSERT_DOUBLE_EQ(12, yaml.spectra[1].depth);
}

TEST_F(environment_parsersTest, can_parse_stretching_data)
{
    ASSERT_DOUBLE_EQ(0.5, yaml.spectra.at(0).stretching.delta);
    ASSERT_DOUBLE_EQ(100, yaml.spectra.at(0).stretching.h);
    ASSERT_DOUBLE_EQ(0.6, yaml.spectra.at(1).stretching.delta);
    ASSERT_DOUBLE_EQ(101, yaml.spectra.at(1).stretching.h);
}

TEST_F(environment_parsersTest, clearer_error_message_if_missing_unit_value)
{
    std::string wave_yaml = test_data::waves_for_parser_validation_only();
    boost::replace_all(wave_yaml, "h: {unit: m, value: 101}", "h: 101");
    try
    {
        parse_waves(wave_yaml);
    }
    catch (const InvalidInputException& e)
    {
        /*
         * G++ and Intel Compiler do not produce the same result for macro __PRETTY_FUNCTION__
         * There are some space differences in the result, (in front of & characters)
         * Therefore, we use regex to check the result.
         * Note: We do not use Google Test MatchesRegex, since it requires POSIX regex
         * that is not shipped with Windows MXE cross platform build
         * Thus, we use boost::regex!
         */
        const boost::regex expected_regex("Error parsing section wave/spectra: In file /opt/share/code/yaml_parser/environment_parsers.cpp, line [1-9][0-9]*, function void operator>>\\(const YAML::Node\\s*&, YamlStretching\\s*&\\): Error parsing wave stretching parameters \\('wave/spectra/stretching' section in the YAML file\\): In file /opt/share/code/yaml_parser/environment_parsers.cpp, line [1-9][0-9]*, function void operator>>\\(const YAML::Node\\s*&, YamlStretching\\s*&\\): Error parsing wave stretching parameters 'h': was expecting an object with fields 'unit' and 'value', e.g.:\n\th: \\{unit: 'm', value: 101\\}\nbut got the following error trying to parse it: yaml-cpp: error at line 0, column 0: bad dereference");
        ASSERT_TRUE(boost::regex_match(e.get_message(), expected_regex)) << e.get_message();
    }
    ASSERT_THROW(parse_waves(wave_yaml), InvalidInputException);
}

TEST_F(environment_parsersTest, can_parse_grpc_data)
{
    const std::string yaml_string = "model: grpc\n"
                                    "url: localhost:50001\n"
                                    "xmin: -10\n"
                                    "xmax: 10\n";
    const YamlGRPC yaml_grpc = parse_grpc(yaml_string);
    ASSERT_EQ("localhost:50001", yaml_grpc.url);
    ASSERT_EQ(yaml_string, yaml_grpc.rest_of_the_yaml);
}

TEST_F(environment_parsersTest, can_parse_equal_energy_bins_parameter)
{
    ASSERT_TRUE(yaml.discretization.equal_energy_bins);
    std::string wave_yaml = test_data::waves_for_parser_validation_only();
    boost::replace_all(wave_yaml, "equal energy bins: true", "equal energy bins: false");
    const auto res = parse_waves(wave_yaml);
    ASSERT_FALSE(res.discretization.equal_energy_bins);
}

const YamlWaveModel environment_parsers_from_list_of_rays_Test::yaml = parse_waves(test_data::waves_from_a_list_of_rays_for_parser_validation_only());

environment_parsers_from_list_of_rays_Test::environment_parsers_from_list_of_rays_Test() : a(ssc::random_data_generator::DataGenerator(1212))
{
}

TEST_F(environment_parsers_from_list_of_rays_Test, can_parse_depth_for_wave_models)
{
    ASSERT_EQ(0,yaml.spectra.size());
    ASSERT_EQ(2, yaml.spectra_from_rays.size());
    ASSERT_DOUBLE_EQ(1700, yaml.spectra_from_rays[0].depth);
    ASSERT_DOUBLE_EQ(1000, yaml.spectra_from_rays[1].depth);
}

TEST_F(environment_parsers_from_list_of_rays_Test, can_parse_stretching_data)
{
    ASSERT_DOUBLE_EQ(0.5, yaml.spectra_from_rays.at(0).stretching.delta);
    ASSERT_DOUBLE_EQ(100, yaml.spectra_from_rays.at(0).stretching.h);
    ASSERT_DOUBLE_EQ(0.6, yaml.spectra_from_rays.at(1).stretching.delta);
    ASSERT_DOUBLE_EQ(101, yaml.spectra_from_rays.at(1).stretching.h);
}

TEST_F(environment_parsers_from_list_of_rays_Test, can_parse_rays_for_wave_models)
{
    ASSERT_EQ(0, yaml.spectra.size());
    ASSERT_EQ(2, yaml.spectra_from_rays.size());
    ASSERT_EQ(2, yaml.spectra_from_rays[0].rays.a.size());
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.a[0], 0.1);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.a[1], 0.5);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.psi[0], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.psi[1], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.omega[0], 0.628318);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.omega[1], 0.785398);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.k[0], 3.872832);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.k[1], 6.051301);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.phase[0], 50/180.0*PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.phase[1], 90/180.0*PI);
    ASSERT_EQ(2, yaml.spectra_from_rays[1].rays.a.size());
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.a[0], 0.1);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.a[1], 0.5);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.psi[0], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.psi[1], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.omega[0], 0.523598);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.omega[1], 0.628318);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.k[0], 2.689467);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.k[1], 3.872832);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.phase[0], 110/180.0*PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.phase[1], 130/180.0*PI);
}

TEST_F(environment_parsers_from_external_list_of_rays_Test, can_parse_all_data)
{
    const std::string filename("rays.csv");
    std::ofstream file(filename);
    file << test_data::waves_from_an_external_list_of_rays_for_parser_validation_only_csv();
    file.close();
    const YamlWaveModel yaml = parse_waves(test_data::waves_from_an_external_list_of_rays_for_parser_validation_only());
    ASSERT_EQ(0,yaml.spectra.size());
    ASSERT_EQ(2, yaml.spectra_from_rays.size());
    ASSERT_DOUBLE_EQ(1700, yaml.spectra_from_rays[0].depth);
    ASSERT_DOUBLE_EQ(1000, yaml.spectra_from_rays[1].depth);
    ASSERT_DOUBLE_EQ(0.5, yaml.spectra_from_rays.at(0).stretching.delta);
    ASSERT_DOUBLE_EQ(100, yaml.spectra_from_rays.at(0).stretching.h);
    ASSERT_DOUBLE_EQ(0.6, yaml.spectra_from_rays.at(1).stretching.delta);
    ASSERT_DOUBLE_EQ(101, yaml.spectra_from_rays.at(1).stretching.h);
    ASSERT_EQ(0, yaml.spectra.size());
    ASSERT_EQ(2, yaml.spectra_from_rays.size());
    ASSERT_EQ(2, yaml.spectra_from_rays[0].rays.a.size());
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.a[0], 0.1);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.a[1], 0.5);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.psi[0], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.psi[1], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.omega[0], 0.628318);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.omega[1], 0.785398);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.k[0], 3.872832);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.k[1], 6.051301);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.phase[0], 50/180.0*PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[0].rays.phase[1], 90/180.0*PI);
    ASSERT_EQ(2, yaml.spectra_from_rays[1].rays.a.size());
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.a[0], 0.1);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.a[1], 0.5);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.psi[0], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.psi[1], PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.omega[0], 0.523598);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.omega[1], 0.628318);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.k[0], 2.689467);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.k[1], 3.872832);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.phase[0], 110/180.0*PI);
    ASSERT_DOUBLE_EQ(yaml.spectra_from_rays[1].rays.phase[1], 130/180.0*PI);
    EXPECT_EQ(0,remove(filename.c_str()));
}
