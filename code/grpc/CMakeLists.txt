CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(grpc)

SET(SRC
    SurfaceElevationFromGRPC.cpp
    GRPCForceModel.cpp
    ToGRPC.cpp
    ToGRPCCommon.cpp
    FromGRPC.cpp
    GrpcControllerInterface.cpp
    )

INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${listeners_and_controllers_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS}) # For MeshIntersector
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS}) # For History
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${GRPC_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

# gRPC
SET(wave_proto_types ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/proto/wave_types.proto)
GET_FILENAME_COMPONENT(proto_path "${wave_proto_types}" ABSOLUTE)
GET_FILENAME_COMPONENT(proto_path "${proto_path}" DIRECTORY)

SET(wave_proto_srcs_t "${CMAKE_CURRENT_BINARY_DIR}/wave_types.pb.cc")
SET(wave_proto_hdrs_t "${CMAKE_CURRENT_BINARY_DIR}/wave_types.pb.h")
SET(wave_grpc_srcs_t "${CMAKE_CURRENT_BINARY_DIR}/wave_types.grpc.pb.cc")
SET(wave_grpc_hdrs_t "${CMAKE_CURRENT_BINARY_DIR}/wave_types.grpc.pb.h")
ADD_CUSTOM_COMMAND(
    OUTPUT "${wave_proto_srcs_t}" "${wave_proto_hdrs_t}" "${wave_grpc_srcs_t}" "${wave_grpc_hdrs_t}"
    COMMAND ${PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         -I "${PROTOC_PREFIX}${proto_path}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
         "wave_types.proto"
    DEPENDS "${waves_proto_types}")

SET(wave_proto_grpc ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/proto/wave_grpc.proto)
SET(wave_proto_srcs_g "${CMAKE_CURRENT_BINARY_DIR}/wave_grpc.pb.cc")
SET(wave_proto_hdrs_g "${CMAKE_CURRENT_BINARY_DIR}/wave_grpc.pb.h")
SET(wave_grpc_srcs_g "${CMAKE_CURRENT_BINARY_DIR}/wave_grpc.grpc.pb.cc")
SET(wave_grpc_hdrs_g "${CMAKE_CURRENT_BINARY_DIR}/wave_grpc.grpc.pb.h")
ADD_CUSTOM_COMMAND(
    OUTPUT "${wave_proto_srcs_g}" "${wave_proto_hdrs_g}" "${wave_grpc_srcs_g}" "${wave_grpc_hdrs_g}"
    COMMAND ${PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         -I "${PROTOC_PREFIX}${proto_path}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
         "wave_grpc.proto"
    DEPENDS "${wave_proto_grpc}")

SET(force_proto ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/proto/force.proto)
SET(force_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/force.pb.cc")
SET(force_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/force.pb.h")
SET(force_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/force.grpc.pb.cc")
SET(force_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/force.grpc.pb.h")
ADD_CUSTOM_COMMAND(
    OUTPUT "${force_proto_srcs}" "${force_proto_hdrs}" "${force_grpc_srcs}" "${force_grpc_hdrs}"
    COMMAND ${PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
         -I "${PROTOC_PREFIX}${proto_path}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
         "force.proto"
    DEPENDS "${force_proto}")

SET(controller_proto ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/proto/controller.proto)
SET(controller_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/controller.pb.cc")
SET(controller_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/controller.pb.h")
SET(controller_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/controller.grpc.pb.cc")
SET(controller_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/controller.grpc.pb.h")
ADD_CUSTOM_COMMAND(
    OUTPUT "${controller_proto_srcs}" "${controller_proto_hdrs}" "${controller_grpc_srcs}" "${controller_grpc_hdrs}"
    COMMAND ${PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
            --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
            -I "${PROTOC_PREFIX}${proto_path}"
            --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
            "controller.proto"
    DEPENDS "${controller_proto}")

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
ADD_LIBRARY(${PROJECT_NAME}_gen OBJECT
    ${wave_proto_srcs_t}
    ${wave_grpc_srcs_t}
    ${wave_proto_srcs_g}
    ${wave_grpc_srcs_g}
    ${force_proto_srcs}
    ${force_grpc_srcs}
    ${controller_proto_srcs}
    ${controller_grpc_srcs}
    )
SET(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

IF((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang"))
    MESSAGE(STATUS "Remove the -Wc++17-attribute-extensions option for ${PROJECT_NAME}")
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME}_gen PRIVATE -Wno-c++17-extensions)
    # gRPC code: fatal error: implicit conversion changes signedness: 'unsigned int' to 'int' [-Wsign-conversion]
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME}_gen PRIVATE -Wno-sign-conversion)
ENDIF()
IF((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME}_gen PRIVATE -Wno-effc++)
ENDIF()

ADD_SUBDIRECTORY(unit_tests)

ADD_LIBRARY(${PROJECT_NAME}_airy OBJECT AiryGRPC.cpp)
ADD_CUSTOM_TARGET(genhdr_grpc_airy DEPENDS
    ${wave_proto_hdrs_t}
    ${wave_grpc_hdrs_t}
    ${wave_proto_hdrs_g}
    ${wave_grpc_hdrs_g})
ADD_DEPENDENCIES(${PROJECT_NAME}_airy genhdr_grpc_airy)
