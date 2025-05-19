// Define CATCH_CONFIG_NO_POSIX_SIGNALS before including Catch2
// #define CATCH_CONFIG_NO_POSIX_SIGNALS

// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "../../src/libs/core/safe_shared_ptr.hpp"
#include <string>
#include <memory>

// Test class for shared pointer tests
class TestClass
{
public:
    int value;
    explicit TestClass(int val) : value(val) {}
    int getValue() const { return value; }
    void setValue(int val) { value = val; }
};

TEST_CASE("SafeSharedPtr constructors", "[safe_shared_ptr]")
{
    SECTION("Default constructor creates null pointer")
    {
        cpp_ex::SafeSharedPtr<int> ptr;
        REQUIRE(ptr.isNull());
        REQUIRE_FALSE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Constructor with raw pointer")
    {
        int *rawPtr = new int(42);
        cpp_ex::SafeSharedPtr<int> ptr(rawPtr);
        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == rawPtr);
        REQUIRE(*ptr == 42);
    }

    SECTION("Constructor with nullptr")
    {
        cpp_ex::SafeSharedPtr<int> ptr(nullptr);
        REQUIRE(ptr.isNull());
        REQUIRE_FALSE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Copy constructor")
    {
        cpp_ex::SafeSharedPtr<int> ptr1(new int(42));
        cpp_ex::SafeSharedPtr<int> ptr2(ptr1);

        REQUIRE_FALSE(ptr1.isNull());
        REQUIRE_FALSE(ptr2.isNull());
        REQUIRE(*ptr1 == 42);
        REQUIRE(*ptr2 == 42);
        REQUIRE(ptr1.get() == ptr2.get());
        REQUIRE(ptr1.useCount() == 2);
        REQUIRE(ptr2.useCount() == 2);
    }

    SECTION("Move constructor")
    {
        cpp_ex::SafeSharedPtr<int> ptr1(new int(42));
        cpp_ex::SafeSharedPtr<int> ptr2(std::move(ptr1));

        REQUIRE(ptr1.isNull());
        REQUIRE_FALSE(ptr2.isNull());
        REQUIRE(*ptr2 == 42);
        REQUIRE(ptr2.useCount() == 1);
    }

    SECTION("Constructor from std::shared_ptr")
    {
        std::shared_ptr<int> stdPtr = std::make_shared<int>(42);
        cpp_ex::SafeSharedPtr<int> ptr(stdPtr);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(*ptr == 42);
        REQUIRE(ptr.get() == stdPtr.get());
        REQUIRE(ptr.useCount() == 2);
        REQUIRE(stdPtr.use_count() == 2);
    }
}

TEST_CASE("SafeSharedPtr assignment operators", "[safe_shared_ptr]")
{
    SECTION("Copy assignment operator")
    {
        cpp_ex::SafeSharedPtr<int> ptr1(new int(42));
        cpp_ex::SafeSharedPtr<int> ptr2;

        ptr2 = ptr1;

        REQUIRE_FALSE(ptr1.isNull());
        REQUIRE_FALSE(ptr2.isNull());
        REQUIRE(*ptr1 == 42);
        REQUIRE(*ptr2 == 42);
        REQUIRE(ptr1.get() == ptr2.get());
        REQUIRE(ptr1.useCount() == 2);
        REQUIRE(ptr2.useCount() == 2);
    }

    SECTION("Move assignment operator")
    {
        cpp_ex::SafeSharedPtr<int> ptr1(new int(42));
        cpp_ex::SafeSharedPtr<int> ptr2;

        ptr2 = std::move(ptr1);

        REQUIRE(ptr1.isNull());
        REQUIRE_FALSE(ptr2.isNull());
        REQUIRE(*ptr2 == 42);
        REQUIRE(ptr2.useCount() == 1);
    }

    SECTION("nullptr assignment operator")
    {
        cpp_ex::SafeSharedPtr<int> ptr(new int(42));

        ptr = nullptr;

        REQUIRE(ptr.isNull());
        REQUIRE_FALSE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("std::shared_ptr assignment operator")
    {
        std::shared_ptr<int> stdPtr = std::make_shared<int>(42);
        cpp_ex::SafeSharedPtr<int> ptr;

        ptr = stdPtr;

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(*ptr == 42);
        REQUIRE(ptr.get() == stdPtr.get());
        REQUIRE(ptr.useCount() == 2);
        REQUIRE(stdPtr.use_count() == 2);
    }
}

TEST_CASE("SafeSharedPtr access operators", "[safe_shared_ptr]")
{
    SECTION("Dereference operator (*) with valid pointer")
    {
        cpp_ex::SafeSharedPtr<int> ptr(new int(42));
        REQUIRE(*ptr == 42);

        *ptr = 100;
        REQUIRE(*ptr == 100);
    }

    SECTION("Dereference operator (*) with null pointer throws")
    {
        cpp_ex::SafeSharedPtr<int> ptr;
        REQUIRE_THROWS_AS(*ptr, cpp_ex::exceptions::NullPointerAccessException);
    }

    SECTION("Arrow operator (->) with valid pointer")
    {
        cpp_ex::SafeSharedPtr<TestClass> ptr(new TestClass(42));
        REQUIRE(ptr->getValue() == 42);

        ptr->setValue(100);
        REQUIRE(ptr->getValue() == 100);
    }

    SECTION("Arrow operator (->) with null pointer throws")
    {
        cpp_ex::SafeSharedPtr<TestClass> ptr;
        REQUIRE_THROWS_AS(ptr->getValue(), cpp_ex::exceptions::NullPointerAccessException);
    }
}

TEST_CASE("SafeSharedPtr utility methods", "[safe_shared_ptr]")
{
    SECTION("get() method")
    {
        int *rawPtr = new int(42);
        cpp_ex::SafeSharedPtr<int> ptr(rawPtr);

        REQUIRE(ptr.get() == rawPtr);
    }

    SECTION("isNull() method")
    {
        cpp_ex::SafeSharedPtr<int> nullPtr;
        cpp_ex::SafeSharedPtr<int> validPtr(new int(42));

        REQUIRE(nullPtr.isNull());
        REQUIRE_FALSE(validPtr.isNull());
    }

    SECTION("reset() method without arguments")
    {
        cpp_ex::SafeSharedPtr<int> ptr(new int(42));

        ptr.reset();

        REQUIRE(ptr.isNull());
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("reset() method with argument")
    {
        cpp_ex::SafeSharedPtr<int> ptr(new int(42));
        int *newRawPtr = new int(100);

        ptr.reset(newRawPtr);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(ptr.get() == newRawPtr);
        REQUIRE(*ptr == 100);
    }

    SECTION("useCount() method")
    {
        cpp_ex::SafeSharedPtr<int> ptr1(new int(42));
        REQUIRE(ptr1.useCount() == 1);

        {
            cpp_ex::SafeSharedPtr<int> ptr2 = ptr1;
            REQUIRE(ptr1.useCount() == 2);
            REQUIRE(ptr2.useCount() == 2);
        }

        REQUIRE(ptr1.useCount() == 1);
    }

    SECTION("swap() method")
    {
        cpp_ex::SafeSharedPtr<int> ptr1(new int(42));
        cpp_ex::SafeSharedPtr<int> ptr2(new int(100));

        ptr1.swap(ptr2);

        REQUIRE(*ptr1 == 100);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("toSharedPtr() method")
    {
        cpp_ex::SafeSharedPtr<int> safePtr(new int(42));
        std::shared_ptr<int> stdPtr = safePtr.toSharedPtr();

        REQUIRE(stdPtr.get() == safePtr.get());
        REQUIRE(*stdPtr == 42);
        REQUIRE(stdPtr.use_count() == 2);
    }
}

TEST_CASE("SafeSharedPtr helper functions", "[safe_shared_ptr]")
{
    SECTION("makeSafeShared function")
    {
        auto ptr = cpp_ex::makeSafeShared<int>(42);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(*ptr == 42);
        REQUIRE(ptr.useCount() == 1);
    }

    SECTION("makeSafeShared with custom class")
    {
        auto ptr = cpp_ex::makeSafeShared<TestClass>(42);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(ptr->getValue() == 42);
        REQUIRE(ptr.useCount() == 1);
    }

    SECTION("safeStaticPointerCast function")
    {
        class Base
        {
        public:
            virtual ~Base() = default;
        };

        class Derived : public Base
        {
        public:
            int value = 42;
        };

        auto derivedPtr = cpp_ex::makeSafeShared<Derived>();
        auto basePtr = cpp_ex::SafeSharedPtr<Base>(derivedPtr);

        auto castedPtr = cpp_ex::safeStaticPointerCast<Derived>(basePtr);

        REQUIRE_FALSE(castedPtr.isNull());
        REQUIRE(castedPtr->value == 42);
    }

    SECTION("safeDynamicPointerCast function")
    {
        class Base
        {
        public:
            virtual ~Base() = default;
        };

        class Derived : public Base
        {
        public:
            int value = 42;
        };

        auto derivedPtr = cpp_ex::makeSafeShared<Derived>();
        auto basePtr = cpp_ex::SafeSharedPtr<Base>(derivedPtr);

        auto castedPtr = cpp_ex::safeDynamicPointerCast<Derived>(basePtr);

        REQUIRE_FALSE(castedPtr.isNull());
        REQUIRE(castedPtr->value == 42);
    }

    SECTION("safeConstPointerCast function")
    {
        auto constPtr = cpp_ex::makeSafeShared<const int>(42);
        auto nonConstPtr = cpp_ex::safeConstPointerCast<int>(constPtr);

        REQUIRE_FALSE(nonConstPtr.isNull());
        REQUIRE(*nonConstPtr == 42);

        *nonConstPtr = 100;
        REQUIRE(*nonConstPtr == 100);
        REQUIRE(*constPtr == 100);
    }
}