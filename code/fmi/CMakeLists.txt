CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(fmi)

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${observers_and_api_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${observers_and_api_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})

CONFIGURE_FILE(src/get_sha.cpp "${CMAKE_CURRENT_BINARY_DIR}/get_sha.cpp" @ONLY)

set(SRC src/FMI.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/get_sha.cpp
        src/calculate_sha1.cpp
        src/FMIXml.cpp
        src/ParseFMIXml.cpp
        src/EmitFMIXml.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF(NOT(MSVC))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

CONFIGURE_FILE(src/generate_fmi_xml.cpp "${CMAKE_CURRENT_BINARY_DIR}/generate_fmi_xml.cpp" @ONLY)
ADD_EXECUTABLE(generate_fmi_xml ${CMAKE_CURRENT_BINARY_DIR}/generate_fmi_xml.cpp)

TARGET_LINK_LIBRARIES(generate_fmi_xml x-dyn
                      ${GRPC_GRPCPP_UNSECURE}
                      ${PROTOBUF_LIBPROTOBUF})

INSTALL(TARGETS generate_fmi_xml
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
ADD_SUBDIRECTORY(unit_tests)
