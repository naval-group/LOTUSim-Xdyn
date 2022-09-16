CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(listeners_and_controllers)

SET(SRC
    builders.cpp
    listeners.cpp
    InterpolationModule.cpp
    Controller.cpp
    PIDController.cpp
    GrpcController.cpp
    CSVController.cpp
    CSVLineByLineReader.cpp
    TempFile.cpp
    CSVYaml.cpp
    )

INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

ADD_SUBDIRECTORY(unit_tests)
