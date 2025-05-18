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
         */
        class NullPointerAccessException : public std::runtime_error
        {
        public:
            NullPointerAccessException(const std::string &what_arg = "Null pointer access attempt")
                : std::runtime_error(what_arg) {}
        };

    } // namespace exceptions
} // namespace cpp_ex

#endif // CPPEX_COMMON_HPP
