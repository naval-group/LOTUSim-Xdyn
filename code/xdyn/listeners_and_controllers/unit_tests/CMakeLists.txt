PROJECT(listeners_and_controllers_tests)
SET(SRC
    GrpcControllerTest.cpp
    CSVLineByLineReaderTest.cpp
    )

# Tests for Controller.cpp are found in:
# - observers_and_api/unit_tests/PIDControllerTest.cpp
# because these tests need a Sim instance, which requires the observers_and_api include directory.
# To avoid cross-dependencies, the corresponding unit tests are moved to observers_and_api/unit_tests.

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
