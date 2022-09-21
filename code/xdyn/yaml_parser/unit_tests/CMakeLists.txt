PROJECT(yaml_parser_tests)
SET(SRC
    environment_parsersTest.cpp
    parse_addressTest.cpp
    parse_controllersTest.cpp
    parse_outputTest.cpp
    parse_time_seriesTest.cpp
    SimulatorYamlParserTest.cpp
    )
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
