SET(MODULE_UNDER_TEST environment_models)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    JonswapSpectrumTest.cpp
    PiersonMoskowitzSpectrumTest.cpp
    BretschneiderSpectrumTest.cpp
    DiracDirectionalSpreadingTest.cpp
    Cos2sDirectionalSpreadingTest.cpp
    DiscreteDirectionalWaveSpectrumTest.cpp
    DiracSpectralDensityTest.cpp
    WaveNumberFunctorTest.cpp
    AiryTest.cpp
    discretizeTest.cpp
    WaveSpectralDensityTest.cpp
    StretchingTest.cpp
    DefaultWindModelTest.cpp
    WindMeanVelocityProfileTest.cpp
    )

INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_macros_SOURCE_DIR})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${kissfft_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
