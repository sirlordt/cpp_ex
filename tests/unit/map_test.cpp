// Define CATCH_CONFIG_NO_POSIX_SIGNALS before including Catch2
// #define CATCH_CONFIG_NO_POSIX_SIGNALS

// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "../../src/libs/core/map.hpp"
#include <string>
#include <functional>

TEST_CASE("Map constructors", "[map]")
{
    SECTION("Default constructor")
    {
        cpp_ex::Map<int, std::string> map;
        REQUIRE(map.isEmpty());
        REQUIRE(map.getSize() == 0);
    }

    SECTION("Constructor with custom comparator")
    {
        cpp_ex::Map<int, std::string, std::greater<int>> map;

        map[1] = "one";
        map[2] = "two";
        map[3] = "three";

        // With std::greater, keys should be in descending order
        auto keys = map.getKeys();
        REQUIRE(keys.getSize() == 3);
        REQUIRE(keys[0] == 3);
        REQUIRE(keys[1] == 2);
        REQUIRE(keys[2] == 1);
    }

    SECTION("Constructor with initializer list")
    {
        cpp_ex::Map<int, std::string> map = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        REQUIRE(map.getSize() == 3);
        REQUIRE(map[1] == "one");
        REQUIRE(map[2] == "two");
        REQUIRE(map[3] == "three");
    }

    SECTION("Copy constructor")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        cpp_ex::Map<int, std::string> map2(map1);

        REQUIRE(map2.getSize() == 3);
        REQUIRE(map2[1] == "one");
        REQUIRE(map2[2] == "two");
        REQUIRE(map2[3] == "three");

        // Modifying map2 should not affect map1
        map2[2] = "TWO";
        REQUIRE(map2[2] == "TWO");
        REQUIRE(map1[2] == "two");
    }

    SECTION("Move constructor")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        cpp_ex::Map<int, std::string> map2(std::move(map1));

        REQUIRE(map2.getSize() == 3);
        REQUIRE(map2[1] == "one");
        REQUIRE(map2[2] == "two");
        REQUIRE(map2[3] == "three");

        REQUIRE(map1.isEmpty());
    }

    SECTION("Constructor from std::map")
    {
        std::map<int, std::string> stdMap = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        cpp_ex::Map<int, std::string> map(stdMap);

        REQUIRE(map.getSize() == 3);
        REQUIRE(map[1] == "one");
        REQUIRE(map[2] == "two");
        REQUIRE(map[3] == "three");
    }
}

TEST_CASE("Map assignment operators", "[map]")
{
    SECTION("Copy assignment operator")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        cpp_ex::Map<int, std::string> map2;
        map2 = map1;

        REQUIRE(map2.getSize() == 3);
        REQUIRE(map2[1] == "one");
        REQUIRE(map2[2] == "two");
        REQUIRE(map2[3] == "three");

        // Modifying map2 should not affect map1
        map2[2] = "TWO";
        REQUIRE(map2[2] == "TWO");
        REQUIRE(map1[2] == "two");
    }

    SECTION("Move assignment operator")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        cpp_ex::Map<int, std::string> map2;
        map2 = std::move(map1);

        REQUIRE(map2.getSize() == 3);
        REQUIRE(map2[1] == "one");
        REQUIRE(map2[2] == "two");
        REQUIRE(map2[3] == "three");

        REQUIRE(map1.isEmpty());
    }

    SECTION("Initializer list assignment operator")
    {
        cpp_ex::Map<int, std::string> map;

        map = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        REQUIRE(map.getSize() == 3);
        REQUIRE(map[1] == "one");
        REQUIRE(map[2] == "two");
        REQUIRE(map[3] == "three");
    }
}

TEST_CASE("Map conversion methods", "[map]")
{
    SECTION("Conversion to std::map")
    {
        cpp_ex::Map<int, std::string> map = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        std::map<int, std::string> stdMap = map;

        REQUIRE(stdMap.size() == 3);
        REQUIRE(stdMap[1] == "one");
        REQUIRE(stdMap[2] == "two");
        REQUIRE(stdMap[3] == "three");
    }

    SECTION("getStdMap() method")
    {
        cpp_ex::Map<int, std::string> map = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        const auto &stdMap = map.getStdMap();

        REQUIRE(stdMap.size() == 3);
        REQUIRE(stdMap.at(1) == "one");
        REQUIRE(stdMap.at(2) == "two");
        REQUIRE(stdMap.at(3) == "three");
    }
}

TEST_CASE("Map iterator methods", "[map]")
{
    cpp_ex::Map<int, std::string> map = {
        {1, "one"},
        {2, "two"},
        {3, "three"}};

    SECTION("begin() and end() methods")
    {
        int count = 0;
        for (auto it = map.begin(); it != map.end(); ++it)
        {
            REQUIRE(it->second == (it->first == 1 ? "one" : (it->first == 2 ? "two" : "three")));
            count++;
        }
        REQUIRE(count == 3);
    }

    SECTION("cbegin() and cend() methods")
    {
        int count = 0;
        for (auto it = map.cbegin(); it != map.cend(); ++it)
        {
            REQUIRE(it->second == (it->first == 1 ? "one" : (it->first == 2 ? "two" : "three")));
            count++;
        }
        REQUIRE(count == 3);
    }

    SECTION("rbegin() and rend() methods")
    {
        auto it = map.rbegin();
        REQUIRE(it->first == 3);
        REQUIRE(it->second == "three");

        ++it;
        REQUIRE(it->first == 2);
        REQUIRE(it->second == "two");

        ++it;
        REQUIRE(it->first == 1);
        REQUIRE(it->second == "one");

        ++it;
        REQUIRE(it == map.rend());
    }

    SECTION("crbegin() and crend() methods")
    {
        auto it = map.crbegin();
        REQUIRE(it->first == 3);
        REQUIRE(it->second == "three");

        ++it;
        REQUIRE(it->first == 2);
        REQUIRE(it->second == "two");

        ++it;
        REQUIRE(it->first == 1);
        REQUIRE(it->second == "one");

        ++it;
        REQUIRE(it == map.crend());
    }
}

TEST_CASE("Map capacity methods", "[map]")
{
    SECTION("isEmpty() method")
    {
        cpp_ex::Map<int, std::string> emptyMap;
        REQUIRE(emptyMap.isEmpty());

        cpp_ex::Map<int, std::string> nonEmptyMap = {{1, "one"}};
        REQUIRE_FALSE(nonEmptyMap.isEmpty());
    }

    SECTION("getSize() method")
    {
        cpp_ex::Map<int, std::string> map;
        REQUIRE(map.getSize() == 0);

        map[1] = "one";
        REQUIRE(map.getSize() == 1);

        map[2] = "two";
        REQUIRE(map.getSize() == 2);

        map.erase(1);
        REQUIRE(map.getSize() == 1);
    }

    SECTION("getMaxSize() method")
    {
        cpp_ex::Map<int, std::string> map;
        // Just check that it returns a reasonable value
        REQUIRE(map.getMaxSize() > 0);
    }
}

TEST_CASE("Map element access methods", "[map]")
{
    cpp_ex::Map<int, std::string> map = {
        {1, "one"},
        {2, "two"},
        {3, "three"}};

    SECTION("at() method")
    {
        REQUIRE(map.at(1) == "one");
        REQUIRE(map.at(2) == "two");
        REQUIRE(map.at(3) == "three");

        // Modifying through at()
        map.at(2) = "TWO";
        REQUIRE(map.at(2) == "TWO");

        // at() with non-existent key should throw
        REQUIRE_THROWS_AS(map.at(4), std::out_of_range);
    }

    SECTION("operator[] method")
    {
        REQUIRE(map[1] == "one");
        REQUIRE(map[2] == "two");
        REQUIRE(map[3] == "three");

        // Modifying through operator[]
        map[2] = "TWO";
        REQUIRE(map[2] == "TWO");

        // operator[] with non-existent key should create a new entry
        REQUIRE(map.getSize() == 3);
        map[4] = "four";
        REQUIRE(map.getSize() == 4);
        REQUIRE(map[4] == "four");
    }
}

TEST_CASE("Map modifier methods", "[map]")
{
    SECTION("clear() method")
    {
        cpp_ex::Map<int, std::string> map = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        REQUIRE(map.getSize() == 3);

        map.clear();

        REQUIRE(map.isEmpty());
        REQUIRE(map.getSize() == 0);
    }

    SECTION("insert() methods")
    {
        cpp_ex::Map<int, std::string> map;

        // Insert with value_type
        auto result1 = map.insert({1, "one"});
        REQUIRE(result1.second);
        REQUIRE(result1.first->first == 1);
        REQUIRE(result1.first->second == "one");

        // Insert with already existing key
        auto result2 = map.insert({1, "ONE"});
        REQUIRE_FALSE(result2.second);
        REQUIRE(map[1] == "one"); // Value should not change

        // Insert with hint
        auto it = map.insert(map.begin(), {2, "two"});
        REQUIRE(it->first == 2);
        REQUIRE(it->second == "two");

        // Insert range
        std::map<int, std::string> sourceMap = {
            {3, "three"},
            {4, "four"}};
        map.insert(sourceMap.begin(), sourceMap.end());

        REQUIRE(map.getSize() == 4);
        REQUIRE(map[3] == "three");
        REQUIRE(map[4] == "four");

        // Insert initializer list
        map.insert({{5, "five"},
                    {6, "six"}});

        REQUIRE(map.getSize() == 6);
        REQUIRE(map[5] == "five");
        REQUIRE(map[6] == "six");
    }

    SECTION("emplace() method")
    {
        cpp_ex::Map<int, std::string> map;

        auto result = map.emplace(1, "one");
        REQUIRE(result.second);
        REQUIRE(result.first->first == 1);
        REQUIRE(result.first->second == "one");

        REQUIRE(map.getSize() == 1);
        REQUIRE(map[1] == "one");
    }

    SECTION("emplaceHint() method")
    {
        cpp_ex::Map<int, std::string> map;

        auto it = map.emplaceHint(map.begin(), 1, "one");
        REQUIRE(it->first == 1);
        REQUIRE(it->second == "one");

        REQUIRE(map.getSize() == 1);
        REQUIRE(map[1] == "one");
    }

    SECTION("erase() methods")
    {
        cpp_ex::Map<int, std::string> map = {
            {1, "one"},
            {2, "two"},
            {3, "three"},
            {4, "four"},
            {5, "five"}};

        // Erase by iterator
        auto it = map.find(2);
        auto nextIt = map.erase(it);
        REQUIRE(nextIt->first == 3);
        REQUIRE(map.getSize() == 4);
        REQUIRE_FALSE(map.contains(2));

        // Erase by key
        auto count = map.erase(4);
        REQUIRE(count == 1);
        REQUIRE(map.getSize() == 3);
        REQUIRE_FALSE(map.contains(4));

        // Erase by range
        auto first = map.find(1);
        auto last = map.find(5);
        auto newIt = map.erase(first, last);
        REQUIRE(newIt->first == 5);
        REQUIRE(map.getSize() == 1);
        REQUIRE(map.contains(5));
    }

    SECTION("swap() method")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"}};

        cpp_ex::Map<int, std::string> map2 = {
            {3, "three"},
            {4, "four"},
            {5, "five"}};

        map1.swap(map2);

        REQUIRE(map1.getSize() == 3);
        REQUIRE(map1.contains(3));
        REQUIRE(map1.contains(4));
        REQUIRE(map1.contains(5));

        REQUIRE(map2.getSize() == 2);
        REQUIRE(map2.contains(1));
        REQUIRE(map2.contains(2));
    }
}

TEST_CASE("Map lookup methods", "[map]")
{
    cpp_ex::Map<int, std::string> map = {
        {1, "one"},
        {2, "two"},
        {3, "three"}};

    SECTION("count() method")
    {
        REQUIRE(map.count(1) == 1);
        REQUIRE(map.count(2) == 1);
        REQUIRE(map.count(4) == 0);
    }

    SECTION("find() method")
    {
        auto it1 = map.find(2);
        REQUIRE(it1 != map.end());
        REQUIRE(it1->first == 2);
        REQUIRE(it1->second == "two");

        auto it2 = map.find(4);
        REQUIRE(it2 == map.end());
    }

    SECTION("contains() method")
    {
        REQUIRE(map.contains(1));
        REQUIRE(map.contains(2));
        REQUIRE(map.contains(3));
        REQUIRE_FALSE(map.contains(4));
    }

    SECTION("equalRange() method")
    {
        auto range = map.equalRange(2);
        REQUIRE(range.first != range.second);
        REQUIRE(range.first->first == 2);
        REQUIRE(range.first->second == "two");

        auto nonExistentRange = map.equalRange(4);
        REQUIRE(nonExistentRange.first == nonExistentRange.second);
    }

    SECTION("lowerBound() method")
    {
        auto it1 = map.lowerBound(2);
        REQUIRE(it1 != map.end());
        REQUIRE(it1->first == 2);

        auto it2 = map.lowerBound(4);
        REQUIRE(it2 == map.end());
    }

    SECTION("upperBound() method")
    {
        auto it1 = map.upperBound(2);
        REQUIRE(it1 != map.end());
        REQUIRE(it1->first == 3);

        auto it2 = map.upperBound(3);
        REQUIRE(it2 == map.end());
    }
}

TEST_CASE("Map observer methods", "[map]")
{
    cpp_ex::Map<int, std::string> map;

    SECTION("keyComp() method")
    {
        auto comp = map.keyComp();
        REQUIRE(comp(1, 2));
        REQUIRE_FALSE(comp(2, 1));
        REQUIRE_FALSE(comp(2, 2));
    }

    SECTION("valueComp() method")
    {
        auto comp = map.valueComp();
        REQUIRE(comp({1, "a"}, {2, "b"}));
        REQUIRE_FALSE(comp({2, "a"}, {1, "b"}));
        REQUIRE_FALSE(comp({2, "a"}, {2, "b"}));
    }
}

TEST_CASE("Map comparison operators", "[map]")
{
    cpp_ex::Map<int, std::string> map1 = {
        {1, "one"},
        {2, "two"},
        {3, "three"}};

    cpp_ex::Map<int, std::string> map2 = {
        {1, "one"},
        {2, "two"},
        {3, "three"}};

    cpp_ex::Map<int, std::string> map3 = {
        {1, "one"},
        {2, "TWO"},
        {3, "three"}};

    cpp_ex::Map<int, std::string> map4 = {
        {1, "one"},
        {2, "two"}};

    SECTION("operator== and operator!=")
    {
        REQUIRE(map1 == map2);
        REQUIRE_FALSE(map1 != map2);

        REQUIRE_FALSE(map1 == map3);
        REQUIRE(map1 != map3);

        REQUIRE_FALSE(map1 == map4);
        REQUIRE(map1 != map4);
    }

    SECTION("operator<, operator<=, operator>, operator>=")
    {
        REQUIRE_FALSE(map1 < map2);
        REQUIRE(map1 <= map2);
        REQUIRE_FALSE(map1 > map2);
        REQUIRE(map1 >= map2);

        REQUIRE(map3 < map1);
        REQUIRE(map3 <= map1);
        REQUIRE_FALSE(map3 > map1);
        REQUIRE_FALSE(map3 >= map1);

        REQUIRE_FALSE(map1 < map4);
        REQUIRE_FALSE(map1 <= map4);
        REQUIRE(map1 > map4);
        REQUIRE(map1 >= map4);
    }
}

TEST_CASE("Map additional methods", "[map]")
{
    cpp_ex::Map<int, std::string> map = {
        {1, "one"},
        {2, "two"},
        {3, "three"}};

    SECTION("getKeys() method")
    {
        auto keys = map.getKeys();

        REQUIRE(keys.getSize() == 3);
        REQUIRE(keys[0] == 1);
        REQUIRE(keys[1] == 2);
        REQUIRE(keys[2] == 3);
    }

    SECTION("getValues() method")
    {
        auto values = map.getValues();

        REQUIRE(values.getSize() == 3);
        REQUIRE(values[0] == "one");
        REQUIRE(values[1] == "two");
        REQUIRE(values[2] == "three");
    }

    SECTION("getEntries() method")
    {
        auto entries = map.getEntries();

        REQUIRE(entries.getSize() == 3);
        REQUIRE(entries[0].first == 1);
        REQUIRE(entries[0].second == "one");
        REQUIRE(entries[1].first == 2);
        REQUIRE(entries[1].second == "two");
        REQUIRE(entries[2].first == 3);
        REQUIRE(entries[2].second == "three");
    }

    SECTION("mapValues() method")
    {
        auto lengthMap = map.mapValues<size_t>([](const std::string &str)
                                               { return str.length(); });

        REQUIRE(lengthMap.getSize() == 3);
        REQUIRE(lengthMap[1] == 3); // "one" has length 3
        REQUIRE(lengthMap[2] == 3); // "two" has length 3
        REQUIRE(lengthMap[3] == 5); // "three" has length 5
    }

    SECTION("filterEntries() method")
    {
        auto filteredMap = map.filterEntries([](int key, const std::string &value)
                                             { return key % 2 == 1 && value.length() > 3; });

        REQUIRE(filteredMap.getSize() == 1);
        REQUIRE(filteredMap.contains(3));
        REQUIRE(filteredMap[3] == "three");
    }

    SECTION("forEach() method")
    {
        int sum = 0;
        map.forEach([&sum](int key, const std::string &value)
                    { sum += key; });

        REQUIRE(sum == 6); // 1 + 2 + 3

        // Test const version
        const auto &constMap = map;
        std::string concatenated;
        constMap.forEach([&concatenated](int key, const std::string &value)
                         { concatenated += value; });

        REQUIRE(concatenated == "onetwothree");
    }

    SECTION("merge() method")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"}};

        cpp_ex::Map<int, std::string> map2 = {
            {2, "TWO"},
            {3, "three"}};

        auto mergedMap = map1.merge(map2);

        REQUIRE(mergedMap.getSize() == 3);
        REQUIRE(mergedMap[1] == "one");
        REQUIRE(mergedMap[2] == "two"); // map1's value for key 2 is preserved
        REQUIRE(mergedMap[3] == "three");
    }

    SECTION("difference() method")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        cpp_ex::Map<int, std::string> map2 = {
            {2, "TWO"},
            {3, "THREE"},
            {4, "four"}};

        auto diffMap = map1.difference(map2);

        REQUIRE(diffMap.getSize() == 1);
        REQUIRE(diffMap.contains(1));
        REQUIRE(diffMap[1] == "one");
    }

    SECTION("intersection() method")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"},
            {3, "three"}};

        cpp_ex::Map<int, std::string> map2 = {
            {2, "TWO"},
            {3, "THREE"},
            {4, "four"}};

        auto intersectionMap = map1.intersection(map2);

        REQUIRE(intersectionMap.getSize() == 2);
        REQUIRE(intersectionMap.contains(2));
        REQUIRE(intersectionMap.contains(3));
        REQUIRE(intersectionMap[2] == "two"); // map1's values are used
        REQUIRE(intersectionMap[3] == "three");
    }
}

TEST_CASE("Map non-member functions", "[map]")
{
    SECTION("swap() function")
    {
        cpp_ex::Map<int, std::string> map1 = {
            {1, "one"},
            {2, "two"}};

        cpp_ex::Map<int, std::string> map2 = {
            {3, "three"},
            {4, "four"},
            {5, "five"}};

        swap(map1, map2);

        REQUIRE(map1.getSize() == 3);
        REQUIRE(map1.contains(3));
        REQUIRE(map1.contains(4));
        REQUIRE(map1.contains(5));

        REQUIRE(map2.getSize() == 2);
        REQUIRE(map2.contains(1));
        REQUIRE(map2.contains(2));
    }
}