PROJECT(environment_models_tests)
SET(SRC
    SeabedTest.cpp
    AiryTest.cpp
    BretschneiderSpectrumTest.cpp
    Cos2sDirectionalSpreadingTest.cpp
    DefaultWindModelTest.cpp
    DiracDirectionalSpreadingTest.cpp
    DiracSpectralDensityTest.cpp
    discretizeTest.cpp
    EkmanUWCurrentTest.cpp
    JonswapSpectrumTest.cpp
    PiersonMoskowitzSpectrumTest.cpp
    StretchingTest.cpp
    WaveNumberFunctorTest.cpp
    WaveSpectralDensityTest.cpp
    WindMeanVelocityProfileTest.cpp
    )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${THIRDPARTY_CODE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${ssc_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
