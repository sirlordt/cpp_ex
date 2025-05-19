/**
 * @file common.hpp
 * @brief Common exceptions and utilities for the cpp_ex library
 * @author cpp_ex team
 * @date 2025-05-18
 */

#ifndef CPPEX_COMMON_HPP
#define CPPEX_COMMON_HPP

#include <stdexcept>
#include <string>

namespace cpp_ex
{
    namespace exceptions
    {

        /**
         * @brief Exception thrown when attempting to access a null pointer
         *
         * This exception is thrown by the safe pointer classes when an attempt
         * is made to dereference a null pointer, providing better error handling
         * than a segmentation fault.
         *
         * @example
         * ```cpp
         * try {
         *     cpp_ex::SafeSharedPtr<int> ptr; // null pointer
         *     int value = *ptr; // This will throw NullPointerAccessException
         * } catch (const cpp_ex::exceptions::NullPointerAccessException& e) {
         *     std::cout << "Exception caught: " << e.what() << std::endl;
         * }
         * ```
         */
        class NullPointerAccessException : public std::runtime_error
        {
        public:
            /**
             * @brief Construct a new NullPointerAccessException
             *
             * @param what_arg The error message (defaults to "Null pointer access attempt")
             */
            NullPointerAccessException(const std::string &what_arg = "Null pointer access attempt")
                : std::runtime_error(what_arg) {}
        };

    } // namespace exceptions
} // namespace cpp_ex

#endif // CPPEX_COMMON_HPP
