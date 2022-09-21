PROJECT(environment_models_tests)
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

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
