CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(environment_models)

set(SRC
    Airy.cpp
    BretschneiderSpectrum.cpp
    ConstantUWCurrentModel.cpp
    Cos2sDirectionalSpreading.cpp
    DefaultUWCurrentModel.cpp
    DefaultWindModel.cpp
    DiracDirectionalSpreading.cpp
    DiracSpectralDensity.cpp
    DiscreteDirectionalWaveSpectrum.cpp
    discretize.cpp
    # DnvrpUWCurrentModel.cpp
    EkmanUWCurrentModel.cpp
    # IsscUWCurrentModel.cpp
    JonswapSpectrum.cpp
    LogWindVelocityProfile.cpp
    # NocUWCurrentModel.cpp
    PiersonMoskowitzSpectrum.cpp
    PowerLawWindVelocityProfile.cpp
    Seabed.cpp
    Stretching.cpp
    SumOfWaveDirectionalSpreadings.cpp
    SumOfWaveSpectralDensities.cpp
    UniformWindVelocityProfile.cpp
    UWCurrentModel.cpp
    WaveDirectionalSpreading.cpp
    WaveModel.cpp
    WaveNumberFunctor.cpp
    WaveSpectralDensity.cpp
    WindMeanVelocityProfile.cpp
    WindModel.cpp
    YamlSpectraInput.cpp
)

INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${ssc_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

ADD_SUBDIRECTORY(unit_tests)
