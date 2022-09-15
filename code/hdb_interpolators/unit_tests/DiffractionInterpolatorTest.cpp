/*
 * DiffractionInterpolatorTest.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cady
 */

#include "DiffractionInterpolatorTest.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "hdb_interpolators/HDBParser.hpp"
#include "hdb_data.hpp"
#include "RaoInterpolator.hpp"
#include "external_data_structures/YamlRAO.hpp"

DiffractionInterpolatorTest::DiffractionInterpolatorTest() : a(ssc::random_data_generator::DataGenerator(989899))
{
}

DiffractionInterpolatorTest::~DiffractionInterpolatorTest()
{
}

void DiffractionInterpolatorTest::SetUp()
{
}

void DiffractionInterpolatorTest::TearDown()
{
}

TEST_F(DiffractionInterpolatorTest, example)
{
//! [DiffractionInterpolatorTest example]
    const HDBParser parser = HDBParser::from_string(test_data::bug_3210());
    YamlRAO yaml;
    yaml.mirror = true;
    RaoInterpolator radiation(parser, yaml);
//! [DiffractionInterpolatorTest example]
//! [DiffractionInterpolatorTest expected output]
    const size_t k = 0; // X-axis
    double module = radiation.interpolate_module(k, 64.0, 0.0);
    ASSERT_NEAR(3.378373E+03, module, 1E-9);
    module = radiation.interpolate_module(k, 125.0, 30.0 / 180.0 * PI);
    ASSERT_NEAR(9.067188E+02, module, 1E-9);
    double phase = radiation.interpolate_phase(k, 64.0, 0.0);
    ASSERT_NEAR(2.088816E+00, phase, 1E-9);
    phase = radiation.interpolate_phase(k, 125.0, 30.0 / 180.0 * PI);
    ASSERT_NEAR(2.123063E+00, phase, 1E-9);
//! [DiffractionInterpolatorTest expected output]
}

TEST_F(DiffractionInterpolatorTest, example_deprecated_aquaplus_format)
{
    const HDBParser parser = HDBParser::from_string(test_data::bug_3210_deprecated_aquaplus_format());
    YamlRAO yaml;
    yaml.mirror = true;
    RaoInterpolator radiation(parser, yaml);
    const size_t k = 0; // X-axis
    double module = radiation.interpolate_module(k, 64.0, 0.0);
    ASSERT_NEAR(3.378373E+03, module, 1E-9);
    module = radiation.interpolate_module(k, 125.0, 30.0 / 180.0 * PI);
    ASSERT_NEAR(9.067188E+02, module, 1E-9);
    double phase = radiation.interpolate_phase(k, 64.0, 0.0);
    ASSERT_NEAR(2.088816E+00, phase, 1E-9);
    phase = radiation.interpolate_phase(k, 125.0, 30.0 / 180.0 * PI);
    ASSERT_NEAR(2.123063E+00, phase, 1E-9);
}
