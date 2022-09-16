SET(MODULE_UNDER_TEST external_file_formats)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    low_level_hdb_parserTest.cpp
    stl_io_hdf5Test.cpp
    stl_readerTest.cpp
    stl_writerTest.cpp
    )
INCLUDE_DIRECTORIES(SYSTEM ${base91x_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
