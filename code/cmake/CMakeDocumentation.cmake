# Add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/DoxygenLayout.in ${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml @ONLY)
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc/dev_fr")
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc/dev_fr/html")
    MESSAGE(STATUS "Adding doc_doxygen target")
    ADD_CUSTOM_TARGET(doc_doxygen
        ${DOXYGEN_EXECUTABLE} Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
ELSE()
    MESSAGE("Doxygen not found.")
ENDIF()
