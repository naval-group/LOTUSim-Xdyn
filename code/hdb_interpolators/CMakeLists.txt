cmake_minimum_required(VERSION 2.8.8)
project(hdb_interpolators)

set(SRC src/HDBParser.cpp
        src/RadiationDampingBuilder.cpp
        src/History.cpp
        src/PrecalParserHelper.cpp
        src/PrecalParser.cpp
        src/RaoInterpolator.cpp
        src/HydroDBParser.cpp
        )

include_directories(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
