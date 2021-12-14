cmake_minimum_required(VERSION 3.4...3.18)
project(xdyn_python)

set(PYTHON_PACKAGE_NAME pyxdyn)

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

message(STATUS "Python_VERSION ${Python_VERSION}")
message(STATUS "Python_VERSION_MAJOR ${Python_VERSION_MAJOR}")
message(STATUS "Python_VERSION_MINOR ${Python_VERSION_MINOR}")

message(STATUS "PYTHON WRAPPER!")
if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib.linux-x86_64-${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/)
endif()
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

pybind11_add_module(${PYTHON_PACKAGE_NAME}
    src/main.cpp
    src/py_sandbox.cpp
    src/py_ssc.cpp
    src/py_xdyn_core.cpp
    src/py_xdyn_exe.cpp
    src/py_xdyn_data.cpp
    src/py_xdyn_env.cpp
    src/py_xdyn_force.cpp
    src/py_xdyn_hdb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../executables/src/XdynCommandLineArguments.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../executables/src/ErrorReporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../executables/src/build_observers_description.cpp
    )
set_target_properties(${PYTHON_PACKAGE_NAME} PROPERTIES OUTPUT_NAME "xdyn")

# set_target_properties(${PYTHON_PACKAGE_NAME} PROPERTIES SUFFIX ".cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-x86_64-linux-gnu.so")

target_compile_definitions(${PYTHON_PACKAGE_NAME} PRIVATE GIT_VERSION="${GIT_VERSION_MAJOR}.${GIT_VERSION_MINOR}.${GIT_VERSION_PATCH}")

message(STATUS "ssc_INCLUDE_DIRS ${ssc_INCLUDE_DIRS}")
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE ${Python_INCLUDE_DIRS})
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${ssc_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${exceptions_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${external_data_structures_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${external_file_formats_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${mesh_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${environment_models_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${yaml_parser_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${hdb_interpolators_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${core_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE $<BUILD_INTERFACE:${interface_hdf5_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../executables/inc)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../listeners_and_controllers/inc)
target_include_directories(${PYTHON_PACKAGE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../observers_and_api/inc)
target_include_directories(${PYTHON_PACKAGE_NAME} SYSTEM PRIVATE $<BUILD_INTERFACE:${eigen_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} SYSTEM PRIVATE $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} SYSTEM PRIVATE $<BUILD_INTERFACE:${YAML_CPP_INCLUDE_DIRS}>)
target_include_directories(${PYTHON_PACKAGE_NAME} SYSTEM PRIVATE ${HDF5_INCLUDE_DIR})

target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE
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
    $<TARGET_OBJECTS:csv_writer_object>
    $<TARGET_OBJECTS:matrix_and_vector_classes_object>
    $<TARGET_OBJECTS:functors_for_optimizer_object>
    $<TARGET_OBJECTS:websocket_object>
    $<TARGET_OBJECTS:f2c_object>
    $<TARGET_OBJECTS:json_object>
    $<TARGET_OBJECTS:solver_object>
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
    $<TARGET_OBJECTS:test_data_generator>
    )

target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE yaml-cpp)
target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE gfortran)
target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE ${HDF5_CXX_STATIC_LIBRARY} ${HDF5_C_STATIC_LIBRARY})
target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE ${Boost_SYSTEM_LIBRARY})
target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE ${Boost_REGEX_LIBRARY})


IF(WIN32)
    target_link_libraries(${PYTHON_PACKAGE_NAME} PUBLIC wsock32 ws2_32)
ELSE()
    target_link_libraries(${PYTHON_PACKAGE_NAME} PUBLIC pthread)
ENDIF()

target_link_libraries(${PYTHON_PACKAGE_NAME} PRIVATE
     ${GRPC_GRPCPP_UNSECURE}
     ${PROTOBUF_LIBPROTOBUF}
     ${Boost_THREAD_LIBRARY}
   )
