SET(MODULE_UNDER_TEST force_models)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    AeroPolarForceModelTest.cpp
    ConstantForceModelTest.cpp
    DefaultSurfaceElevationTest.cpp
    DiffractionForceModelTest.cpp
    env_for_tests.cpp
    FlettnerRotorForceModelTest.cpp
    FroudeKrylovForceModelTest.cpp
    GravityForceModelTest.cpp
    HDBParserForTests.cpp
    HoltropMennenForceModelTest.cpp
    HydroPolarForceModelTest.cpp
    HydrostaticForceModelTest.cpp
    KtKqForceModelTest.cpp
    LinearDampingForceModelTest.cpp
    LinearFroudeKrylovForceModelTest.cpp
    LinearHydrostaticForceModelTest.cpp
    LinearStiffnessForceModelTest.cpp
    maneuvering_compilerTest.cpp
    maneuvering_DataSource_builderTest.cpp
    ManeuveringForceModelTest.cpp
    maneuvering_parserTest.cpp
    MMGManeuveringForceModelTest.cpp
    NumericalEvaluator.cpp
    QuadraticDampingForceModelTest.cpp
    RadiationDampingForceModelTest.cpp
    ResistanceCurveForceModelTest.cpp
    RudderForceModelTest.cpp
    SimpleHeadingKeepingControllerTest.cpp
    SimpleStationKeepingControllerTest.cpp
    StringEvaluator.cpp
    WageningenControlledForceModelTest.cpp
    )

INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
