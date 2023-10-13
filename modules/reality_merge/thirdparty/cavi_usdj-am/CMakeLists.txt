cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(cavi_usdj_am VERSION 0.0.1
                     LANGUAGES CXX
                     DESCRIPTION "A C++ USDJ-AM node proxy library")
                     
include(CTest)

include(CMakePackageConfigHelpers)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(BUILD_SHARED_LIBS "Enable the choice of a shared or static library.")

set(DEFAULT_LIBRARY_NAME ${PROJECT_NAME})

set(LIBRARY_NAME "${DEFAULT_LIBRARY_NAME}" CACHE STRING "The library's name.")

set(LIBRARY_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}" CACHE STRING "An output library's prefix.")

set(LIBRARY_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}" CACHE STRING "An output library's suffix.")

add_library(${LIBRARY_NAME})

if(WIN32)
    target_compile_definitions(${LIBRARY_NAME} PRIVATE _SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING)
endif()

target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_17)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC automerge_core
           automerge
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>"
           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_dependencies(${LIBRARY_NAME} automerge_core automerge)

# Enable an external build tool to find the library in the root of the
# out-of-source build directory when it has overridden an aspect of its name.
if(NOT (("${LIBRARY_PREFIX}" STREQUAL "${CMAKE_STATIC_LIBRARY_PREFIX}") AND
        ("${LIBRARY_NAME}" STREQUAL "${DEFAULT_LIBRARY_NAME}") AND
        ("${LIBRARY_SUFFIX}" STREQUAL "${CMAKE_STATIC_LIBRARY_SUFFIX}")))
    add_custom_command(
        TARGET ${LIBRARY_NAME}
        POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E echo "Copying \"${CMAKE_STATIC_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}\" to \"${CMAKE_BINARY_DIR}/${LIBRARY_PREFIX}${LIBRARY_NAME}${LIBRARY_SUFFIX}\"..."
        COMMAND
            ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_BINARY_DIR}/${LIBRARY_PREFIX}${LIBRARY_NAME}${LIBRARY_SUFFIX}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT
            "Aliasing the library for the external build tool..."
        VERBATIM
    )
endif()

# Generate the configuration header.
string(MAKE_C_IDENTIFIER ${PROJECT_NAME} SYMBOL_PREFIX)

string(TOUPPER ${SYMBOL_PREFIX} SYMBOL_PREFIX)

math(EXPR INTEGER_PROJECT_VERSION_MAJOR "${PROJECT_VERSION_MAJOR} * 100000")

math(EXPR INTEGER_PROJECT_VERSION_MINOR "${PROJECT_VERSION_MINOR} * 100")

math(EXPR INTEGER_PROJECT_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

math(EXPR INTEGER_PROJECT_VERSION "${INTEGER_PROJECT_VERSION_MAJOR} + \
                                   ${INTEGER_PROJECT_VERSION_MINOR} + \
                                   ${INTEGER_PROJECT_VERSION_PATCH}")

configure_file(
    ${CMAKE_MODULE_PATH}/config.hpp.in
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/config.hpp
    @ONLY
    NEWLINE_STYLE LF
)

target_sources(${LIBRARY_NAME}
    PRIVATE
        src/assignment.cpp
        src/assignment_keyword.cpp
        src/class_declaration.cpp
        src/class_definition.cpp
        src/declaration.cpp
        src/declaration_keyword.cpp
        src/definition.cpp
        src/definition_statement.cpp
        src/definition_type.cpp
        src/descriptor.cpp
        src/external_reference.cpp
        src/external_reference_import.cpp
        src/file.cpp
        src/node.cpp
        src/number.cpp
        src/object_declaration.cpp
        src/object_declarations.cpp
        src/object_declaration_entries.cpp
        src/object_declaration_list.cpp
        src/object_declaration_list_value.cpp
        src/object_value.cpp
        src/reference_file.cpp
        src/statement.cpp
        src/statement_type.cpp
        src/string_.cpp
        src/value.cpp
        src/value_type.cpp
        src/variant_definition.cpp
        src/variant_set.cpp
        src/visitor.cpp
        src/utils/bytes.cpp
        src/utils/document.cpp
        src/utils/item.cpp
        src/utils/json_writer.cpp
    PUBLIC
        FILE_SET api TYPE HEADERS
            BASE_DIRS
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am
            FILES
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/assignment.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/assignment_keyword.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/class_declaration.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/class_definition.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/declaration.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/declaration_keyword.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/definition.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/definition_statement.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/definition_type.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/descriptor.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/external_reference.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/external_reference_import.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/file.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/input_iterator.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/input_range.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/node.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/number.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/object_declaration.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/object_declarations.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/object_declaration_entries.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/object_declaration_list.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/object_declaration_list_value.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/object_value.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/reference_file.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/statement.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/statement_type.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/string_.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/value.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/value_type.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/variant_definition.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/variant_set.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/visitor.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/bytes.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/document.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/item.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/json_writer.hpp
    INTERFACE
        FILE_SET config TYPE HEADERS
            BASE_DIRS
                ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am
            FILES
                ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/config.hpp
)

install(
    TARGETS ${LIBRARY_NAME}
    EXPORT ${PROJECT_NAME}-config
    FILE_SET api
    FILE_SET config
)

install(EXPORT ${PROJECT_NAME}-config
        FILE ${PROJECT_NAME}-config.cmake
        NAMESPACE "${PROJECT_NAME}::"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB}
)

add_subdirectory("../automerge/rust/automerge-c" "${CMAKE_CURRENT_BINARY_DIR}/automerge-c")

if(BUILD_TESTING)
    add_subdirectory(test EXCLUDE_FROM_ALL)
endif()
