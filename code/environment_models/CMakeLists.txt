CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(environment_models)

set(SRC
    Airy.cpp
    BretschneiderSpectrum.cpp
    Cos2sDirectionalSpreading.cpp
    DefaultWindModel.cpp
    DiracDirectionalSpreading.cpp
    DiracSpectralDensity.cpp
    DiscreteDirectionalWaveSpectrum.cpp
    discretize.cpp
    JonswapSpectrum.cpp
    LogWindVelocityProfile.cpp
    PiersonMoskowitzSpectrum.cpp
    PowerLawWindVelocityProfile.cpp
    Stretching.cpp
    SumOfWaveDirectionalSpreadings.cpp
    SumOfWaveSpectralDensities.cpp
    UniformWindVelocityProfile.cpp
    WaveDirectionalSpreading.cpp
    WaveDirectionalSpreading.cpp
    WaveModel.cpp
    WaveNumberFunctor.cpp
    WaveSpectralDensity.cpp
    WindMeanVelocityProfile.cpp
    WindModel.cpp
    YamlSpectraInput.cpp
)

INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
