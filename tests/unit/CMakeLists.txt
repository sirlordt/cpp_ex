# Unit tests for cpp_ex library

# Add the unit tests executable
add_executable(unit_tests
    common_test.cpp
    safe_shared_ptr_test.cpp
    safe_unique_ptr_test.cpp
    map_test.cpp
    vector_test.cpp
    string_test.cpp
)

# Link against Catch2 and the cpp_ex_core library
target_link_libraries(unit_tests PRIVATE
    Catch2::Catch2WithMain
    cpp_ex_core
)

# Define CATCH_CONFIG_NO_POSIX_SIGNALS as an extra safety measure
target_compile_definitions(unit_tests PRIVATE
    CATCH_CONFIG_NO_POSIX_SIGNALS
)

# Add the test to CTest
add_test(NAME unit_tests COMMAND unit_tests)

# Set working directory for the test
set_tests_properties(unit_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    ENVIRONMENT "ASAN_OPTIONS=handle_segv=0:allow_user_segv_handler=1:detect_leaks=0"
)