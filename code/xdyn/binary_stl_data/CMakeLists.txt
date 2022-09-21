CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(binary_stl_data)

SET(SRC
    generate_test_ship.cpp
    )

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # Otherwise CLang generates fatal error: string literal of length 253876 exceeds maximum length 65536 that C++ compilers are required to support
    SET(CMAKE_C_FLAGS "-Werror -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wmissing-declarations -Wmissing-format-attribute -Wsign-compare -Wstrict-aliasing=2 -Wundef -Wconversion")
    SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Woverloaded-virtual -Weffc++ -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros")
ENDIF()

INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${base91x_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
