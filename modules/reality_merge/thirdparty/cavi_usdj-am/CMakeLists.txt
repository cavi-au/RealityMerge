cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(cavi_usdj-am VERSION 0.0.1
                     LANGUAGES CXX
                     DESCRIPTION "A C++ USDJ-AM node adapter library")

set(CMAKE_VERBOSE_MAKEFILE ON)

include(CTest)

include(CMakePackageConfigHelpers)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(AUTOMERGE-C "automerge")

block(SCOPE_FOR VARIABLES)
    set(BUILD_SHARED_LIBS OFF)

    unset(LIBRARY_NAME CACHE)
    
    add_subdirectory("../automerge/rust/automerge-c" "${PROJECT_BINARY_DIR}/automerge-c")
endblock()

set(DEFAULT_LIBRARY_NAME ${PROJECT_NAME})

if(NOT DEFINED LIBRARY_NAME)
    set(LIBRARY_NAME "${DEFAULT_LIBRARY_NAME}")
endif()

if(NOT DEFINED STATIC_LIBRARY_PREFIX)
    set(STATIC_LIBRARY_PREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
endif()

if(NOT DEFINED STATIC_LIBRARY_SUFFIX)
    set(STATIC_LIBRARY_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

IF(NOT DEFINED SHARED_LIBRARY_PREFIX)
    set(SHARED_LIBRARY_PREFIX "${CMAKE_SHARED_LIBRARY_PREFIX}")
endif()

if(NOT DEFINED SHARED_LIBRARY_SUFFIX)
    set(SHARED_LIBRARY_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

option(BUILD_SHARED_LIBS "Enable the choice of a shared or static library.")

add_library(${LIBRARY_NAME})

target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_17)

target_include_directories(${LIBRARY_NAME}
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>"
           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(${LIBRARY_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "TRUE")

add_dependencies(${LIBRARY_NAME} ${AUTOMERGE-C})

if(BUILD_SHARED_LIBS)
    target_link_libraries(${LIBRARY_NAME} PUBLIC "$<LINK_LIBRARY:WHOLE_ARCHIVE,${AUTOMERGE-C}>")

    # Enable an external build tool to find the shared library in the root of the
    # out-of-source build directory when it has overridden an aspect of its name.
    if(NOT (("${SHARED_LIBRARY_PREFIX}" STREQUAL "${CMAKE_SHARED_LIBRARY_PREFIX}") AND
            ("${LIBRARY_NAME}" STREQUAL "${DEFAULT_LIBRARY_NAME}") AND
            ("${SHARED_LIBRARY_SUFFIX}" STREQUAL "${CMAKE_SHARED_LIBRARY_SUFFIX}")))
        add_custom_command(
            TARGET ${LIBRARY_NAME}
            POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E echo "Copying \"${PROJECT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}\" to \"${PROJECT_BINARY_DIR}/${SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${SHARED_LIBRARY_SUFFIX}\"..."
            COMMAND
                ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX} ${PROJECT_BINARY_DIR}/${SHARED_LIBRARY_PREFIX}${LIBRARY_NAME}${SHARED_LIBRARY_SUFFIX}
            WORKING_DIRECTORY
                ${PROJECT_SOURCE_DIR}
            COMMENT
                "Aliasing the library for the external build tool..."
            VERBATIM
        )
    endif()
else()
    target_link_libraries(${LIBRARY_NAME} PUBLIC ${AUTOMERGE-C})

    if(WIN32)
        find_program(LIB_TOOL "lib" REQUIRED)

        add_custom_command(
            TARGET ${LIBRARY_NAME}
            POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E echo "Merging its dependent libraries into \"$<TARGET_FILE:${LIBRARY_NAME}>\"..."
            COMMAND
                ${LIB_TOOL} /OUT:$<TARGET_FILE_NAME:${LIBRARY_NAME}> $<TARGET_FILE_NAME:${LIBRARY_NAME}> $<TARGET_FILE:${AUTOMERGE-C}>
            WORKING_DIRECTORY
                ${PROJECT_BINARY_DIR}
            COMMENT
                "Merging the libraries..."
            VERBATIM
        )
    else()
        if (APPLE)
            set(LTO_PLUGIN_OPT "")
        else()
            set(LTO_PLUGIN_OPT "--plugin /usr/lib/bfd-plugins/liblto_plugin.so")
        endif()
        set(OBJECTS_DIR objects)

        set(AUTOMERGE-C_OBJECTS_DIR ${OBJECTS_DIR}/$<TARGET_NAME:${AUTOMERGE-C}>)

        add_custom_command(
            TARGET "${LIBRARY_NAME}"
            POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E echo "Merging its dependent libraries into \"$<TARGET_FILE:${LIBRARY_NAME}>\"..."
            COMMAND
                ${CMAKE_COMMAND} -E rm -rf ${OBJECTS_DIR}
            COMMAND
                ${CMAKE_COMMAND} -E make_directory ${AUTOMERGE-C_OBJECTS_DIR}
            COMMAND
                ${CMAKE_COMMAND} -E echo "${CMAKE_AR} -x  ${LTO_PLUGIN_OPT} $<TARGET_FILE:${AUTOMERGE-C}>"
            COMMAND
                ${CMAKE_COMMAND} -E chdir ${AUTOMERGE-C_OBJECTS_DIR} ${CMAKE_AR} -x ${LTO_PLUGIN_OPT} $<TARGET_FILE:${AUTOMERGE-C}>
            COMMAND
                ${CMAKE_COMMAND} -E echo "${CMAKE_AR} -rs ${LTO_PLUGIN_OPT} $<TARGET_FILE_NAME:${LIBRARY_NAME}> ${AUTOMERGE-C_OBJECTS_DIR}/*.o"
            COMMAND
                ${CMAKE_AR} -rs ${LTO_PLUGIN_OPT} $<TARGET_FILE_NAME:${LIBRARY_NAME}> ${AUTOMERGE-C_OBJECTS_DIR}/*.o
            WORKING_DIRECTORY
                ${PROJECT_BINARY_DIR}
            COMMENT
                "Merging the libraries' constituent object files..."
        )
    endif()
endif()

if(WIN32 OR NOT BUILD_SHARED_LIBS)
    # Enable an external build tool to find the static/import library in the
    # root of the out-of-source build directory when it has overridden an aspect
    # of its name.
    if(NOT (("${STATIC_LIBRARY_PREFIX}" STREQUAL "${CMAKE_STATIC_LIBRARY_PREFIX}") AND
            ("${LIBRARY_NAME}" STREQUAL "${DEFAULT_LIBRARY_NAME}") AND
            ("${STATIC_LIBRARY_SUFFIX}" STREQUAL "${CMAKE_STATIC_LIBRARY_SUFFIX}")))
        add_custom_command(
            TARGET ${LIBRARY_NAME}
            POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E echo "Copying \"${PROJECT_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}\" to \"${PROJECT_BINARY_DIR}/${STATIC_LIBRARY_PREFIX}${LIBRARY_NAME}${STATIC_LIBRARY_SUFFIX}\"..."
            COMMAND
                ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBRARY_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} ${PROJECT_BINARY_DIR}/${STATIC_LIBRARY_PREFIX}${LIBRARY_NAME}${STATIC_LIBRARY_SUFFIX}
            WORKING_DIRECTORY
                ${PROJECT_SOURCE_DIR}
            COMMENT
                "Aliasing the static/import library for the external build tool..."
            VERBATIM
        )
    endif()
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
    ${PROJECT_SOURCE_DIR}/cmake/config.hpp.in
    ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/config.hpp
    @ONLY
    NEWLINE_STYLE LF
)

target_sources(${LIBRARY_NAME}
    PRIVATE
        src/array_iterator.cpp
        src/array_range.cpp
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
        src/usd/token_type.cpp
        src/usd/geom/token_type.cpp
        src/usd/geom/xform_op_type.cpp
        src/usd/physics/token_type.cpp
        src/usd/sdf/value_type_name.cpp
        src/utils/bytes.cpp
        src/utils/document.cpp
        src/utils/item.cpp
        src/utils/json_writer.cpp
    PUBLIC
        FILE_SET api TYPE HEADERS
            BASE_DIRS
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am
            FILES
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/array_iterator.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/array_range.hpp
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
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/usd/token_type.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/usd/physics/token_type.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/usd/geom/token_type.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/usd/geom/xform_op_type.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/usd/sdf/value_type_name.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/bytes.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/document.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/item.hpp
                ${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/utils/json_writer.hpp
    INTERFACE
        FILE_SET config TYPE HEADERS
            BASE_DIRS
                ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am
            FILES
                ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/cavi/usdj_am/config.hpp
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

if(BUILD_TESTING)
    add_subdirectory(test EXCLUDE_FROM_ALL)

    enable_testing()
endif()
