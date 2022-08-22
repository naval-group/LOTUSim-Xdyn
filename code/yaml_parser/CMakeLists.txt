cmake_minimum_required(VERSION 2.8.8)
project(yaml_parser)

set(SRC src/SimulatorYamlParser.cpp
        src/environment_parsers.cpp
        src/check_input_yaml.cpp
        src/external_data_structures_parsers.cpp
        src/parse_controllers.cpp
        src/parse_output.cpp
        src/parse_time_series.cpp
        src/parse_address.cpp
        )

include_directories(inc)
include_directories(${ssc_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(${environment_models_INCLUDE_DIRS})
include_directories(..)

add_library(${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
