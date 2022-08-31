SET(MODULE_UNDER_TEST yaml_parser)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    environment_parsersTest.cpp
    parse_addressTest.cpp
    parse_controllersTest.cpp
    parse_outputTest.cpp
    parse_time_seriesTest.cpp
    SimulatorYamlParserTest.cpp
    )
INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
