PROJECT(observers_and_api_tests)
SET(SRC
    ConfBuilderTest.cpp
    CSVControllerTest.cpp # because it needs a Sim instance, which requires the observers_and_api include directory.
    EnvironmentTest.cpp
    EverythingObserverTest.cpp
    ForceTester.cpp
    ForceTests.cpp
    Hdf5ObserverTest.cpp
    Hdf5WaveObserverBuilderTest.cpp
    Hdf5WaveObserverTest.cpp
    JsonObserverTest.cpp
    JSONSerializerTest.cpp
    listenersTest.cpp
    ListOfObserversTest.cpp
    MapObserverTest.cpp
    ObserverTests.cpp
    PIDControllerTest.cpp # because it needs a Sim instance, which requires the observers_and_api include directory.
    SimTest.cpp
    SimulationServerObserverTest.cpp
    XdynForCSTest.cpp
    XdynForMETest.cpp
    )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

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
