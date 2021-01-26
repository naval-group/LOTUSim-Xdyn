/*
 * ForceTests.hpp
 *
 *  Created on: Feb 16, 2015
 *      Author: cady
 */


#ifndef FORCETESTS_HPP_
#define FORCETESTS_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "DiffractionForceModel.hpp"
struct YamlModel;

class ForceTests : public ::testing::Test
{
    protected:
        ForceTests();
        virtual ~ForceTests();
        virtual void SetUp();
        virtual void TearDown();
        EnvironmentAndFrames get_env(const YamlModel& waves, const std::string& body_name) const;
        DiffractionForceModel get_diffraction_force_model(const EnvironmentAndFrames& env, const std::string& body_name, const std::string& diffraction_yaml, const std::string& hdb_file_contents) const;
        YamlModel get_regular_wave(const double propagation_angle_in_ned_frame_in_degrees, const double Hs_in_meters, const double wave_period_in_seconds) const;
        std::string get_diffraction_conf(const double x, const double y, const double z) const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* FORCETESTS_HPP_ */
