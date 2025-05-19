// Define CATCH_CONFIG_NO_POSIX_SIGNALS before including Catch2
// #define CATCH_CONFIG_NO_POSIX_SIGNALS

// Include Catch2 headers
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "../../src/libs/core/safe_unique_ptr.hpp"
#include <string>
#include <memory>

// Test class for unique pointer tests
class TestClass
{
public:
    int value;
    explicit TestClass(int val) : value(val) {}
    int getValue() const { return value; }
    void setValue(int val) { value = val; }
};

// Custom deleter for testing
struct CustomDeleter
{
    bool *deleted;

    explicit CustomDeleter(bool *flag) : deleted(flag) {}

    void operator()(int *ptr)
    {
        *deleted = true;
        delete ptr;
    }
};

/* Temporarily commented out for testing */
TEST_CASE("SafeUniquePtr constructors full", "[safe_unique_ptr]")
{
    SECTION("Default constructor creates null pointer")
    {
        cpp_ex::SafeUniquePtr<int> ptr;
        REQUIRE(ptr.isNull());
        REQUIRE_FALSE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Constructor with raw pointer")
    {
        int *rawPtr = new int(42);
        cpp_ex::SafeUniquePtr<int> ptr(rawPtr);
        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == rawPtr);
        REQUIRE(*ptr == 42);
    }

    SECTION("Constructor with raw pointer and custom deleter")
    {
        bool deleted = false;
        int *rawPtr = new int(42);
        {
            cpp_ex::SafeUniquePtr<int, CustomDeleter> ptr(rawPtr, CustomDeleter(&deleted));
            REQUIRE_FALSE(ptr.isNull());
            REQUIRE(*ptr == 42);
        }
        // Check that the custom deleter was called
        REQUIRE(deleted);
    }

    SECTION("Constructor with nullptr")
    {
        cpp_ex::SafeUniquePtr<int> ptr(nullptr);
        REQUIRE(ptr.isNull());
        REQUIRE_FALSE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Move constructor")
    {
        cpp_ex::SafeUniquePtr<int> ptr1(new int(42));
        int *rawPtr = ptr1.get();
        cpp_ex::SafeUniquePtr<int> ptr2(std::move(ptr1));

        REQUIRE(ptr1.isNull());
        REQUIRE_FALSE(ptr2.isNull());
        REQUIRE(ptr2.get() == rawPtr);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("Converting move constructor")
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

        cpp_ex::SafeUniquePtr<Derived> derivedPtr(new Derived());
        cpp_ex::SafeUniquePtr<Base> basePtr(std::move(derivedPtr));

        REQUIRE(derivedPtr.isNull());
        REQUIRE_FALSE(basePtr.isNull());
    }
}

TEST_CASE("SafeUniquePtr assignment operators", "[safe_unique_ptr]")
{
    SECTION("Move assignment operator")
    {
        cpp_ex::SafeUniquePtr<int> ptr1(new int(42));
        int *rawPtr = ptr1.get();
        cpp_ex::SafeUniquePtr<int> ptr2;

        ptr2 = std::move(ptr1);

        REQUIRE(ptr1.isNull());
        REQUIRE_FALSE(ptr2.isNull());
        REQUIRE(ptr2.get() == rawPtr);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("Converting move assignment operator")
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

        cpp_ex::SafeUniquePtr<Derived> derivedPtr(new Derived());
        cpp_ex::SafeUniquePtr<Base> basePtr;

        basePtr = std::move(derivedPtr);

        REQUIRE(derivedPtr.isNull());
        REQUIRE_FALSE(basePtr.isNull());
    }

    SECTION("nullptr assignment operator")
    {
        cpp_ex::SafeUniquePtr<int> ptr(new int(42));

        ptr = nullptr;

        REQUIRE(ptr.isNull());
        REQUIRE_FALSE(static_cast<bool>(ptr));
        REQUIRE(ptr.get() == nullptr);
    }
}

TEST_CASE("SafeUniquePtr access operators", "[safe_unique_ptr]")
{
    SECTION("Dereference operator (*) with valid pointer")
    {
        cpp_ex::SafeUniquePtr<int> ptr(new int(42));
        REQUIRE(*ptr == 42);

        *ptr = 100;
        REQUIRE(*ptr == 100);
    }

    SECTION("Dereference operator (*) with null pointer throws")
    {
        cpp_ex::SafeUniquePtr<int> ptr;
        REQUIRE_THROWS_AS(*ptr, cpp_ex::exceptions::NullPointerAccessException);
    }

    SECTION("Arrow operator (->) with valid pointer")
    {
        cpp_ex::SafeUniquePtr<TestClass> ptr(new TestClass(42));
        REQUIRE(ptr->getValue() == 42);

        ptr->setValue(100);
        REQUIRE(ptr->getValue() == 100);
    }

    SECTION("Arrow operator (->) with null pointer throws")
    {
        cpp_ex::SafeUniquePtr<TestClass> ptr;
        REQUIRE_THROWS_AS(ptr->getValue(), cpp_ex::exceptions::NullPointerAccessException);
    }
}

TEST_CASE("SafeUniquePtr utility methods", "[safe_unique_ptr]")
{
    SECTION("get() method")
    {
        int *rawPtr = new int(42);
        cpp_ex::SafeUniquePtr<int> ptr(rawPtr);

        REQUIRE(ptr.get() == rawPtr);
    }

    SECTION("isNull() method")
    {
        cpp_ex::SafeUniquePtr<int> nullPtr;
        cpp_ex::SafeUniquePtr<int> validPtr(new int(42));

        REQUIRE(nullPtr.isNull());
        REQUIRE_FALSE(validPtr.isNull());
    }

    SECTION("reset() method without arguments")
    {
        cpp_ex::SafeUniquePtr<int> ptr(new int(42));

        ptr.reset();

        REQUIRE(ptr.isNull());
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("reset() method with argument")
    {
        cpp_ex::SafeUniquePtr<int> ptr(new int(42));
        int *newRawPtr = new int(100);

        ptr.reset(newRawPtr);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(ptr.get() == newRawPtr);
        REQUIRE(*ptr == 100);
    }

    SECTION("release() method")
    {
        int *rawPtr = new int(42);
        cpp_ex::SafeUniquePtr<int> ptr(rawPtr);

        int *releasedPtr = ptr.release();

        REQUIRE(ptr.isNull());
        REQUIRE(releasedPtr == rawPtr);
        REQUIRE(*releasedPtr == 42);

        // Clean up the released pointer
        delete releasedPtr;
    }

    SECTION("swap() method")
    {
        cpp_ex::SafeUniquePtr<int> ptr1(new int(42));
        cpp_ex::SafeUniquePtr<int> ptr2(new int(100));
        int *rawPtr1 = ptr1.get();
        int *rawPtr2 = ptr2.get();

        ptr1.swap(ptr2);

        REQUIRE(ptr1.get() == rawPtr2);
        REQUIRE(ptr2.get() == rawPtr1);
        REQUIRE(*ptr1 == 100);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("getDeleter() method")
    {
        bool deleted = false;
        cpp_ex::SafeUniquePtr<int, CustomDeleter> ptr(new int(42), CustomDeleter(&deleted));

        REQUIRE(ptr.getDeleter().deleted == &deleted);
    }

    SECTION("toUniquePtr() method")
    {
        cpp_ex::SafeUniquePtr<int> safePtr(new int(42));
        int *rawPtr = safePtr.get();

        std::unique_ptr<int> stdPtr = std::move(safePtr).toUniquePtr();

        REQUIRE(safePtr.isNull());
        REQUIRE(stdPtr.get() == rawPtr);
        REQUIRE(*stdPtr == 42);
    }
}

TEST_CASE("SafeUniquePtr helper functions", "[safe_unique_ptr]")
{
    SECTION("makeSafeUnique function")
    {
        auto ptr = cpp_ex::makeSafeUnique<int>(42);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(*ptr == 42);
    }

    SECTION("makeSafeUnique with custom class")
    {
        auto ptr = cpp_ex::makeSafeUnique<TestClass>(42);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(ptr->getValue() == 42);
    }
}

TEST_CASE("SafeUniquePtr array specialization", "[safe_unique_ptr]")
{
    SECTION("Constructor with raw array pointer")
    {
        int *rawArray = new int[5]{1, 2, 3, 4, 5};
        cpp_ex::SafeUniquePtr<int[]> ptr(rawArray);

        REQUIRE_FALSE(ptr.isNull());
        REQUIRE(ptr.get() == rawArray);
        REQUIRE(ptr[0] == 1);
        REQUIRE(ptr[1] == 2);
        REQUIRE(ptr[2] == 3);
        REQUIRE(ptr[3] == 4);
        REQUIRE(ptr[4] == 5);
    }

    SECTION("Array access operator with valid pointer")
    {
        std::cout << "Creating array for access operator test" << std::endl;
        int *rawArray = new int[3]{10, 20, 30};
        std::cout << "Raw array created at address " << static_cast<void *>(rawArray)
                  << " with size " << 3 * sizeof(int) << " bytes" << std::endl;

        cpp_ex::SafeUniquePtr<int[]> ptr(rawArray);
        std::cout << "SafeUniquePtr created with address " << static_cast<void *>(ptr.get()) << std::endl;

        std::cout << "Reading array elements" << std::endl;
        std::cout << "ptr[0] = " << ptr[0] << " at address " << &ptr[0] << std::endl;
        REQUIRE(ptr[0] == 10);

        std::cout << "ptr[1] = " << ptr[1] << " at address " << &ptr[1] << std::endl;
        REQUIRE(ptr[1] == 20);

        std::cout << "ptr[2] = " << ptr[2] << " at address " << &ptr[2] << std::endl;
        REQUIRE(ptr[2] == 30);

        std::cout << "Modifying array element" << std::endl;
        ptr[1] = 25;
        std::cout << "After modification: ptr[1] = " << ptr[1] << std::endl;
        REQUIRE(ptr[1] == 25);
    }

    SECTION("Array access operator with null pointer throws")
    {
        cpp_ex::SafeUniquePtr<int[]> ptr;

        REQUIRE_THROWS_AS(ptr[0], cpp_ex::exceptions::NullPointerAccessException);
    }

    SECTION("makeSafeUnique for arrays")
    {
        std::cout << "Creating array with makeSafeUnique<int[]>(5)" << std::endl;

        // Use makeSafeUnique to create the array
        auto ptr = cpp_ex::makeSafeUnique<int[]>(5);

        std::cout << "Pointer address: " << static_cast<void *>(ptr.get()) << std::endl;
        REQUIRE_FALSE(ptr.isNull());

        // Initialize array elements - add bounds check for safety
        for (int i = 0; i < 5; ++i)
        {
            // Print index for debugging
            std::cout << "Setting index " << i << " at address " << &ptr[i] << std::endl;
            ptr[i] = i * 10;
            std::cout << "  Value set: " << ptr[i] << std::endl;
        }

        // Verify array elements - add bounds check for safety
        for (int i = 0; i < 5; ++i)
        {
            // Print index and value for debugging
            std::cout << "Checking index " << i << " at address " << &ptr[i] << std::endl;
            std::cout << "  Value: " << ptr[i] << std::endl;
            REQUIRE(ptr[i] == i * 10);
        }
    }

    SECTION("Array specialization utility methods")
    {
        // Create an array with explicit initialization
        std::cout << "Creating initial array" << std::endl;
        cpp_ex::SafeUniquePtr<int[]> ptr(new int[3]{10, 20, 30});

        // Test reset()
        std::cout << "Testing reset()" << std::endl;
        ptr.reset();
        REQUIRE(ptr.isNull());

        // Test reset(T*)
        std::cout << "Testing reset(T*)" << std::endl;
        int *newArray = new int[3]{40, 50, 60};
        ptr.reset(newArray);
        REQUIRE_FALSE(ptr.isNull());

        std::cout << "Accessing array elements after reset" << std::endl;
        REQUIRE(ptr[0] == 40);
        REQUIRE(ptr[1] == 50);
        REQUIRE(ptr[2] == 60);

        // Test release()
        std::cout << "Testing release()" << std::endl;
        int *releasedArray = ptr.release();
        REQUIRE(ptr.isNull());
        REQUIRE(releasedArray == newArray);
        REQUIRE(releasedArray[0] == 40);

        // Clean up
        std::cout << "Cleaning up released array" << std::endl;
        delete[] releasedArray;

        // Test swap()
        std::cout << "Testing swap()" << std::endl;
        cpp_ex::SafeUniquePtr<int[]> ptr1(new int[2]{1, 2});
        cpp_ex::SafeUniquePtr<int[]> ptr2(new int[2]{3, 4});
        int *rawPtr1 = ptr1.get();
        int *rawPtr2 = ptr2.get();

        ptr1.swap(ptr2);

        REQUIRE(ptr1.get() == rawPtr2);
        REQUIRE(ptr2.get() == rawPtr1);

        std::cout << "Accessing array elements after swap" << std::endl;
        REQUIRE(ptr1[0] == 3);
        REQUIRE(ptr1[1] == 4);
        REQUIRE(ptr2[0] == 1);
        REQUIRE(ptr2[1] == 2);
    }
}
