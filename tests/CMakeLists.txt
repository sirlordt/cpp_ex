# Include FetchContent module
include(FetchContent)

# Download and build Catch2 from GitHub
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0 # Using the latest version as of May 2025
)

FetchContent_MakeAvailable(catch2)

# Run the script to modify Catch2's signal handling
execute_process(
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/modify_catch2.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE SCRIPT_RESULT
)

if(NOT SCRIPT_RESULT EQUAL 0)
    message(WARNING "Failed to modify Catch2's signal handling. Tests may not work correctly.")
endif()

# Enable Address Sanitizer and Undefined Behavior Sanitizer if ENABLE_ASAN is ON
if(ENABLE_ASAN)
    message(STATUS "Address Sanitizer enabled for tests")

    # Ensure ASAN runtime is linked first
    set(CMAKE_CXX_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer ${CMAKE_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address,undefined ${CMAKE_EXE_LINKER_FLAGS}")
else()
    message(STATUS "Address Sanitizer disabled for tests")
endif()

# Run try_catch_guard tests if available and enabled
if(TRY_CATCH_GUARD_AVAILABLE AND BUILD_TRY_CATCH_GUARD_TESTS)
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

    else()
        message(WARNING "try_catch_guard library found but tests directory not available")
    endif()
elseif(TRY_CATCH_GUARD_AVAILABLE AND NOT BUILD_TRY_CATCH_GUARD_TESTS)
    message(STATUS "try_catch_guard tests are disabled")
endif()

# Add unit tests subdirectory
add_subdirectory(unit)