// Define CATCH_CONFIG_NO_POSIX_SIGNALS before including Catch2
// #define CATCH_CONFIG_NO_POSIX_SIGNALS

// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "../../src/libs/core/vector.hpp"
#include <string>
#include <algorithm>
#include <numeric>

TEST_CASE("Vector constructors", "[vector]")
{
    SECTION("Default constructor")
    {
        cpp_ex::Vector<int> vec;
        REQUIRE(vec.isEmpty());
        REQUIRE(vec.getSize() == 0);
    }

    SECTION("Constructor with size")
    {
        cpp_ex::Vector<int> vec(5);
        REQUIRE_FALSE(vec.isEmpty());
        REQUIRE(vec.getSize() == 5);

        // Elements should be default-initialized
        for (size_t i = 0; i < vec.getSize(); ++i)
        {
            REQUIRE(vec[i] == 0);
        }
    }

    SECTION("Constructor with size and value")
    {
        cpp_ex::Vector<int> vec(5, 42);
        REQUIRE_FALSE(vec.isEmpty());
        REQUIRE(vec.getSize() == 5);

        // All elements should be initialized to the provided value
        for (size_t i = 0; i < vec.getSize(); ++i)
        {
            REQUIRE(vec[i] == 42);
        }
    }

    SECTION("Constructor with initializer list")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};
        REQUIRE_FALSE(vec.isEmpty());
        REQUIRE(vec.getSize() == 5);

        // Elements should match the initializer list
        for (size_t i = 0; i < vec.getSize(); ++i)
        {
            REQUIRE(vec[i] == static_cast<int>(i + 1));
        }
    }

    SECTION("Copy constructor")
    {
        cpp_ex::Vector<int> vec1 = {1, 2, 3, 4, 5};
        cpp_ex::Vector<int> vec2(vec1);

        REQUIRE(vec2.getSize() == 5);

        // Elements should match the original vector
        for (size_t i = 0; i < vec2.getSize(); ++i)
        {
            REQUIRE(vec2[i] == static_cast<int>(i + 1));
        }

        // Modifying vec2 should not affect vec1
        vec2[2] = 100;
        REQUIRE(vec2[2] == 100);
        REQUIRE(vec1[2] == 3);
    }

    SECTION("Move constructor")
    {
        cpp_ex::Vector<int> vec1 = {1, 2, 3, 4, 5};
        cpp_ex::Vector<int> vec2(std::move(vec1));

        REQUIRE(vec2.getSize() == 5);

        // Elements should match the original vector
        for (size_t i = 0; i < vec2.getSize(); ++i)
        {
            REQUIRE(vec2[i] == static_cast<int>(i + 1));
        }

        // vec1 should be empty after move
        REQUIRE(vec1.isEmpty());
    }

    SECTION("Constructor from std::vector")
    {
        std::vector<int> stdVec = {1, 2, 3, 4, 5};
        cpp_ex::Vector<int> vec(stdVec);

        REQUIRE(vec.getSize() == 5);

        // Elements should match the std::vector
        for (size_t i = 0; i < vec.getSize(); ++i)
        {
            REQUIRE(vec[i] == stdVec[i]);
        }
    }
}
TEST_CASE("Vector assignment operators", "[vector]")
{
    SECTION("Copy assignment operator")
    {
        cpp_ex::Vector<int> vec1 = {1, 2, 3, 4, 5};
        cpp_ex::Vector<int> vec2;

        vec2 = vec1;

        REQUIRE(vec2.getSize() == 5);

        // Elements should match the original vector
        for (size_t i = 0; i < vec2.getSize(); ++i)
        {
            REQUIRE(vec2[i] == static_cast<int>(i + 1));
        }

        // Modifying vec2 should not affect vec1
        vec2[2] = 100;
        REQUIRE(vec2[2] == 100);
        REQUIRE(vec1[2] == 3);
    }

    SECTION("Move assignment operator")
    {
        cpp_ex::Vector<int> vec1 = {1, 2, 3, 4, 5};
        cpp_ex::Vector<int> vec2;

        vec2 = std::move(vec1);

        REQUIRE(vec2.getSize() == 5);

        // Elements should match the original vector
        for (size_t i = 0; i < vec2.getSize(); ++i)
        {
            REQUIRE(vec2[i] == static_cast<int>(i + 1));
        }

        // vec1 should be empty after move
        REQUIRE(vec1.isEmpty());
    }

    SECTION("Initializer list assignment operator")
    {
        cpp_ex::Vector<int> vec;

        vec = {1, 2, 3, 4, 5};

        REQUIRE(vec.getSize() == 5);

        // Elements should match the initializer list
        for (size_t i = 0; i < vec.getSize(); ++i)
        {
            REQUIRE(vec[i] == static_cast<int>(i + 1));
        }
    }
}

TEST_CASE("Vector conversion methods", "[vector]")
{
    SECTION("Conversion to std::vector")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};
        std::vector<int> stdVec = vec;

        REQUIRE(stdVec.size() == 5);

        // Elements should match the original vector
        for (size_t i = 0; i < stdVec.size(); ++i)
        {
            REQUIRE(stdVec[i] == static_cast<int>(i + 1));
        }
    }

    SECTION("getStdVector() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};
        const auto &stdVec = vec.getStdVector();

        REQUIRE(stdVec.size() == 5);

        // Elements should match the original vector
        for (size_t i = 0; i < stdVec.size(); ++i)
        {
            REQUIRE(stdVec[i] == static_cast<int>(i + 1));
        }
    }
}

TEST_CASE("Vector element access methods", "[vector]")
{
    cpp_ex::Vector<int> vec = {10, 20, 30, 40, 50};

    SECTION("at() method")
    {
        REQUIRE(vec.at(0) == 10);
        REQUIRE(vec.at(1) == 20);
        REQUIRE(vec.at(2) == 30);
        REQUIRE(vec.at(3) == 40);
        REQUIRE(vec.at(4) == 50);

        // Modifying through at()
        vec.at(2) = 300;
        REQUIRE(vec.at(2) == 300);

        // at() with out-of-range index should throw
        REQUIRE_THROWS_AS(vec.at(5), std::out_of_range);
    }

    SECTION("operator[] method")
    {
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 20);
        REQUIRE(vec[2] == 30);
        REQUIRE(vec[3] == 40);
        REQUIRE(vec[4] == 50);

        // Modifying through operator[]
        vec[2] = 300;
        REQUIRE(vec[2] == 300);
    }

    SECTION("getFront() method")
    {
        REQUIRE(vec.getFront() == 10);

        // Modifying through getFront()
        vec.getFront() = 100;
        REQUIRE(vec.getFront() == 100);
        REQUIRE(vec[0] == 100);
    }

    SECTION("getBack() method")
    {
        REQUIRE(vec.getBack() == 50);

        // Modifying through getBack()
        vec.getBack() = 500;
        REQUIRE(vec.getBack() == 500);
        REQUIRE(vec[4] == 500);
    }

    SECTION("getData() method")
    {
        const int *data = vec.getData();

        REQUIRE(data[0] == 10);
        REQUIRE(data[1] == 20);
        REQUIRE(data[2] == 30);
        REQUIRE(data[3] == 40);
        REQUIRE(data[4] == 50);
    }
}
TEST_CASE("Vector iterator methods", "[vector]")
{
    cpp_ex::Vector<int> vec = {10, 20, 30, 40, 50};

    SECTION("begin() and end() methods")
    {
        int sum = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            sum += *it;
        }
        REQUIRE(sum == 150); // 10 + 20 + 30 + 40 + 50
    }

    SECTION("cbegin() and cend() methods")
    {
        int sum = 0;
        for (auto it = vec.cbegin(); it != vec.cend(); ++it)
        {
            sum += *it;
        }
        REQUIRE(sum == 150);
    }

    SECTION("rbegin() and rend() methods")
    {
        std::vector<int> reversed;
        for (auto it = vec.rbegin(); it != vec.rend(); ++it)
        {
            reversed.push_back(*it);
        }

        REQUIRE(reversed.size() == 5);
        REQUIRE(reversed[0] == 50);
        REQUIRE(reversed[1] == 40);
        REQUIRE(reversed[2] == 30);
        REQUIRE(reversed[3] == 20);
        REQUIRE(reversed[4] == 10);
    }

    SECTION("crbegin() and crend() methods")
    {
        std::vector<int> reversed;
        for (auto it = vec.crbegin(); it != vec.crend(); ++it)
        {
            reversed.push_back(*it);
        }

        REQUIRE(reversed.size() == 5);
        REQUIRE(reversed[0] == 50);
        REQUIRE(reversed[1] == 40);
        REQUIRE(reversed[2] == 30);
        REQUIRE(reversed[3] == 20);
        REQUIRE(reversed[4] == 10);
    }
}

TEST_CASE("Vector capacity methods", "[vector]")
{
    SECTION("isEmpty() method")
    {
        cpp_ex::Vector<int> emptyVec;
        REQUIRE(emptyVec.isEmpty());

        cpp_ex::Vector<int> nonEmptyVec = {1, 2, 3};
        REQUIRE_FALSE(nonEmptyVec.isEmpty());
    }

    SECTION("getSize() method")
    {
        cpp_ex::Vector<int> vec;
        REQUIRE(vec.getSize() == 0);

        vec.pushBack(10);
        REQUIRE(vec.getSize() == 1);

        vec.pushBack(20);
        REQUIRE(vec.getSize() == 2);

        vec.popBack();
        REQUIRE(vec.getSize() == 1);
    }

    SECTION("getMaxSize() method")
    {
        cpp_ex::Vector<int> vec;
        // Just check that it returns a reasonable value
        REQUIRE(vec.getMaxSize() > 0);
    }

    SECTION("reserve() and getCapacity() methods")
    {
        cpp_ex::Vector<int> vec;

        vec.reserve(10);
        REQUIRE(vec.getCapacity() >= 10);
        REQUIRE(vec.getSize() == 0);

        // Adding elements should not cause reallocation
        size_t capacityBefore = vec.getCapacity();
        for (int i = 0; i < 10; ++i)
        {
            vec.pushBack(i);
        }
        REQUIRE(vec.getCapacity() == capacityBefore);
        REQUIRE(vec.getSize() == 10);
    }

    SECTION("shrinkToFit() method")
    {
        cpp_ex::Vector<int> vec;
        vec.reserve(100);

        for (int i = 0; i < 10; ++i)
        {
            vec.pushBack(i);
        }

        REQUIRE(vec.getCapacity() >= 100);
        REQUIRE(vec.getSize() == 10);

        vec.shrinkToFit();
        REQUIRE(vec.getCapacity() >= 10);
        REQUIRE(vec.getSize() == 10);
    }
}

TEST_CASE("Vector modifier methods", "[vector]")
{
    SECTION("clear() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};

        REQUIRE(vec.getSize() == 5);

        vec.clear();

        REQUIRE(vec.isEmpty());
        REQUIRE(vec.getSize() == 0);
    }

    SECTION("insert() methods")
    {
        cpp_ex::Vector<int> vec = {10, 30, 40};

        // Insert single element
        auto it1 = vec.insert(vec.begin() + 1, 20);
        REQUIRE(*it1 == 20);
        REQUIRE(vec.getSize() == 4);
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 20);
        REQUIRE(vec[2] == 30);
        REQUIRE(vec[3] == 40);

        // Insert multiple copies of an element
        auto it2 = vec.insert(vec.begin(), 3, 5);
        REQUIRE(*it2 == 5);
        REQUIRE(vec.getSize() == 7);
        REQUIRE(vec[0] == 5);
        REQUIRE(vec[1] == 5);
        REQUIRE(vec[2] == 5);
        REQUIRE(vec[3] == 10);

        // Insert range
        std::vector<int> source = {60, 70, 80};
        auto it3 = vec.insert(vec.end(), source.begin(), source.end());
        REQUIRE(*it3 == 60);
        REQUIRE(vec.getSize() == 10);
        REQUIRE(vec[7] == 60);
        REQUIRE(vec[8] == 70);
        REQUIRE(vec[9] == 80);

        // Insert initializer list
        auto it4 = vec.insert(vec.end(), {90, 100});
        REQUIRE(*it4 == 90);
        REQUIRE(vec.getSize() == 12);
        REQUIRE(vec[10] == 90);
        REQUIRE(vec[11] == 100);
    }

    SECTION("emplace() method")
    {
        cpp_ex::Vector<std::string> vec = {"hello", "world"};

        auto it = vec.emplace(vec.begin() + 1, "beautiful");

        REQUIRE(*it == "beautiful");
        REQUIRE(vec.getSize() == 3);
        REQUIRE(vec[0] == "hello");
        REQUIRE(vec[1] == "beautiful");
        REQUIRE(vec[2] == "world");
    }

    SECTION("erase() methods")
    {
        cpp_ex::Vector<int> vec = {10, 20, 30, 40, 50};

        // Erase single element
        auto it1 = vec.erase(vec.begin() + 1);
        REQUIRE(*it1 == 30);
        REQUIRE(vec.getSize() == 4);
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 30);
        REQUIRE(vec[2] == 40);
        REQUIRE(vec[3] == 50);

        // Erase range
        auto it2 = vec.erase(vec.begin() + 1, vec.begin() + 3);
        REQUIRE(*it2 == 50);
        REQUIRE(vec.getSize() == 2);
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 50);
    }

    SECTION("pushBack() methods")
    {
        cpp_ex::Vector<std::string> vec;

        // Push back lvalue
        std::string s1 = "hello";
        vec.pushBack(s1);
        REQUIRE(vec.getSize() == 1);
        REQUIRE(vec[0] == "hello");

        // Push back rvalue
        vec.pushBack(std::string("world"));
        REQUIRE(vec.getSize() == 2);
        REQUIRE(vec[0] == "hello");
        REQUIRE(vec[1] == "world");
    }

    SECTION("emplaceBack() method")
    {
        cpp_ex::Vector<std::string> vec;

        auto &ref = vec.emplaceBack("hello");
        REQUIRE(vec.getSize() == 1);
        REQUIRE(vec[0] == "hello");
        REQUIRE(ref == "hello");

        vec.emplaceBack("world");
        REQUIRE(vec.getSize() == 2);
        REQUIRE(vec[0] == "hello");
        REQUIRE(vec[1] == "world");
    }

    SECTION("popBack() method")
    {
        cpp_ex::Vector<int> vec = {10, 20, 30};

        vec.popBack();
        REQUIRE(vec.getSize() == 2);
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 20);

        vec.popBack();
        REQUIRE(vec.getSize() == 1);
        REQUIRE(vec[0] == 10);

        vec.popBack();
        REQUIRE(vec.isEmpty());
    }

    SECTION("resize() methods")
    {
        cpp_ex::Vector<int> vec = {10, 20, 30};

        // Resize to larger size
        vec.resize(5);
        REQUIRE(vec.getSize() == 5);
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 20);
        REQUIRE(vec[2] == 30);
        REQUIRE(vec[3] == 0);
        REQUIRE(vec[4] == 0);

        // Resize to smaller size
        vec.resize(2);
        REQUIRE(vec.getSize() == 2);
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 20);

        // Resize with value
        vec.resize(4, 42);
        REQUIRE(vec.getSize() == 4);
        REQUIRE(vec[0] == 10);
        REQUIRE(vec[1] == 20);
        REQUIRE(vec[2] == 42);
        REQUIRE(vec[3] == 42);
    }

    SECTION("swap() method")
    {
        cpp_ex::Vector<int> vec1 = {1, 2, 3};
        cpp_ex::Vector<int> vec2 = {4, 5, 6, 7};

        vec1.swap(vec2);

        REQUIRE(vec1.getSize() == 4);
        REQUIRE(vec1[0] == 4);
        REQUIRE(vec1[1] == 5);
        REQUIRE(vec1[2] == 6);
        REQUIRE(vec1[3] == 7);

        REQUIRE(vec2.getSize() == 3);
        REQUIRE(vec2[0] == 1);
        REQUIRE(vec2[1] == 2);
        REQUIRE(vec2[2] == 3);
    }
}
TEST_CASE("Vector additional methods", "[vector]")
{
    SECTION("contains() method")
    {
        cpp_ex::Vector<int> vec = {10, 20, 30, 40, 50};

        REQUIRE(vec.contains(10));
        REQUIRE(vec.contains(30));
        REQUIRE(vec.contains(50));
        REQUIRE_FALSE(vec.contains(15));
        REQUIRE_FALSE(vec.contains(60));
    }

    SECTION("countValue() method")
    {
        cpp_ex::Vector<int> vec = {10, 20, 10, 30, 10, 40, 10};

        REQUIRE(vec.countValue(10) == 4);
        REQUIRE(vec.countValue(20) == 1);
        REQUIRE(vec.countValue(30) == 1);
        REQUIRE(vec.countValue(40) == 1);
        REQUIRE(vec.countValue(50) == 0);
    }

    SECTION("countIf() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        // Count even numbers
        auto evenCount = vec.countIf([](int n)
                                     { return n % 2 == 0; });
        REQUIRE(evenCount == 5);

        // Count numbers greater than 5
        auto greaterThan5Count = vec.countIf([](int n)
                                             { return n > 5; });
        REQUIRE(greaterThan5Count == 5);
    }

    SECTION("map() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};

        // Map to squares
        auto squares = vec.map([](int n)
                               { return n * n; });

        REQUIRE(squares.getSize() == 5);
        REQUIRE(squares[0] == 1);
        REQUIRE(squares[1] == 4);
        REQUIRE(squares[2] == 9);
        REQUIRE(squares[3] == 16);
        REQUIRE(squares[4] == 25);

        // Map to strings
        auto strings = vec.map([](int n)
                               { return "Number " + std::to_string(n); });

        REQUIRE(strings.getSize() == 5);
        REQUIRE(strings[0] == "Number 1");
        REQUIRE(strings[1] == "Number 2");
        REQUIRE(strings[2] == "Number 3");
        REQUIRE(strings[3] == "Number 4");
        REQUIRE(strings[4] == "Number 5");
    }

    SECTION("filter() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        // Filter even numbers
        auto evens = vec.filter([](int n)
                                { return n % 2 == 0; });

        REQUIRE(evens.getSize() == 5);
        REQUIRE(evens[0] == 2);
        REQUIRE(evens[1] == 4);
        REQUIRE(evens[2] == 6);
        REQUIRE(evens[3] == 8);
        REQUIRE(evens[4] == 10);

        // Filter numbers greater than 5
        auto greaterThan5 = vec.filter([](int n)
                                       { return n > 5; });

        REQUIRE(greaterThan5.getSize() == 5);
        REQUIRE(greaterThan5[0] == 6);
        REQUIRE(greaterThan5[1] == 7);
        REQUIRE(greaterThan5[2] == 8);
        REQUIRE(greaterThan5[3] == 9);
        REQUIRE(greaterThan5[4] == 10);
    }

    SECTION("forEach() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};

        // Multiply each element by 2
        vec.forEach([](int &n)
                    { n *= 2; });

        REQUIRE(vec[0] == 2);
        REQUIRE(vec[1] == 4);
        REQUIRE(vec[2] == 6);
        REQUIRE(vec[3] == 8);
        REQUIRE(vec[4] == 10);

        // Test const version
        int sum = 0;
        const auto &constVec = vec;
        constVec.forEach([&sum](const int &n)
                         { sum += n; });

        REQUIRE(sum == 30); // 2 + 4 + 6 + 8 + 10
    }

    SECTION("reduce() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};

        // Sum
        int sum = vec.reduce(0, [](int acc, int n)
                             { return acc + n; });
        REQUIRE(sum == 15);

        // Product
        int product = vec.reduce(1, [](int acc, int n)
                                 { return acc * n; });
        REQUIRE(product == 120);

        // Max
        int max = vec.reduce(std::numeric_limits<int>::min(), [](int acc, int n)
                             { return std::max(acc, n); });
        REQUIRE(max == 5);
    }

    SECTION("sort() methods")
    {
        cpp_ex::Vector<int> vec = {5, 3, 1, 4, 2};

        // Default sort (ascending)
        vec.sort();

        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == 2);
        REQUIRE(vec[2] == 3);
        REQUIRE(vec[3] == 4);
        REQUIRE(vec[4] == 5);

        // Custom sort (descending)
        vec.sort(std::greater<int>());

        REQUIRE(vec[0] == 5);
        REQUIRE(vec[1] == 4);
        REQUIRE(vec[2] == 3);
        REQUIRE(vec[3] == 2);
        REQUIRE(vec[4] == 1);
    }

    SECTION("reverse() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5};

        vec.reverse();

        REQUIRE(vec[0] == 5);
        REQUIRE(vec[1] == 4);
        REQUIRE(vec[2] == 3);
        REQUIRE(vec[3] == 2);
        REQUIRE(vec[4] == 1);
    }

    SECTION("findFirstIndex() method")
    {
        cpp_ex::Vector<int> vec = {10, 20, 30, 20, 10};

        REQUIRE(vec.findFirstIndex(10) == 0);
        REQUIRE(vec.findFirstIndex(20) == 1);
        REQUIRE(vec.findFirstIndex(30) == 2);
        REQUIRE(vec.findFirstIndex(40) == static_cast<size_t>(-1)); // Not found
    }

    SECTION("findFirstIndexIf() method")
    {
        cpp_ex::Vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        // Find first even number
        auto firstEvenIndex = vec.findFirstIndexIf([](int n)
                                                   { return n % 2 == 0; });
        REQUIRE(firstEvenIndex == 1); // 2 is at index 1

        // Find first number greater than 5
        auto firstGreaterThan5Index = vec.findFirstIndexIf([](int n)
                                                           { return n > 5; });
        REQUIRE(firstGreaterThan5Index == 5); // 6 is at index 5

        // Find something that doesn't exist
        auto notFoundIndex = vec.findFirstIndexIf([](int n)
                                                  { return n > 100; });
        REQUIRE(notFoundIndex == static_cast<size_t>(-1));
    }

    SECTION("equals() method")
    {
        cpp_ex::Vector<int> vec1 = {1, 2, 3, 4, 5};
        cpp_ex::Vector<int> vec2 = {1, 2, 3, 4, 5};
        cpp_ex::Vector<int> vec3 = {5, 4, 3, 2, 1};

        REQUIRE(vec1.equals(vec2));
        REQUIRE_FALSE(vec1.equals(vec3));
    }
}

TEST_CASE("Vector comparison operators", "[vector]")
{
    cpp_ex::Vector<int> vec1 = {1, 2, 3};
    cpp_ex::Vector<int> vec2 = {1, 2, 3};
    cpp_ex::Vector<int> vec3 = {1, 2, 4};
    cpp_ex::Vector<int> vec4 = {1, 2};

    SECTION("operator== and operator!=")
    {
        REQUIRE(vec1 == vec2);
        REQUIRE_FALSE(vec1 != vec2);

        REQUIRE_FALSE(vec1 == vec3);
        REQUIRE(vec1 != vec3);

        REQUIRE_FALSE(vec1 == vec4);
        REQUIRE(vec1 != vec4);
    }

    SECTION("operator<, operator<=, operator>, operator>=")
    {
        REQUIRE_FALSE(vec1 < vec2);
        REQUIRE(vec1 <= vec2);
        REQUIRE_FALSE(vec1 > vec2);
        REQUIRE(vec1 >= vec2);

        REQUIRE(vec1 < vec3);
        REQUIRE(vec1 <= vec3);
        REQUIRE_FALSE(vec1 > vec3);
        REQUIRE_FALSE(vec1 >= vec3);

        REQUIRE_FALSE(vec1 < vec4);
        REQUIRE_FALSE(vec1 <= vec4);
        REQUIRE(vec1 > vec4);
        REQUIRE(vec1 >= vec4);
    }
}

TEST_CASE("Vector non-member functions", "[vector]")
{
    SECTION("swap() function")
    {
        cpp_ex::Vector<int> vec1 = {1, 2, 3};
        cpp_ex::Vector<int> vec2 = {4, 5, 6, 7};

        swap(vec1, vec2);

        REQUIRE(vec1.getSize() == 4);
        REQUIRE(vec1[0] == 4);
        REQUIRE(vec1[1] == 5);
        REQUIRE(vec1[2] == 6);
        REQUIRE(vec1[3] == 7);

        REQUIRE(vec2.getSize() == 3);
        REQUIRE(vec2[0] == 1);
        REQUIRE(vec2[1] == 2);
        REQUIRE(vec2[2] == 3);
    }
}