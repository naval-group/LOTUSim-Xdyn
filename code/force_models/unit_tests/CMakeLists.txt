# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
SET(MODULE_UNDER_TEST force_models)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    QuadraticDampingForceModelTest.cpp
    DefaultSurfaceElevationTest.cpp
    GravityForceModelTest.cpp
    HydrostaticForceModelTest.cpp
    FroudeKrylovForceModelTest.cpp
    LinearDampingForceModelTest.cpp
    WageningenControlledForceModelTest.cpp
    ResistanceCurveForceModelTest.cpp
    RadiationDampingForceModelTest.cpp
    HDBParserForTests.cpp
    DiffractionForceModelTest.cpp
    SimpleHeadingKeepingControllerTest.cpp
    ManeuveringForceModelTest.cpp
    maneuvering_parserTest.cpp
    maneuvering_compilerTest.cpp
    NumericalEvaluator.cpp
    StringEvaluator.cpp
    maneuvering_DataSource_builderTest.cpp
    SimpleStationKeepingControllerTest.cpp
    RudderForceModelTest.cpp
    env_for_tests.cpp
    KtKqForceModelTest.cpp
    LinearHydrostaticForceModelTest.cpp
    ConstantForceModelTest.cpp
    HoltropMennenForceModelTest.cpp
    AeroPolarForceModelTest.cpp
    HydroPolarForceModelTest.cpp
    LinearFroudeKrylovForceModelTest.cpp
    MMGManeuveringForceModelTest.cpp
    LinearStiffnessForceModelTest.cpp
    FlettnerRotorForceModelTest.cpp
    )
# ------8<---------------------------------------------->8-----

# Include directories
INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_INCLUDE_DIRS}/../unit_tests/inc)
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS}/../unit_tests/inc)
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS}/../unit_tests/inc)

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
