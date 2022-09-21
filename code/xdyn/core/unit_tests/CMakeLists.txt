PROJECT(core_tests)
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

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
