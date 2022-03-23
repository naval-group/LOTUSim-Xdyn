/*
 * CSVControllerTest.hpp
 *
 *  Created on: Mar 16, 2022
 *      Author: cady
 */

#ifndef CSVCONTROLLERTEST_HPP_
#define CSVCONTROLLERTEST_HPP_

#include <fstream>
#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class TempFile
{
    public:
        TempFile();
        ~TempFile();

        template <typename T> TempFile& operator<<(const T& rhs)
        {
            csv << rhs;
            return *this;
        }

        std::string get_filename() const;

        void close();

    private:
        std::string filename;
        std::ofstream csv;
};

class CSVControllerTest : public ::testing::Test
{
    protected:
      CSVControllerTest();
      virtual ~CSVControllerTest();
      virtual void SetUp();
      virtual void TearDown();
      std::string test_yaml() const;
      TempFile csv;
      ssc::random_data_generator::DataGenerator a;
};

#endif  /* PIDCONTROLLERTEST_HPP_ */
