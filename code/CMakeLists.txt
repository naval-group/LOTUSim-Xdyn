CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(x-dyn)
SET(DESCRIPTION "X-DYN : Simulateur 6ddl de navires")
SET(CMAKE_VERBOSE_MAKEFILE OFF)
#############################################################################
OPTION(BUILD_DOCUMENTATION "Boolean used to build xdyn documentation" ON)
OPTION(THIRD_PARTY_DIRECTORY "Where should CMake look for (former submodules) eigen, eigen3-hdf5, etc... ?" ${CMAKE_CURRENT_SOURCE_DIR})
#############################################################################
# User configuration
IF(NOT DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

SET(CMAKE_CXX_STANDARD 11)

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
   SET(ARCH64_BITS TRUE)
ELSE()
   SET(ARCH64_BITS FALSE)
ENDIF()

IF(DEFINED INSTALL_PREFIX)
    SET(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
ELSE()
    SET(CMAKE_INSTALL_PREFIX ../install${CMAKE_BUILD_TYPE})
ENDIF()
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX :${CMAKE_INSTALL_PREFIX}")

#############################################################################
# Compilation configuration
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#############################################################################
# GIT SHA
INCLUDE(GetGitRevisionDescription)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt"
               "${CMAKE_CURRENT_BINARY_DIR}/Copyright.txt" @ONLY)
#############################################################################
# External libraries
INCLUDE(CMakeOutputDirectories)
INCLUDE(CMakeGMock)
INCLUDE(CMakeYamlCpp)
INCLUDE(CMakeEigen)
INCLUDE(CMakeHdf5)
INCLUDE(CMakeProtoBuf)
INCLUDE(CMakeGRPC)

INCLUDE(CMakeBoost)
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
#SET(Boost_USE_STATIC_RUNTIME    OFF)
FIND_PACKAGE(Boost 1.53 COMPONENTS program_options filesystem system regex thread random chrono REQUIRED) #random
IF(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIRS  : ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES     : ${Boost_LIBRARIES}")
ENDIF()

#############################################################################
# Compilation part
ADD_SUBDIRECTORY(ssc/ssc/f2c)
INCLUDE(CMakeCompilationOptions)
SET(ssc_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/ssc" CACHE PATH "Path to SSC include directory")
SET(SSC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/ssc" CACHE PATH "Path to SSC include directory")
SET(F2C_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ssc/ssc/f2c" CACHE PATH "Path to f2c include directory")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/ssc)
ADD_SUBDIRECTORY(base91x)
ADD_SUBDIRECTORY(boost_program_options_descriptions)
ADD_SUBDIRECTORY(xdyn/binary_stl_data)
ADD_SUBDIRECTORY(xdyn/core)
ADD_SUBDIRECTORY(xdyn/exceptions)
ADD_SUBDIRECTORY(xdyn/environment_models)
ADD_SUBDIRECTORY(xdyn/external_data_structures)
ADD_SUBDIRECTORY(xdyn/external_file_formats)
ADD_SUBDIRECTORY(get_git_sha)
ADD_SUBDIRECTORY(test_data_generator)
ADD_SUBDIRECTORY(interface_hdf5)
ADD_SUBDIRECTORY(mesh)
ADD_SUBDIRECTORY(yaml_parser)
ADD_SUBDIRECTORY(hdb_interpolators)
ADD_SUBDIRECTORY(xdyn/force_models)
ADD_SUBDIRECTORY(grpc)
ADD_SUBDIRECTORY(listeners_and_controllers)
ADD_SUBDIRECTORY(observers_and_api)
ADD_SUBDIRECTORY(gz_curves)
ADD_SUBDIRECTORY(xdyn/executables)
ADD_SUBDIRECTORY(fmi)

IF (WIN32)
    SET(STATICORSHARED STATIC)
ELSE()
    SET(STATICORSHARED SHARED)
ENDIF()
SET(websocketpp_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ssc/ssc/websocketpp)
ADD_SUBDIRECTORY(ssc/ssc/data_source)
ADD_SUBDIRECTORY(ssc/ssc/random_data_generator)
ADD_SUBDIRECTORY(ssc/ssc/exception_handling)
ADD_SUBDIRECTORY(ssc/ssc/decode_unit)
ADD_SUBDIRECTORY(ssc/ssc/yaml_parser)
ADD_SUBDIRECTORY(ssc/ssc/text_file_reader)
ADD_SUBDIRECTORY(ssc/ssc/integrate)
ADD_SUBDIRECTORY(ssc/ssc/numeric)
ADD_SUBDIRECTORY(ssc/ssc/kinematics)
ADD_SUBDIRECTORY(ssc/ssc/interpolation)
ADD_SUBDIRECTORY(ssc/ssc/csv_file_reader)
ADD_SUBDIRECTORY(ssc/ssc/websocket)
ADD_SUBDIRECTORY(ssc/ssc/json)
ADD_SUBDIRECTORY(ssc/ssc/solver)



ADD_LIBRARY(${PROJECT_NAME} ${STATICORSHARED}
    $<TARGET_OBJECTS:random_data_generator_object>
    $<TARGET_OBJECTS:exception_handling_object>
    $<TARGET_OBJECTS:decode_unit_object>
    $<TARGET_OBJECTS:yaml_parser_object>
    $<TARGET_OBJECTS:text_file_reader_object>
    $<TARGET_OBJECTS:integrate_object>
    $<TARGET_OBJECTS:numeric_object>
    $<TARGET_OBJECTS:data_source_object>
    $<TARGET_OBJECTS:kinematics_object>
    $<TARGET_OBJECTS:interpolation_object>
    $<TARGET_OBJECTS:csv_file_reader_object>
    $<TARGET_OBJECTS:websocket_object>
    $<TARGET_OBJECTS:f2c_object>
    $<TARGET_OBJECTS:json_object>
    $<TARGET_OBJECTS:solver_object>

        $<TARGET_OBJECTS:binary_stl_data>
        $<TARGET_OBJECTS:core>
        $<TARGET_OBJECTS:get_git_sha>
        $<TARGET_OBJECTS:observers_and_api>
        $<TARGET_OBJECTS:listeners_and_controllers>
        $<TARGET_OBJECTS:force_models>
        $<TARGET_OBJECTS:environment_models>
        $<TARGET_OBJECTS:external_file_formats>
        $<TARGET_OBJECTS:mesh>
        $<TARGET_OBJECTS:yaml_parser>
        $<TARGET_OBJECTS:fmi_simulator>
        $<TARGET_OBJECTS:external_data_structures>
        $<TARGET_OBJECTS:hdb_interpolators>
        $<TARGET_OBJECTS:interface_hdf5>
        $<TARGET_OBJECTS:gz_curves>
        $<TARGET_OBJECTS:grpc>
        $<TARGET_OBJECTS:grpc_gen>
        )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE yaml-cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE gfortran)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${HDF5_CXX_STATIC_LIBRARY} ${HDF5_C_STATIC_LIBRARY})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${Boost_FILESYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${Boost_REGEX_LIBRARY})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${Boost_THREAD_LIBRARY})
IF(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC wsock32 ws2_32)
ELSE()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC pthread)
ENDIF()
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

IF(UNIX AND NOT(WIN32))
    SET(LIBRARY_OUTPUT_DIRECTORY "lib")
    INSTALL(TARGETS ${PROJECT_NAME}
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib)
ELSE()
    INSTALL(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
            ARCHIVE DESTINATION ${LIBRARY_OUTPUT_DIRECTORY}
            LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
ENDIF()
#############################################################################
# Testing part

INSTALL(FILES integration_tests.py
        DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

SET(TEST_EXE run_all_tests)
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${GRPC_INCLUDE_DIR})
ADD_EXECUTABLE(${TEST_EXE}
        xdyn/executables/run_all_tests.cpp
        $<TARGET_OBJECTS:mesh_tests>
        $<TARGET_OBJECTS:yaml_parser_tests>
        $<TARGET_OBJECTS:external_file_formats_tests>
        $<TARGET_OBJECTS:core_tests>
        $<TARGET_OBJECTS:environment_models_tests>
        $<TARGET_OBJECTS:force_models_tests>
        $<TARGET_OBJECTS:listeners_and_controllers_tests>
        $<TARGET_OBJECTS:test_data_generator>
        $<TARGET_OBJECTS:hdb_interpolators_tests>
        $<TARGET_OBJECTS:interface_hdf5_tests>
        $<TARGET_OBJECTS:observers_and_api_tests>
        $<TARGET_OBJECTS:gz_curves_tests>
        $<TARGET_OBJECTS:fmi_simulator_tests>
        $<TARGET_OBJECTS:grpc_tests>
        )

TARGET_LINK_LIBRARIES(${TEST_EXE}
        gtest      # static
        gmock_main # static
        ${PROJECT_NAME}
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        ${Boost_FILESYSTEM_LIBRARY}
        )
SET_TARGET_PROPERTIES(${TEST_EXE} PROPERTIES LINKER_LANGUAGE CXX)

MESSAGE(STATUS "CMAKE_BUILD_TYPE_UPPER : ${CMAKE_BUILD_TYPE_UPPER}")
IF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
MESSAGE(STATUS "Adding coverage")
FIND_PACKAGE(codecov)
add_coverage(${TEST_EXE})
ENDIF()

INCLUDE(CMakeTesting)
IF(BUILD_DOCUMENTATION)
    MESSAGE(STATUS "Build documentation")
    INCLUDE(CMakeDocumentation)
ENDIF()
INCLUDE(CMakeCustomTargets)
INCLUDE(CMakePack)

IF(BUILD_PYTHON_WRAPPER)
    ADD_SUBDIRECTORY(wrapper_python)
ENDIF()
