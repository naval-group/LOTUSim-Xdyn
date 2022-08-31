SET(MODULE_UNDER_TEST core)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    generate_body_for_tests.cpp
    SimulatorBuilderTest.cpp
    update_kinematicsTests.cpp
    BodyBuilderTest.cpp
    SurfaceElevationFromWavesTest.cpp
    BodyTest.cpp
    ForceModelTest.cpp
    random_kinematics.cpp
    BlockedDOFTest.cpp
    WrenchTest.cpp
    EnvironmentAndFramesTest.cpp
    StatesFilterTest.cpp
    )

INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${listeners_and_controllers_INCLUDE_DIRS}/../unit_tests)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "--std=gnu++0x")
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
