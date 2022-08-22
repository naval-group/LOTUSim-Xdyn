SET(MODULE_UNDER_TEST observers_and_api)
PROJECT(${MODULE_UNDER_TEST}_tests)
SET(SRC
    ListOfObserversTest.cpp
    Hdf5ObserverTest.cpp
    Hdf5WaveObserverTest.cpp
    Hdf5WaveObserverBuilderTest.cpp
    JsonObserverTest.cpp
    SimTest.cpp
    ForceTester.cpp
    ForceTests.cpp
    MapObserverTest.cpp
    ObserverTests.cpp
    PIDControllerTest.cpp # because it needs a Sim instance, which requires the observers_and_api include directory.
    CSVControllerTest.cpp # because it needs a Sim instance, which requires the observers_and_api include directory.
    ConfBuilderTest.cpp
    XdynForCSTest.cpp
    XdynForMETest.cpp
    EverythingObserverTest.cpp
    JSONSerializerTest.cpp
    CoSimulationObserverTest.cpp
    listenersTest.cpp
    )

INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})

IF(WIN32)
    IF(${CMAKE_SYSTEM_VERSION} STRLESS 6.0)
        MESSAGE(STATUS "Modification of compilation option to prevent a crashing bug")
        MESSAGE(STATUS "    CMAKE_SYSTEM_VERSION ${CMAKE_SYSTEM_VERSION} -> Running on Windows XP")
        MESSAGE(STATUS "    There is a crashing bug when using optimized compilation on Windows XP with MinGW, Boost Asio and WebSocketPP")
        MESSAGE(STATUS "    This bug therefore appears in RELEASE mode (-O3)")
        MESSAGE(STATUS "    Apparently, the bug is located in a thread, needed to create WebSocketPP tests requiring a threaded server and a client")
        MESSAGE(STATUS "    To fix this bug, one forces the -O0 compilation option for all files that uses thread")
        MESSAGE(STATUS "    ")
        SET_SOURCE_FILES_PROPERTIES(src/WebSocketObserverTest.cpp PROPERTIES COMPILE_FLAGS -O0)
    ENDIF()
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
