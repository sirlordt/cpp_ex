#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// Include the try_catch_guard header
#include "try_catch_guard.hpp"

// Include the safe pointer headers
#include "libs/core/safe_shared_ptr.hpp"
#include "libs/core/safe_unique_ptr.hpp"
#include "libs/core/string.hpp"

// Function that will cause a null pointer exception
void causeNullPointerException()
{
    int *ptr = nullptr;
    *ptr = 42; // This will cause a segmentation fault
}

// Function that throws a standard exception
void throwStandardException()
{
    throw std::runtime_error("This is a standard exception");
}

void exampleMakeSafeShared()
{
    // Create a SafeSharedPtr with an object
    auto ptr = cpp_ex::makeSafeShared<int>(42);
    std::cout << "Value: " << *ptr << std::endl; // OK

    // Create a null SafeSharedPtr
    cpp_ex::SafeSharedPtr<int> nullPtr;

    _try
    {
        // This will throw an exception
        std::cout << "Value: " << *nullPtr << std::endl;
    }
    _catch(cpp_ex::exceptions::NullPointerAccessException, e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Example with a class
    class Person
    {
    public:
        std::string name;
        Person(const std::string &n) : name(n) {}
        void greet() { std::cout << "Hello, I am " << name << std::endl; }
    };

    auto person = cpp_ex::makeSafeShared<Person>("John");
    person->greet(); // OK

    cpp_ex::SafeSharedPtr<Person> nullPerson;
    _try
    {
        // This will throw an exception
        nullPerson->greet();
    }
    _catch(cpp_ex::exceptions::NullPointerAccessException, e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

// Usage example
void exampleMakeSafeUnique()
{
    // Create a SafeUniquePtr with an object
    auto ptr = cpp_ex::makeSafeUnique<int>(42);
    std::cout << "Value: " << *ptr << std::endl; // OK

    // Create a null SafeUniquePtr
    cpp_ex::SafeUniquePtr<int> nullPtr;

    _try
    {
        // This will throw an exception
        std::cout << "Value: " << *nullPtr << std::endl;
    }
    _catch(cpp_ex::exceptions::NullPointerAccessException, e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Example with a class
    class Person
    {
    public:
        std::string name;
        Person(const std::string &n) : name(n) {}
        void greet() { std::cout << "Hello, I am " << name << std::endl; }
    };

    auto person = cpp_ex::makeSafeUnique<Person>("John");
    person->greet(); // OK

    cpp_ex::SafeUniquePtr<Person> nullPerson;
    _try
    {
        // This will throw an exception
        nullPerson->greet();
    }
    _catch(cpp_ex::exceptions::NullPointerAccessException, e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // Array example
    auto arr = cpp_ex::makeSafeUnique<int[]>(5);
    arr[0] = 10; // Using the array access operator
    std::cout << "Array value: " << arr[0] << std::endl;
}

int main(int argc, char *argv[])
{

    cpp_ex::String str("hello");
    auto vec = str.toStringVector();

    // Print the content of the vector for debugging
    std::cout << "Vector size: " << vec.getSize() << std::endl;
    for (size_t i = 0; i < vec.getSize(); ++i)
    {
        std::cout << "vec[" << i << "] = '" << vec[i].getString() << "'" << std::endl;
    }

    std::cout << "Hello from C++ extended library!" << std::endl;

    std::vector<std::string> args(argv, argv + argc);

    if (args.size() > 1)
    {
        std::cout << "Arguments:" << std::endl;
        for (size_t i = 1; i < args.size(); ++i)
        {
            std::cout << "  " << i << ": " << args[i] << std::endl;
        }
    }

    // Test try_catch_guard with null pointer exception
    std::cout << "\nTesting try_catch_guard with null pointer exception:" << std::endl;
    _try
    {
        std::cout << "Inside _try block, about to cause null pointer exception..." << std::endl;
        causeNullPointerException();
        std::cout << "This line should not be reached." << std::endl;
    }
    _catch(try_catch_guard::InvalidMemoryAccessException, e)
    {
        std::cout << "Caught InvalidMemoryAccessException: " << e.what() << std::endl;
    }

    // Test try_catch_guard with standard exception
    std::cout << "\nTesting try_catch_guard with standard exception:" << std::endl;
    _try
    {
        std::cout << "Inside _try block, about to throw standard exception..." << std::endl;
        throwStandardException();
        std::cout << "This line should not be reached." << std::endl;
    }
    _catch(std::exception, e)
    {
        std::cout << "Caught standard exception: " << e.what() << std::endl;
    }

    std::cout << "\nTesting SafeSharedPtr:" << std::endl;
    exampleMakeSafeShared();

    std::cout << "\nTesting SafeUniquePtr:" << std::endl;
    exampleMakeSafeUnique();

    std::cout << "\nProgram completed successfully!" << std::endl;
    return 0;
}
