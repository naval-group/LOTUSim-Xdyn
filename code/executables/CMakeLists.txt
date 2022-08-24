CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(executables)

INCLUDE_DIRECTORIES(${get_git_sha_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${boost_program_options_descriptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${force_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${listeners_and_controllers_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${observers_and_api_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${gz_curves_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${interface_hdf5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${grpc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${base91x_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_hdf5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${HDF5_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${GRPC_INCLUDE_DIR})

SET(cosimulation_proto ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/proto/cosimulation.proto)
SET(cosimulation_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.pb.cc")
SET(cosimulation_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.pb.h")
SET(cosimulation_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.grpc.pb.cc")
SET(cosimulation_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.grpc.pb.h")
GET_FILENAME_COMPONENT(proto_path "${cosimulation_proto}" PATH)
ADD_CUSTOM_COMMAND(
    OUTPUT "${cosimulation_proto_srcs}" "${cosimulation_proto_hdrs}" "${cosimulation_grpc_srcs}" "${cosimulation_grpc_hdrs}"
    COMMAND ${PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         -I "${PROTOC_PREFIX}${proto_path}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
         "${PROTOC_PREFIX}${cosimulation_proto}"
    DEPENDS "${cosimulation_proto}")

SET(model_exchange_proto ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/proto/model_exchange.proto)
SET(model_exchange_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.pb.cc")
SET(model_exchange_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.pb.h")
SET(model_exchange_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.grpc.pb.cc")
SET(model_exchange_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.grpc.pb.h")
ADD_CUSTOM_COMMAND(
    OUTPUT "${model_exchange_proto_srcs}" "${model_exchange_proto_hdrs}" "${model_exchange_grpc_srcs}" "${model_exchange_grpc_hdrs}"
    COMMAND ${PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         -I "${PROTOC_PREFIX}${proto_path}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
         "${PROTOC_PREFIX}${model_exchange_proto}"
    DEPENDS "${model_exchange_proto}")

ADD_EXECUTABLE(xdyn
    display_command_line_arguments.cpp
    parse_XdynCommandLineArguments.cpp
    build_observers_description.cpp
    XdynCommandLineArguments.cpp
    ErrorReporter.cpp
    xdyn.cpp
    )

TARGET_LINK_LIBRARIES(xdyn
    x-dyn
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    boost_program_options_descriptions_static
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    ${Boost_THREAD_LIBRARY}
    )

ADD_EXECUTABLE(test_orbital_velocities_and_dynamic_pressures
    test_orbital_velocities_and_dynamic_pressures.cpp
    )

TARGET_LINK_LIBRARIES(test_orbital_velocities_and_dynamic_pressures
    x-dyn
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    )

ADD_EXECUTABLE(test_hs
    test_hs.cpp
    $<TARGET_OBJECTS:test_data_generator>
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:listeners_and_controllers>
    $<TARGET_OBJECTS:force_models>
    $<TARGET_OBJECTS:environment_models>
    $<TARGET_OBJECTS:external_file_formats>
    $<TARGET_OBJECTS:mesh>
    $<TARGET_OBJECTS:yaml_parser>
    $<TARGET_OBJECTS:external_data_structures>
    )

TARGET_LINK_LIBRARIES(test_hs
    x-dyn
    binary_stl_data_static
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    yaml-cpp
    gfortran
    )

ADD_EXECUTABLE(yml2test yml2test.cpp)

ADD_EXECUTABLE(quat2eul convert_quaternion_to_euler.cpp
    $<TARGET_OBJECTS:kinematics_object>
    $<TARGET_OBJECTS:data_source_object>
    $<TARGET_OBJECTS:exception_handling_object>
    $<TARGET_OBJECTS:numeric_object>
    )

ADD_EXECUTABLE(convert_stl_files_to_code convert_stl_files_to_code.cpp)

TARGET_LINK_LIBRARIES(convert_stl_files_to_code
    x-dyn
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    boost_program_options_descriptions_static
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    )

ADD_EXECUTABLE(gz
    gz.cpp
    display_command_line_arguments.cpp
    parse_XdynCommandLineArguments.cpp
    XdynCommandLineArguments.cpp
    ErrorReporter.cpp
    build_observers_description.cpp
    )
TARGET_LINK_LIBRARIES(gz
    x-dyn
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    boost_program_options_descriptions_static
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    ${Boost_THREAD_LIBRARY}
    )

ADD_EXECUTABLE(generate_yaml_example
        generate_yaml_examples.cpp file_writer.cpp
        $<TARGET_OBJECTS:test_data_generator>
        )

ADD_EXECUTABLE(generate_stl_examples
    generate_stl_examples.cpp
    file_writer.cpp
    $<TARGET_OBJECTS:test_data_generator>
    )
TARGET_LINK_LIBRARIES(generate_stl_examples
    binary_stl_data_static
    x-dyn
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    )

ADD_EXECUTABLE(xdyn-for-cs
    display_command_line_arguments.cpp
    parse_XdynForCSCommandLineArguments.cpp
    build_observers_description.cpp
    XdynForCSCommandLineArguments.cpp
    XdynCommandLineArguments.cpp
    xdyn_for_cs.cpp
    CosimulationServiceImpl.cpp
    ErrorReporter.cpp
    gRPCChecks.cpp
    "${cosimulation_proto_srcs}"
    "${cosimulation_grpc_srcs}"
    )
TARGET_LINK_LIBRARIES(xdyn-for-cs
    x-dyn
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    boost_program_options_descriptions_static
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    ${Boost_THREAD_LIBRARY}
    )

IF((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang"))
    # MESSAGE(STATUS "Remove the -Wc++17-attribute-extensions option for ${PROJECT_NAME}")
    # fatal error: use of the 'nodiscard' attribute is a C++17 extension [-Wc++17-attribute-extensions]
    # PROTOBUF_NODISCARD ::CosimulationStatesQuaternion* release_states();
    # google/protobuf/port_def.inc:463:30: note: expanded from macro 'PROTOBUF_NODISCARD'
    #define PROTOBUF_NODISCARD [[nodiscard]]
    TARGET_COMPILE_OPTIONS(xdyn-for-cs PRIVATE -Wno-c++17-extensions)
    # gRPC code: fatal error: implicit conversion changes signedness: 'unsigned int' to 'int' [-Wsign-conversion]
    TARGET_COMPILE_OPTIONS(xdyn-for-cs PRIVATE -Wno-sign-conversion)
ENDIF()
IF((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    TARGET_COMPILE_OPTIONS(xdyn-for-cs PRIVATE -Wno-effc++)
ENDIF()

ADD_EXECUTABLE(xdyn-for-me
    display_command_line_arguments.cpp
    parse_XdynForMECommandLineArguments.cpp
    build_observers_description.cpp
    XdynForMECommandLineArguments.cpp
    xdyn_for_me.cpp
    ModelExchangeServiceImpl.cpp
    ErrorReporter.cpp
    gRPCChecks.cpp
    "${model_exchange_proto_srcs}"
    "${model_exchange_grpc_srcs}"
    )
TARGET_LINK_LIBRARIES(xdyn-for-me
    x-dyn
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    boost_program_options_descriptions_static
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    ${Boost_THREAD_LIBRARY}
    )

IF((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang"))
    # MESSAGE(STATUS "Remove the -Wc++17-attribute-extensions option for ${PROJECT_NAME}")
    # fatal error: use of the 'nodiscard' attribute is a C++17 extension [-Wc++17-attribute-extensions]
    # PROTOBUF_NODISCARD ::CosimulationStatesQuaternion* release_states();
    # google/protobuf/port_def.inc:463:30: note: expanded from macro 'PROTOBUF_NODISCARD'
    #define PROTOBUF_NODISCARD [[nodiscard]]
    TARGET_COMPILE_OPTIONS(xdyn-for-me PRIVATE -Wno-c++17-extensions)
    # gRPC code: fatal error: implicit conversion changes signedness: 'unsigned int' to 'int' [-Wsign-conversion]
    TARGET_COMPILE_OPTIONS(xdyn-for-me PRIVATE -Wno-sign-conversion)
ENDIF()
IF((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    TARGET_COMPILE_OPTIONS(xdyn-for-me PRIVATE -Wno-effc++)
ENDIF()

ADD_EXECUTABLE(xdyn-grpc-airy
    xdyn_grpc_airy.cpp
    display_command_line_arguments.cpp
    parse_XdynCommandLineArguments.cpp
    XdynCommandLineArguments.cpp
    ErrorReporter.cpp
    gRPCChecks.cpp
    $<TARGET_OBJECTS:grpc_airy>
    $<TARGET_OBJECTS:test_data_generator>
    )
TARGET_LINK_LIBRARIES(xdyn-grpc-airy
    x-dyn
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    boost_program_options_descriptions_static
    ${GRPC_GRPCPP_UNSECURE}
    ${PROTOBUF_LIBPROTOBUF}
    ${Boost_THREAD_LIBRARY}
    )
################################################################################
INSTALL(TARGETS xdyn
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS xdyn-for-cs
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS gz
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS xdyn-for-me
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS xdyn-grpc-airy
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/demos")

IF(WIN32)
    ADD_CUSTOM_COMMAND(
        TARGET generate_yaml_example
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:x-dyn> .
        COMMAND generate_yaml_example demos
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS x-dyn
        COMMENT "Generate YAML simulation input data files")
    ADD_CUSTOM_COMMAND(
        TARGET generate_stl_examples
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:x-dyn> .
        COMMAND generate_stl_examples demos
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS x-dyn
        COMMENT "Generate STL mesh input data files")
ELSE()
    ADD_CUSTOM_COMMAND(
        TARGET generate_yaml_example
        POST_BUILD
        COMMAND generate_yaml_example demos
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS x-dyn
        COMMENT "Generate YAML simulation input data files")
    ADD_CUSTOM_COMMAND(
        TARGET generate_stl_examples
        POST_BUILD
        COMMAND generate_stl_examples demos
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS x-dyn
        COMMENT "Generate STL mesh input data files")
ENDIF()

INSTALL(
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/Python/plot.py
        ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/Python/animate.py
        ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/Python/waveYamlToCsv.py
    DESTINATION demos)

INSTALL(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/demos
    DESTINATION ".")
