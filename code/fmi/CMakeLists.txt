cmake_minimum_required(VERSION 2.8.8)
project(fmi)

set(SRC src/FMI.cpp
        )


include_directories(inc)
include_directories(${ssc_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} x-dyn)

add_subdirectory(unit_tests)
