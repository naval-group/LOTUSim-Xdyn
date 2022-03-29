/*
 * CSVFileReaderTest.hpp
 *
 *  Created on: March 29, 2022
 *      Author: cady
 */

#ifndef CSVFILEREADERTEST_HPP_
#define CSVFILEREADERTEST_HPP_

#include "gtest/gtest.h"
#include "CSVLineByLineReader.hpp"
#include "TempFile.hpp"

class CSVLineByLineReaderTest : public ::testing::Test
{
    protected:
        CSVLineByLineReaderTest();
        virtual ~CSVLineByLineReaderTest();
        virtual void SetUp();
        virtual void TearDown();
        CSVLineByLineReader get_reader() const;
        TempFile csv;
};


#endif /* CSVFILEREADERTEST_HPP_ */
