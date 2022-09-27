CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(boost_program_options_descriptions)

SET(SRC
    CustomOptionDescription.cpp
    OptionPrinter.cpp
    )

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${SRC})
