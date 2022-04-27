# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST listeners_and_controllers)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/GrpcControllerTest.cpp
              src/CSVLineByLineReaderTest.cpp
              )

# Tests for Controller.cpp are found in:
# - observers_and_api/unit_tests/src/PIDControllerTest.cpp
# because these tests need a Sim instance, which requires the observers_and_api include directory.
# To avoid cross-dependencies, the corresponding unit tests are moved to observers_and_api/unit_tests.

# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${simulator_yaml_parser_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
