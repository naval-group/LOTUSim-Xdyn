CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(mesh)

SET(SRC
    Mesh.cpp
    MeshBuilder.cpp
    MeshIntersector.cpp
    mesh_manipulations.cpp
    CenterOfMass.cpp
    ClosingFacetComputer.cpp
    2DMeshDisplay.cpp
    )

INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

ADD_SUBDIRECTORY(unit_tests)
