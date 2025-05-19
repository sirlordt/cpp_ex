// Define CATCH_CONFIG_NO_POSIX_SIGNALS before including Catch2
// #define CATCH_CONFIG_NO_POSIX_SIGNALS

// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "../../src/libs/core/common.hpp"
#include <string>

TEST_CASE("NullPointerAccessException tests", "[exceptions]")
{
    SECTION("Default constructor")
    {
        // Create exception with default message
        cpp_ex::exceptions::NullPointerAccessException ex;

        // Check that the default message is as expected
        REQUIRE(std::string(ex.what()) == "Null pointer access attempt");
    }

    SECTION("Constructor with custom message")
    {
        // Create exception with custom message
        std::string customMessage = "Custom null pointer error";
        cpp_ex::exceptions::NullPointerAccessException ex(customMessage);

        // Check that the custom message is as expected
        REQUIRE(std::string(ex.what()) == customMessage);
    }

    SECTION("Exception inheritance")
    {
        // Check that NullPointerAccessException is a std::runtime_error
        cpp_ex::exceptions::NullPointerAccessException ex;
        REQUIRE(dynamic_cast<const std::runtime_error *>(&ex) != nullptr);

        // Check that it can be caught as a std::exception
        bool caught = false;
        try
        {
            throw cpp_ex::exceptions::NullPointerAccessException();
        }
        catch (const std::exception &e)
        {
            caught = true;
        }
        REQUIRE(caught);
    }
}