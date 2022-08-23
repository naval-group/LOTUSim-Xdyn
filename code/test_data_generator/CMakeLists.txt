CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(test_data_generator)

SET(SRC
    big_hdb.cpp
    bug_3185.cpp
    bug_3187.cpp
    bug_3207.cpp
    bug_3235.cpp
    bug_3238.cpp
    bug_3241.cpp
    hdb_data_bug_3230.cpp
    hdb_data.cpp
    hdb_data_issue_184.cpp
    issue_20.cpp
    precal_test_data.cpp
    stl_data.cpp
    TriMeshTestData.cpp
    yaml_data.cpp
    yml_data_bug_3230.cpp
    )

INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to ${PROJECT_NAME}'s include directory")
