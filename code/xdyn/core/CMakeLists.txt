CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(core)

SET(SRC
    BlockedDOF.cpp
    BodyBuilder.cpp
    Body.cpp
    BodyStates.cpp
    BodyStates.cpp
    BodyWithoutSurfaceForces.cpp
    BodyWithSurfaceForces.cpp
    DefaultSurfaceElevation.cpp
    EmergedSurfaceForceModel.cpp
    EnvironmentAndFrames.cpp
    ForceModel.cpp
    ImmersedSurfaceForceModel.cpp
    Observer.cpp
    Res.cpp
    Sim.cpp
    SimulatorBuilder.cpp
    State.cpp
    StatesFilter.cpp
    SurfaceElevationBuilder.cpp
    SurfaceElevationFromWaves.cpp
    SurfaceElevationInterface.cpp
    SurfaceForceModel.cpp
    update_kinematics.cpp
    Wrench.cpp
    yaml2eigen.cpp
)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

ADD_SUBDIRECTORY(unit_tests)
