CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(hdb_interpolators)

SET(SRC
    HDBParser.cpp
    RadiationDampingBuilder.cpp
    History.cpp
    PrecalParserHelper.cpp
    PrecalParser.cpp
    RaoInterpolator.cpp
    HydroDBParser.cpp
    )

INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

ADD_SUBDIRECTORY(unit_tests)
