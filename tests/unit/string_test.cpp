// Define CATCH_CONFIG_NO_POSIX_SIGNALS before including Catch2
// #define CATCH_CONFIG_NO_POSIX_SIGNALS

// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "../../src/libs/core/string.hpp"
#include <string>
#include <iostream>

TEST_CASE("String constructors", "[string]")
{
    SECTION("Default constructor")
    {
        cpp_ex::String str;
        REQUIRE(str.isEmpty());
        REQUIRE(str.getLength() == 0);
        REQUIRE(str.getString() == "");
    }

    SECTION("Constructor with std::string")
    {
        std::string stdStr = "Hello, World!";
        cpp_ex::String str(stdStr);
        REQUIRE_FALSE(str.isEmpty());
        REQUIRE(str.getLength() == 13);
        REQUIRE(str.getString() == "Hello, World!");
    }

    SECTION("Constructor with C-string")
    {
        const char *cStr = "Hello, World!";
        cpp_ex::String str(cStr);
        REQUIRE_FALSE(str.isEmpty());
        REQUIRE(str.getLength() == 13);
        REQUIRE(str.getString() == "Hello, World!");
    }

    SECTION("Constructor with count and char")
    {
        cpp_ex::String str(1, 'H');
        REQUIRE(str.getString() == "H");

        cpp_ex::String str2(3, 'h');
        REQUIRE(str2.getString() == "hhh");
    }

    SECTION("Copy constructor")
    {
        cpp_ex::String str1("Hello, World!");
        cpp_ex::String str2(str1);

        REQUIRE(str2.getString() == "Hello, World!");

        // Modifying str2 should not affect str1
        str2 = "Modified";
        REQUIRE(str2.getString() == "Modified");
        REQUIRE(str1.getString() == "Hello, World!");
    }

    SECTION("Constructor with count and character")
    {
        // Test with multiple characters
        cpp_ex::String str(5, 'a');
        REQUIRE(str.getString() == "aaaaa");
        REQUIRE(str.getLength() == 5);

        // Test with a single character
        cpp_ex::String singleChar(1, 'h');
        REQUIRE(singleChar.getString() == "h");
        REQUIRE(singleChar.getLength() == 1);
    }
}

TEST_CASE("String assignment operators", "[string]")
{
    SECTION("Assignment from std::string")
    {
        cpp_ex::String str;
        str = std::string("Hello, World!");

        REQUIRE(str.getString() == "Hello, World!");
    }

    SECTION("Assignment from C-string")
    {
        cpp_ex::String str;
        str = "Hello, World!";

        REQUIRE(str.getString() == "Hello, World!");
    }

    SECTION("Assignment from another String")
    {
        cpp_ex::String str1("Hello, World!");
        cpp_ex::String str2;

        str2 = str1;

        REQUIRE(str2.getString() == "Hello, World!");

        // Modifying str2 should not affect str1
        str2 = "Modified";
        REQUIRE(str2.getString() == "Modified");
        REQUIRE(str1.getString() == "Hello, World!");
    }
}

TEST_CASE("String conversion methods", "[string]")
{
    SECTION("Conversion to std::string")
    {
        cpp_ex::String str("Hello, World!");
        std::string stdStr = str;

        REQUIRE(stdStr == "Hello, World!");
    }

    SECTION("getCString() method")
    {
        cpp_ex::String str("Hello, World!");
        const char *cStr = str.getCString();

        REQUIRE(std::string(cStr) == "Hello, World!");
    }

    SECTION("getString() method")
    {
        cpp_ex::String str("Hello, World!");
        std::string stdStr = str.getString();

        REQUIRE(stdStr == "Hello, World!");
    }
}

TEST_CASE("String basic methods", "[string]")
{
    SECTION("getLength() method")
    {
        cpp_ex::String str("Hello, World!");
        REQUIRE(str.getLength() == 13);

        cpp_ex::String emptyStr;
        REQUIRE(emptyStr.getLength() == 0);
    }

    SECTION("isEmpty() method")
    {
        cpp_ex::String emptyStr;
        REQUIRE(emptyStr.isEmpty());

        cpp_ex::String nonEmptyStr("Hello");
        REQUIRE_FALSE(nonEmptyStr.isEmpty());
    }

    SECTION("clear() method")
    {
        cpp_ex::String str("Hello, World!");

        str.clear();

        REQUIRE(str.isEmpty());
        REQUIRE(str.getLength() == 0);
        REQUIRE(str.getString() == "");
    }
}

TEST_CASE("String access methods", "[string]")
{
    cpp_ex::String str("Hello, World!");

    SECTION("charAt() method")
    {
        REQUIRE(str.charAt(0) == 'H');
        REQUIRE(str.charAt(7) == 'W');
        REQUIRE(str.charAt(12) == '!');

        // Out of range should return null character
        REQUIRE(str.charAt(100) == '\0');
    }

    SECTION("setCharAt() method")
    {
        str.setCharAt(0, 'h');
        REQUIRE(str.charAt(0) == 'h');

        str.setCharAt(7, 'w');
        REQUIRE(str.charAt(7) == 'w');

        // Out of range should have no effect
        str.setCharAt(100, 'X');
        REQUIRE(str.charAt(100) == '\0');
    }

    SECTION("operator[] method")
    {
        REQUIRE(str[0] == 'H');
        REQUIRE(str[7] == 'W');
        REQUIRE(str[12] == '!');
    }
}

TEST_CASE("String modification methods", "[string]")
{
    SECTION("append() methods")
    {
        cpp_ex::String str("Hello");

        // Append String
        cpp_ex::String suffix(", World!");
        str.append(suffix);
        REQUIRE(str.getString() == "Hello, World!");

        // Append std::string
        str.clear();
        str = "Hello";
        str.append(std::string(", World!"));
        REQUIRE(str.getString() == "Hello, World!");

        // Append C-string
        str.clear();
        str = "Hello";
        str.append(", World!");
        REQUIRE(str.getString() == "Hello, World!");

        // Append character
        str.clear();
        str = "Hello";
        str.append('!');
        REQUIRE(str.getString() == "Hello!");
    }

    SECTION("appendAndReturn() method")
    {
        cpp_ex::String str("Hello");

        cpp_ex::String &ref = str.appendAndReturn(", World!");

        REQUIRE(str.getString() == "Hello, World!");
        REQUIRE(&ref == &str); // Should return a reference to the same object
    }

    SECTION("insert() method")
    {
        cpp_ex::String str("Hello World!");

        str.insert(6, "Beautiful ");

        REQUIRE(str.getString() == "Hello Beautiful World!");
    }

    SECTION("remove() method")
    {
        cpp_ex::String str("Hello Beautiful World!");

        str.remove(6, 10); // Remove "Beautiful "

        REQUIRE(str.getString() == "Hello World!");
    }

    SECTION("erase() methods")
    {
        cpp_ex::String str("Hello, World!");

        // Erase with position and length
        str.erase(5, 2); // Erase ", "
        REQUIRE(str.getString() == "HelloWorld!");

        // Erase with position only - use erase(pos, 1) to avoid ambiguity
        str = "Hello, World!";
        str.erase(5, 1); // Erase at position 5 (the comma)
        REQUIRE(str.getString() == "Hello World!");

        // Erase with positions using eraseRange
        str = "Hello, World!";
        str.eraseRange(5, 7); // Erase ", "
        REQUIRE(str.getString() == "HelloWorld!");

        // Erase a single character at a position
        str = "Hello, World!";
        str.eraseChar(5); // Erase ',' at position 5
        REQUIRE(str.getString() == "Hello World!");
    }

    SECTION("replace() methods")
    {
        cpp_ex::String str("Hello, World!");

        // Replace with position, length, and string
        str.replace(7, 5, "C++");
        REQUIRE(str.getString() == "Hello, C++!");

        // Replace all occurrences
        str = "Hello, Hello, Hello!";
        str.replace("Hello", "Hi");
        REQUIRE(str.getString() == "Hi, Hi, Hi!");
    }

    SECTION("substring() method")
    {
        cpp_ex::String str("Hello, World!");

        // Substring with position and length
        cpp_ex::String sub1 = str.substring(7, 5);
        REQUIRE(sub1.getString() == "World");

        // Substring with position only (to the end)
        cpp_ex::String sub2 = str.substring(7);
        REQUIRE(sub2.getString() == "World!");
    }
}

TEST_CASE("String search methods", "[string]")
{
    cpp_ex::String str("Hello, World! Hello again!");

    SECTION("find() methods")
    {
        // Find string
        REQUIRE(str.find("World") == 7);
        REQUIRE(str.find("Hello") == 0);
        REQUIRE(str.find("Hello", 1) == 14); // Second occurrence
        REQUIRE(str.find("NotFound") == std::string::npos);

        // Find character
        REQUIRE(str.find('W') == 7);
        REQUIRE(str.find('!') == 12);
        REQUIRE(str.find('!', 13) == 25); // Second occurrence
        REQUIRE(str.find('X') == std::string::npos);
    }

    SECTION("findLast() methods")
    {
        // Find last string
        REQUIRE(str.findLast("Hello") == 14);
        REQUIRE(str.findLast("World") == 7);
        REQUIRE(str.findLast("Hello", 13) == 0); // First occurrence when searching backwards from position 13
        REQUIRE(str.findLast("NotFound") == std::string::npos);

        // Find last character
        REQUIRE(str.findLast('!') == 25);
        REQUIRE(str.findLast('H') == 14);
        REQUIRE(str.findLast('!', 13) == 12); // First '!' when searching backwards from position 13
        REQUIRE(str.findLast('X') == std::string::npos);
    }

    SECTION("contains() methods")
    {
        // Contains string
        REQUIRE(str.contains("Hello"));
        REQUIRE(str.contains("World"));
        REQUIRE_FALSE(str.contains("NotFound"));

        // Contains character
        REQUIRE(str.contains('H'));
        REQUIRE(str.contains('!'));
        REQUIRE_FALSE(str.contains('X'));
    }

    SECTION("startsWith() method")
    {
        REQUIRE(str.startsWith("Hello"));
        REQUIRE_FALSE(str.startsWith("World"));
    }

    SECTION("endsWith() method")
    {
        REQUIRE(str.endsWith("again!"));
        REQUIRE_FALSE(str.endsWith("World"));
    }
}

TEST_CASE("String transformation methods", "[string]")
{
    SECTION("toUpperCase() method")
    {
        cpp_ex::String str("Hello, World!");
        cpp_ex::String upper = str.toUpperCase();

        REQUIRE(upper.getString() == "HELLO, WORLD!");
        REQUIRE(str.getString() == "Hello, World!"); // Original should be unchanged
    }

    SECTION("toLowerCase() method")
    {
        cpp_ex::String str("Hello, World!");
        cpp_ex::String lower = str.toLowerCase();

        REQUIRE(lower.getString() == "hello, world!");
        REQUIRE(str.getString() == "Hello, World!"); // Original should be unchanged
    }

    SECTION("trim() methods")
    {
        // trim() - both ends
        cpp_ex::String str1("  Hello, World!  ");
        cpp_ex::String trimmed1 = str1.trim();
        REQUIRE(trimmed1.getString() == "Hello, World!");
        REQUIRE(str1.getString() == "  Hello, World!  "); // Original should be unchanged

        // ltrim() - left end only
        cpp_ex::String str2("  Hello, World!  ");
        cpp_ex::String trimmed2 = str2.ltrim();
        REQUIRE(trimmed2.getString() == "Hello, World!  ");

        // rtrim() - right end only
        cpp_ex::String str3("  Hello, World!  ");
        cpp_ex::String trimmed3 = str3.rtrim();
        REQUIRE(trimmed3.getString() == "  Hello, World!");
    }

    SECTION("split() method")
    {
        cpp_ex::String str("apple,banana,cherry,date");
        cpp_ex::Vector<cpp_ex::String> parts = str.split(",");

        REQUIRE(parts.getSize() == 4);
        REQUIRE(parts[0].getString() == "apple");
        REQUIRE(parts[1].getString() == "banana");
        REQUIRE(parts[2].getString() == "cherry");
        REQUIRE(parts[3].getString() == "date");
    }
}

TEST_CASE("String additional methods", "[string]")
{
    SECTION("countCharacters() method")
    {
        cpp_ex::String str("hello");
        auto charCount = str.countCharacters();

        REQUIRE(charCount['h'] == 1);
        REQUIRE(charCount['e'] == 1);
        REQUIRE(charCount['l'] == 2);
        REQUIRE(charCount['o'] == 1);
        REQUIRE(charCount['x'] == 0); // Character not in string
    }

    SECTION("getWordFrequencies() method")
    {
        cpp_ex::String str("hello world hello");
        auto wordFreq = str.getWordFrequencies();

        REQUIRE(wordFreq[cpp_ex::String("hello")] == 2);
        REQUIRE(wordFreq[cpp_ex::String("world")] == 1);
    }

    SECTION("toMap() method")
    {
        cpp_ex::String str("name=John;age=30;city=New York");

        // With split token "=" and separator ";"
        auto map1 = str.toMap("=", ";");
        REQUIRE(map1.getSize() == 3);
        REQUIRE(map1[cpp_ex::String("name")].getString() == "John");
        REQUIRE(map1[cpp_ex::String("age")].getString() == "30");
        REQUIRE(map1[cpp_ex::String("city")].getString() == "New York");

        // With empty split token and non-empty separator
        auto map2 = str.toMap("", ";");
        REQUIRE(map2.getSize() == 3);
        REQUIRE(map2[cpp_ex::String("name=John")].getString() == "name=John");
        REQUIRE(map2[cpp_ex::String("age=30")].getString() == "age=30");
        REQUIRE(map2[cpp_ex::String("city=New York")].getString() == "city=New York");

        // With non-empty split token and empty separator
        auto map3 = str.toMap("=", "");
        REQUIRE(map3.isEmpty());
    }

    SECTION("toStringVector() method")
    {
        cpp_ex::String str("hello");
        auto vec = str.toStringVector();

        /*
        // Print the content of the vector for debugging
        std::cout << "Vector size: " << vec.getSize() << std::endl;
        for (size_t i = 0; i < vec.getSize(); ++i)
        {
            std::cout << "vec[" << i << "] = '" << vec[i].getString() << "'" << std::endl;
        }
        */

        REQUIRE(vec.getSize() == 5);
        REQUIRE(vec[0].getString() == "h");
        REQUIRE(vec[1].getString() == "e");
        REQUIRE(vec[2].getString() == "l");
        REQUIRE(vec[3].getString() == "l");
        REQUIRE(vec[4].getString() == "o");
    }

    SECTION("toCharVector() method")
    {
        cpp_ex::String str("hello");
        auto vec = str.toCharVector();

        REQUIRE(vec.getSize() == 5);
        REQUIRE(vec[0] == 'h');
        REQUIRE(vec[1] == 'e');
        REQUIRE(vec[2] == 'l');
        REQUIRE(vec[3] == 'l');
        REQUIRE(vec[4] == 'o');
    }

    SECTION("splitByToken() method")
    {
        cpp_ex::String str("apple,banana,cherry,date");

        // Without trim
        auto parts1 = str.splitByToken(",");
        REQUIRE(parts1.getSize() == 4);
        REQUIRE(parts1[0].getString() == "apple");
        REQUIRE(parts1[1].getString() == "banana");
        REQUIRE(parts1[2].getString() == "cherry");
        REQUIRE(parts1[3].getString() == "date");

        // With trim
        cpp_ex::String str2("apple, banana , cherry, date");
        auto parts2 = str2.splitByToken(",", true);
        REQUIRE(parts2.getSize() == 4);
        REQUIRE(parts2[0].getString() == "apple");
        REQUIRE(parts2[1].getString() == "banana");
        REQUIRE(parts2[2].getString() == "cherry");
        REQUIRE(parts2[3].getString() == "date");
    }
}

TEST_CASE("String operators", "[string]")
{
    cpp_ex::String str1("Hello");
    cpp_ex::String str2("Hello");
    cpp_ex::String str3("World");

    SECTION("Comparison operators")
    {
        REQUIRE(str1 == str2);
        REQUIRE_FALSE(str1 == str3);

        REQUIRE_FALSE(str1 != str2);
        REQUIRE(str1 != str3);

        REQUIRE_FALSE(str1 < str2);
        REQUIRE(str1 < str3); // "Hello" < "World"

        REQUIRE_FALSE(str1 > str2);
        REQUIRE_FALSE(str1 > str3);

        REQUIRE(str1 <= str2);
        REQUIRE(str1 <= str3);

        REQUIRE(str1 >= str2);
        REQUIRE_FALSE(str1 >= str3);
    }

    SECTION("Concatenation operators")
    {
        // operator+
        cpp_ex::String result = str1 + str3;
        REQUIRE(result.getString() == "HelloWorld");

        // operator+=
        cpp_ex::String str = "Hello";
        str += cpp_ex::String(", World!");
        REQUIRE(str.getString() == "Hello, World!");
    }
}