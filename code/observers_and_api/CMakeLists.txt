CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(observers_and_api)

INCLUDE(CMakeListsGenerateDemoMatLab.txt)
INCLUDE(CMakeListsGenerateDemoPython.txt)

SET(SRC
    simulator_api.cpp
    ListOfObservers.cpp
    Hdf5Observer.cpp
    Hdf5WaveObserver.cpp
    Hdf5WaveObserverBuilder.cpp
    Hdf5WaveSpectrumObserver.cpp
    SimObserver.cpp
    CsvObserver.cpp
    DictObserver.cpp
    TsvObserver.cpp
    JsonObserver.cpp
    WebSocketObserver.cpp
    ${CMAKE_BINARY_DIR}/demoMatLab.cpp
    ${CMAKE_BINARY_DIR}/demoPython.cpp
    MapObserver.cpp
    ConfBuilder.cpp
    JSONSerializer.cpp
    XdynForCS.cpp
    XdynForME.cpp
    SimServerInputs.cpp
    EverythingObserver.cpp
    gRPCProtoBufServer.cpp
    SimulationServerObserver.cpp
    CoSimulationObserver.cpp
    )

INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${interface_hdf5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${force_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${listeners_and_controllers_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${grpc_INCLUDE_DIRS}) # For the code/grpc folder
INCLUDE_DIRECTORIES(SYSTEM ${GRPC_INCLUDE_DIR}) # For the gRPC library
INCLUDE_DIRECTORIES(SYSTEM ${HDF5_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_hdf5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${base91x_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
