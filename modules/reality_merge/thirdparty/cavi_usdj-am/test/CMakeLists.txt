cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

find_package(Catch2 CONFIG REQUIRED)

add_executable(
    ${LIBRARY_NAME}_test
        main.cpp
)

set_target_properties(${LIBRARY_NAME}_test PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${LIBRARY_NAME}_test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain ${LIBRARY_NAME})

add_custom_command(
    TARGET ${LIBRARY_NAME}_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_CURRENT_SOURCE_DIR}/files ${CMAKE_CURRENT_BINARY_DIR}/files
    COMMENT "Copying the test input files into the tests directory..."
)

if(BUILD_SHARED_LIBS AND WIN32)
    add_custom_command(
        TARGET ${LIBRARY_NAME}_test
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LIBRARY_NAME}> $<TARGET_FILE_DIR:${LIBRARY_NAME}_test>
        COMMENT "Copying the DLL into the tests directory..."
        VERBATIM
    )
endif()

add_test(NAME ${LIBRARY_NAME}_test COMMAND ${LIBRARY_NAME}_test)

add_custom_command(
    TARGET ${LIBRARY_NAME}_test
    POST_BUILD
    COMMAND
        ${CMAKE_CTEST_COMMAND} --config $<CONFIG> --output-on-failure
    COMMENT
        "Running the test(s)..."
    VERBATIM
)
