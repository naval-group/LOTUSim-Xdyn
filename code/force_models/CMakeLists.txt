CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(force_models)

SET(SRC
    AbstractRaoForceModel.cpp
    AbstractWageningen.cpp
    AeroPolarForceModel.cpp
    calculate_gz.cpp
    ConstantForceModel.cpp
    DampingForceModel.cpp
    DiffractionForceModel.cpp
    ExactHydrostaticForceModel.cpp
    FastHydrostaticForceModel.cpp
    FlettnerRotorForceModel.cpp
    FroudeKrylovForceModel.cpp
    GMForceModel.cpp
    GravityForceModel.cpp
    HoltropMennenForceModel.cpp
    HydroPolarForceModel.cpp
    KtKqForceModel.cpp
    LinearDampingForceModel.cpp
    LinearFroudeKrylovForceModel.cpp
    LinearHydrostaticForceModel.cpp
    LinearStiffnessForceModel.cpp
    maneuvering_compiler.cpp
    maneuvering_DataSource_builder.cpp
    ManeuveringForceModel.cpp
    ManeuveringInternal.cpp
    MMGManeuveringForceModel.cpp
    PhaseModuleRAOEvaluator.cpp
    QuadraticDampingForceModel.cpp
    RadiationDampingForceModel.cpp
    ResistanceCurveForceModel.cpp
    RudderForceModel.cpp
    SimpleHeadingKeepingController.cpp
    SimpleStationKeepingController.cpp
    WageningenControlledForceModel.cpp
    )

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
