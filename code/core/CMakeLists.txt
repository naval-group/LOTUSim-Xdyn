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

INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ds_integration_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${listeners_and_controllers_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

SET(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
