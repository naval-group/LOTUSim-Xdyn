CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(external_data_structures)

SET(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

SET(SRC
    YamlAngle.cpp
    YamlBody.cpp
    YamlController.cpp
    YamlCoordinates.cpp
    YamlDynamics6x6Matrix.cpp
    YamlDynamics.cpp
    YamlEnvironmentalConstants.cpp
    YamlGRPC.cpp
    YamlModel.cpp
    YamlOutput.cpp
    YamlPoint.cpp
    YamlPosition.cpp
    YamlRadiationDamping.cpp
    YamlRAO.cpp
    YamlRotation.cpp
    YamlSimServerInputs.cpp
    YamlSimulatorInput.cpp
    YamlSpeed.cpp
    YamlState.cpp
    YamlTimeSeries.cpp
    YamlWaveModelInput.cpp
    YamlWaveOutput.cpp
    )

INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
