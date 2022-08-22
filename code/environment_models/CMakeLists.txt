cmake_minimum_required(VERSION 2.8.8)
project(environment_models)

set(SRC src/WaveSpectralDensity.cpp
        src/JonswapSpectrum.cpp
        src/PiersonMoskowitzSpectrum.cpp
        src/BretschneiderSpectrum.cpp
        src/WaveDirectionalSpreading.cpp
        src/DiracDirectionalSpreading.cpp
        src/Cos2sDirectionalSpreading.cpp
        src/DiscreteDirectionalWaveSpectrum.cpp
        src/DiracSpectralDensity.cpp
        src/WaveNumberFunctor.cpp
        src/Airy.cpp
        src/WaveModel.cpp
        src/discretize.cpp
        src/YamlSpectraInput.cpp
        src/SumOfWaveSpectralDensities.cpp
        src/SumOfWaveDirectionalSpreadings.cpp
        src/WaveDirectionalSpreading.cpp
        src/Stretching.cpp
        src/WindModel.cpp
        src/DefaultWindModel.cpp
        src/WindMeanVelocityProfile.cpp
        src/UniformWindVelocityProfile.cpp
        src/PowerLawWindVelocityProfile.cpp
        src/LogWindVelocityProfile.cpp
        )

include_directories(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

add_library (${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
