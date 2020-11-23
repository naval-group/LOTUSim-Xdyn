/*
 * DiscretizeTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DISCRETIZETEST_HPP_
#define DISCRETIZETEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "YamlRadiationDamping.hpp"

class discretizeTest : public ::testing::Test
{
    protected:
        discretizeTest();
        virtual ~discretizeTest();
        virtual void SetUp();
        virtual void TearDown();
        std::vector<double> random_increasing_vector_of_size(const size_t n) const;
        ssc::random_data_generator::DataGenerator a;
        const std::vector<TypeOfQuadrature> quadratures;

};

#endif  /* DISCRETIZETEST_HPP_ */
