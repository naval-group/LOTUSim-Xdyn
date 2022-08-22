cmake_minimum_required(VERSION 2.8.8)
project(gz_curves)

set(SRC src/GZTypes.cpp
        src/make_sim_for_GZ.cpp
        src/ResultantForceComputer.cpp
        src/gz_newton_raphson.cpp
        src/GZCurve.cpp
        )

include_directories(inc)
include_directories(${ssc_INCLUDE_DIRS})
include_directories(${core_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${environment_models_INCLUDE_DIRS})
include_directories(${listeners_and_controllers_INCLUDE_DIRS})
include_directories(${observers_and_api_INCLUDE_DIRS})
include_directories(${force_models_INCLUDE_DIRS})

include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${hdb_interpolators_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})


ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
