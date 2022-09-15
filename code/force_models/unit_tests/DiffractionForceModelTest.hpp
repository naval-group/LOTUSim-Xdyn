/*
 * DiffractionForceModelTest.hpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */


#ifndef DIFFRACTIONFORCEMODELTEST_HPP_
#define DIFFRACTIONFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include "environment_models/WaveModel.hpp"

class DiffractionForceModelTest : public ::testing::Test
{
    public:
        static TR1(shared_ptr)<WaveModel> get_wave_model(const double period, const double direction=0., const double height=1.);
        static EnvironmentAndFrames get_waves_env(const double period, const double direction=0., const double amplitude = 1.);
        static BodyStates get_states_with_forward_speed(const double u = 0);
    protected:
        DiffractionForceModelTest();
        virtual ~DiffractionForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* DIFFRACTIONFORCEMODELTEST_HPP_ */
