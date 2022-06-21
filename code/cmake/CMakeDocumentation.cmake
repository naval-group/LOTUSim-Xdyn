# Add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(XmlTransform)

IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/DoxygenLayout.in ${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml @ONLY)
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc/dev_fr")
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc/dev_fr/html")
    ADD_CUSTOM_TARGET(test_results
        COMMAND ./${TEST_EXE} --gtest_output=xml:test_output.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running all tests" VERBATIM
        DEPENDS ${TEST_EXE}
    )

    IF(WIN32 AND XMLTRANSFORM_FOUND)
        MESSAGE(STATUS "Adding doc_dev_fr target WITH dependency on target functionalities")
        ADD_CUSTOM_TARGET(doc_dev_fr
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            DEPENDS functionalities
            )
    ELSE()
        MESSAGE(STATUS "Adding doc_dev_fr target WITHOUT dependency on target functionalities (No XML transform was found)")
        ADD_CUSTOM_TARGET(doc_dev_fr
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
    ENDIF()
ELSE()
    MESSAGE("Doxygen not found.")
ENDIF()
