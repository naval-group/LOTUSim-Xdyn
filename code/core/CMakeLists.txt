cmake_minimum_required(VERSION 2.8.8)
project(core)

set(SRC src/update_kinematics.cpp
        src/Sim.cpp
        src/SimulatorBuilder.cpp
        src/Res.cpp
        src/SurfaceElevationBuilder.cpp
        src/Body.cpp
        src/BodyWithSurfaceForces.cpp
        src/BodyWithoutSurfaceForces.cpp
        src/BodyStates.cpp
        src/BodyBuilder.cpp
        src/EnvironmentAndFrames.cpp
        src/ForceModel.cpp
        src/SurfaceElevationFromWaves.cpp
        src/SurfaceElevationInterface.cpp
        src/SurfaceForceModel.cpp
        src/ImmersedSurfaceForceModel.cpp
        src/EmergedSurfaceForceModel.cpp
        src/yaml2eigen.cpp
        src/DefaultSurfaceElevation.cpp
        src/BodyStates.cpp
        src/Observer.cpp
        src/BlockedDOF.cpp
        src/State.cpp
        src/Wrench.cpp
        src/StatesFilter.cpp
        )

include_directories(inc)
include_directories(${ssc_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${external_file_formats_INCLUDE_DIRS})
include_directories(${ds_integration_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
include_directories(${environment_models_INCLUDE_DIRS})
include_directories(${listeners_and_controllers_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${hdb_interpolators_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
