# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST fmi_simulator)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/FMITest.cpp
              src/calculate_hashTest.cpp
              src/ParseFMIXmlTest.cpp
              src/random_FMI_XML.cpp
              src/EmitFMIXmlTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${GMOCK_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
#SET(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER})
ADD_EXECUTABLE(test_fmi src/test_fmi.c)
MESSAGE(FATAl_ERROR "GRPC_GRPCPP_UNSECURE=${GRPC_GRPCPP_UNSECURE}")
MESSAGE(FATAl_ERROR "PROTOBUF_LIBPROTOBUF=${PROTOBUF_LIBPROTOBUF}")
TARGET_LINK_LIBRARIES(test_fmi
                      ${Boost_FILESYSTEM_LIBRARY}
                      x-dyn
                      m
                      ${GRPC_GRPCPP_UNSECURE}
                      ${PROTOBUF_LIBPROTOBUF}
                      stdc++
                                            )
