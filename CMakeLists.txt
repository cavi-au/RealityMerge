cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(godot LANGUAGES CXX DESCRIPTION "2D and 3D cross-platform game engine")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_GENERATOR_PLATFORM x64)

set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Godot's \"editor\" build -> \"RelWithDebInfo\"")

find_program(SCONS_PROGRAM scons)

if(NOT SCONS_PROGRAM)
    message(FATAL_ERROR "Cannot build ${PROJECT_NAME} without SCons.")
endif()

message(STATUS "SCons: ${SCONS_PROGRAM}")

if(WIN32)
    set(GODOT_PLATFORM "windows")
elseif(APPLE)
    set(GODOT_PLATFORM "macos")
else()
    set(GODOT_PLATFORM "linuxbsd")
endif()

set(GODOT_TARGET "editor")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(GODOT_TARGET "template_debug")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(GODOT_TARGET "template_release")
endif()

file(GLOB_RECURSE GODOT_SOURCES godot/*.c**)
file(GLOB_RECURSE GODOT_HEADERS godot/*.h**)
file(GLOB_RECURSE MODULE_SOURCES modules/*.c**)
file(GLOB_RECURSE MODULE_SOURCES modules/*.h**)

add_custom_target(${PROJECT_NAME}_scons
                  ALL
                  # COMMAND ${CMAKE_COMMAND} -E env CMAKE_COMMAND=${CMAKE_COMMAND} ${SCONS_PROGRAM} --clean
                  COMMAND ${CMAKE_COMMAND} -E env CMAKE_COMMAND=${CMAKE_COMMAND} ${SCONS_PROGRAM} target=${GODOT_TARGET} custom_modules=../modules platform=${GODOT_PLATFORM} showbuild=1 debug_symbols=yes dev_build=yes debug_crt=yes disable_exceptions=no
                  SOURCES ${GODOT_SOURCES} ${GODOT_HEADERS}
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/godot
                  VERBATIM
)

set_property(TARGET ${PROJECT_NAME}_scons
    APPEND
    PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir"
)

add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL ${GODOT_SOURCES} ${GODOT_HEADERS} ${MODULE_SOURCES} ${MODULE_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME}
                           SYSTEM PUBLIC
                           ${PROJECT_SOURCE_DIR}/godot
                           ${PROJECT_SOURCE_DIR}/godot/platform/${GODOT_PLATFORM}
                           ${PROJECT_SOURCE_DIR}/modules/reality_merge
                           ${PROJECT_SOURCE_DIR}/modules/reality_merge/thirdparty/cavi_usdj-am/include
                           ${PROJECT_SOURCE_DIR}/modules/reality_merge/thirdparty/cavi_usdj-am/build/include
                           ${PROJECT_SOURCE_DIR}/modules/reality_merge/thirdparty/automerge/rust/automerge-c/include
                           ${PROJECT_SOURCE_DIR}/modules/reality_merge/thirdparty/cavi_usdj-am/build/automerge-c/include
)
