SET(MODULE_UNDER_TEST core)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    BlockedDOFTest.cpp
    BodyBuilderTest.cpp
    BodyTest.cpp
    EnvironmentAndFramesTest.cpp
    ForceModelTest.cpp
    SimulatorBuilderTest.cpp
    StatesFilterTest.cpp
    SurfaceElevationFromWavesTest.cpp
    WrenchTest.cpp
    generate_body_for_tests.cpp
    random_kinematics.cpp
    update_kinematicsTests.cpp
    )

INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${listeners_and_controllers_INCLUDE_DIRS}/../unit_tests)

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
