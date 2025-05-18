#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// Include the try_catch_guard header
#include "try_catch_guard.hpp"

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

int main(int argc, char *argv[])
{
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

    std::cout << "\nProgram completed successfully!" << std::endl;
    return 0;
}
