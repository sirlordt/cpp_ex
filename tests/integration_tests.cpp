// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include <vector>
#include <string>
#include <iostream>

// Simple test case for vector operations
TEST_CASE("Vector operations work correctly", "[vector]") {
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    SECTION("Vector has the correct size") {
        REQUIRE(v.size() == 5);
    }
    
    SECTION("Vector elements can be accessed") {
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
        REQUIRE(v[3] == 4);
        REQUIRE(v[4] == 5);
    }
    
    SECTION("Vector can be modified") {
        v.push_back(6);
        REQUIRE(v.size() == 6);
        REQUIRE(v[5] == 6);
    }
}

// Test case for string operations
TEST_CASE("String operations work correctly", "[string]") {
    std::string s = "Hello, world!";
    
    SECTION("String has the correct length") {
        REQUIRE(s.length() == 13);
    }
    
    SECTION("String can be modified") {
        s += " C++ is awesome!";
        REQUIRE(s == "Hello, world! C++ is awesome!");
    }
    
    SECTION("String can be split") {
        size_t pos = s.find(",");
        REQUIRE(pos != std::string::npos);
        REQUIRE(s.substr(0, pos) == "Hello");
    }
}

// Test case for arithmetic operations
TEST_CASE("Arithmetic operations work correctly", "[arithmetic]") {
    SECTION("Addition works") {
        REQUIRE(1 + 1 == 2);
        REQUIRE(2 + 2 == 4);
    }
    
    SECTION("Subtraction works") {
        REQUIRE(5 - 3 == 2);
        REQUIRE(10 - 5 == 5);
    }
    
    SECTION("Multiplication works") {
        REQUIRE(2 * 3 == 6);
        REQUIRE(4 * 5 == 20);
    }
    
    SECTION("Division works") {
        REQUIRE(10 / 2 == 5);
        REQUIRE(20 / 4 == 5);
    }
}

// Test case for boolean logic
TEST_CASE("Boolean logic works correctly", "[boolean]") {
    SECTION("AND operator works") {
        REQUIRE((true && true) == true);
        REQUIRE((true && false) == false);
        REQUIRE((false && true) == false);
        REQUIRE((false && false) == false);
    }
    
    SECTION("OR operator works") {
        REQUIRE((true || true) == true);
        REQUIRE((true || false) == true);
        REQUIRE((false || true) == true);
        REQUIRE((false || false) == false);
    }
    
    SECTION("NOT operator works") {
        REQUIRE((!true) == false);
        REQUIRE((!false) == true);
    }
}