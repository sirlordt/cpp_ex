# Enable Address Sanitizer and Undefined Behavior Sanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")

# Create test executable
add_executable(integration_tests integration_tests.cpp)

# Link with Catch2 and required libraries
target_link_libraries(integration_tests PRIVATE
    Catch2::Catch2WithMain
    pthread
)

# Link with try_catch_guard if available
if(TRY_CATCH_GUARD_AVAILABLE)
    target_link_libraries(integration_tests PRIVATE try_catch_guard)
    target_compile_definitions(integration_tests PRIVATE TRY_CATCH_GUARD_AVAILABLE)
endif()

# Include directories
target_include_directories(integration_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Define CATCH_CONFIG_NO_POSIX_SIGNALS as an extra safety measure
target_compile_definitions(integration_tests PRIVATE
    CATCH_CONFIG_NO_POSIX_SIGNALS
)

# Register test with CTest and set environment variables for Address Sanitizer
# Instead of using catch_discover_tests which has issues with our signal handling,
# we'll run the tests directly with specific labels
add_test(
    NAME integration_tests_direct
    COMMAND integration_tests
)
set_tests_properties(integration_tests_direct PROPERTIES
    ENVIRONMENT "ASAN_OPTIONS=handle_segv=0:allow_user_segv_handler=1:detect_leaks=0"
)

# We'll skip the automatic test discovery since it's causing segfaults with our signal handling
# include(${CMAKE_BINARY_DIR}/_deps/catch2-src/extras/Catch.cmake)
# catch_discover_tests(integration_tests
# TEST_PREFIX ""
# PROPERTIES ENVIRONMENT "ASAN_OPTIONS=handle_segv=0:allow_user_segv_handler=1:detect_leaks=0"
# )

# Run try_catch_guard tests if available
if(TRY_CATCH_GUARD_AVAILABLE)
    message(STATUS "Configuring try_catch_guard tests to run with project tests")

    # Check if try_catch_guard has its own tests directory
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../external/libs/try_catch_guard/tests")
        # We'll run the tests directly from the try_catch_guard project
        # by executing its test binary if it exists after building
        add_custom_target(run_try_catch_guard_tests
            COMMAND ${CMAKE_COMMAND} -E echo "Running try_catch_guard tests if available"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/external/libs/try_catch_guard
            COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/../external/libs/try_catch_guard && ./build_test.sh
            COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/../external/libs/try_catch_guard && ./run_test.sh
            COMMENT "Building and running try_catch_guard tests"
        )
        add_dependencies(integration_tests run_try_catch_guard_tests)
    else()
        message(WARNING "try_catch_guard library found but tests directory not available")
    endif()
endif()