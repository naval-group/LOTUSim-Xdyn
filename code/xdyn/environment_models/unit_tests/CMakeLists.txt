SET(MODULE_UNDER_TEST environment_models)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    AiryTest.cpp
    BretschneiderSpectrumTest.cpp
    Cos2sDirectionalSpreadingTest.cpp
    DefaultWindModelTest.cpp
    DiracDirectionalSpreadingTest.cpp
    DiracSpectralDensityTest.cpp
    discretizeTest.cpp
    JonswapSpectrumTest.cpp
    PiersonMoskowitzSpectrumTest.cpp
    StretchingTest.cpp
    WaveNumberFunctorTest.cpp
    WaveSpectralDensityTest.cpp
    WindMeanVelocityProfileTest.cpp
    )

INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
