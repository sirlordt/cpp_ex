/**
 * @file safe_shared_ptr.hpp
 * @brief Safe shared pointer implementation that throws exceptions on null access
 * @author cpp_ex team
 * @date 2025-05-18
 */

#ifndef CPPEX_SAFE_SHARED_PTR_HPP
#define CPPEX_SAFE_SHARED_PTR_HPP

#include <memory>
#include <string>
#include <utility>

#include "common.hpp"

namespace cpp_ex
{

    /**
     * @brief Safe wrapper class for std::shared_ptr that throws exceptions
     *        when attempting to access a null pointer.
     *
     * This class provides a safer alternative to std::shared_ptr by throwing
     * a NullPointerAccessException when attempting to dereference a null pointer
     * instead of causing undefined behavior or program crashes.
     *
     * @tparam T Type of the object managed by the pointer
     *
     * @example
     * ```cpp
     * // Create a SafeSharedPtr with an object
     * auto ptr = cpp_ex::makeSafeShared<int>(42);
     * std::cout << "Value: " << *ptr << std::endl; // OK
     *
     * // Create a null SafeSharedPtr
     * cpp_ex::SafeSharedPtr<int> nullPtr;
     *
     * try {
     *     // This will throw an exception
     *     std::cout << "Value: " << *nullPtr << std::endl;
     * } catch (const cpp_ex::exceptions::NullPointerAccessException &e) {
     *     std::cout << "Exception caught: " << e.what() << std::endl;
     * }
     * ```
     */
    template <typename T>
    class SafeSharedPtr
    {
    private:
        std::shared_ptr<T> ptr;

        // Declare friendship with all other SafeSharedPtr instantiations
        template <typename U>
        friend class SafeSharedPtr;

    public:
        /**
         * @brief Default constructor, creates a null pointer
         */
        SafeSharedPtr() noexcept = default;

        /**
         * @brief Construct from a raw pointer
         *
         * @tparam U Type of the pointer (can be different from T if convertible)
         * @param p Raw pointer to manage
         */
        template <typename U>
        explicit SafeSharedPtr(U *p) : ptr(p) {}

        /**
         * @brief Construct from nullptr
         *
         * @param nullptr_t nullptr value
         */
        SafeSharedPtr(std::nullptr_t) noexcept : ptr(nullptr) {}

        /**
         * @brief Copy constructor
         *
         * @param other Another SafeSharedPtr to copy from
         */
        SafeSharedPtr(const SafeSharedPtr &other) noexcept : ptr(other.ptr) {}

        /**
         * @brief Converting copy constructor
         *
         * @tparam U Type of the other pointer (can be different from T if convertible)
         * @param other Another SafeSharedPtr to copy from
         */
        template <typename U>
        SafeSharedPtr(const SafeSharedPtr<U> &other) noexcept : ptr(other.ptr) {}

        /**
         * @brief Move constructor
         *
         * @param other Another SafeSharedPtr to move from
         */
        SafeSharedPtr(SafeSharedPtr &&other) noexcept : ptr(std::move(other.ptr)) {}

        /**
         * @brief Converting move constructor
         *
         * @tparam U Type of the other pointer (can be different from T if convertible)
         * @param other Another SafeSharedPtr to move from
         */
        template <typename U>
        SafeSharedPtr(SafeSharedPtr<U> &&other) noexcept : ptr(std::move(other.ptr)) {}

        /**
         * @brief Construct from std::shared_ptr
         *
         * @tparam U Type of the shared_ptr (can be different from T if convertible)
         * @param other A std::shared_ptr to copy from
         */
        template <typename U>
        SafeSharedPtr(const std::shared_ptr<U> &other) noexcept : ptr(other) {}

        /**
         * @brief Assignment operators
         */
        SafeSharedPtr &operator=(const SafeSharedPtr &other) noexcept
        {
            ptr = other.ptr;
            return *this;
        }

        template <typename U>
        SafeSharedPtr &operator=(const SafeSharedPtr<U> &other) noexcept
        {
            ptr = other.ptr;
            return *this;
        }

        SafeSharedPtr &operator=(SafeSharedPtr &&other) noexcept
        {
            ptr = std::move(other.ptr);
            return *this;
        }

        template <typename U>
        SafeSharedPtr &operator=(SafeSharedPtr<U> &&other) noexcept
        {
            ptr = std::move(other.ptr);
            return *this;
        }

        SafeSharedPtr &operator=(std::nullptr_t) noexcept
        {
            ptr = nullptr;
            return *this;
        }

        template <typename U>
        SafeSharedPtr &operator=(const std::shared_ptr<U> &other) noexcept
        {
            ptr = other;
            return *this;
        }

        /**
         * @brief Access operators with null check
         */
        T &operator*() const
        {
            if (!ptr)
            {
                throw exceptions::NullPointerAccessException();
            }
            return *ptr;
        }

        T *operator->() const
        {
            if (!ptr)
            {
                throw exceptions::NullPointerAccessException();
            }
            return ptr.get();
        }

        // Boolean conversion operator
        explicit operator bool() const noexcept
        {
            return static_cast<bool>(ptr);
        }

        // Access to underlying pointer
        T *get() const noexcept
        {
            return ptr.get();
        }

        // Check if pointer is null
        bool isNull() const noexcept
        {
            return !ptr;
        }

        /**
         * @brief Methods for shared_ptr compatibility
         */

        /**
         * @brief Reset the pointer to null
         */
        void reset() noexcept
        {
            ptr.reset();
        }

        /**
         * @brief Reset the pointer to a new raw pointer
         *
         * @tparam U Type of the pointer (can be different from T if convertible)
         * @param p Raw pointer to manage
         */
        template <typename U>
        void reset(U *p)
        {
            ptr.reset(p);
        }

        /**
         * @brief Get the number of shared_ptr objects referring to the same managed object
         *
         * @return long The number of shared_ptr instances sharing the managed object
         */
        long useCount() const noexcept
        {
            return ptr.use_count();
        }

        /**
         * @brief Swap the contents with another SafeSharedPtr
         *
         * @param other Another SafeSharedPtr to swap with
         */
        void swap(SafeSharedPtr &other) noexcept
        {
            ptr.swap(other.ptr);
        }

        /**
         * @brief Get the underlying std::shared_ptr
         *
         * @return std::shared_ptr<T> The underlying shared_ptr
         */
        std::shared_ptr<T> toSharedPtr() const noexcept
        {
            return ptr;
        }
    };

    /**
     * @brief Helper functions to create and manipulate SafeSharedPtr objects
     */

    /**
     * @brief Create a SafeSharedPtr with a newly constructed object
     *
     * This function is similar to std::make_shared but creates a SafeSharedPtr instead.
     * It forwards all arguments to the constructor of T.
     *
     * @tparam T Type of object to create
     * @tparam Args Types of arguments to forward to the constructor
     * @param args Arguments to forward to the constructor
     * @return SafeSharedPtr<T> A SafeSharedPtr managing a new instance of T
     *
     * @example
     * ```cpp
     * // Create an integer
     * auto intPtr = cpp_ex::makeSafeShared<int>(42);
     *
     * // Create a custom class
     * auto personPtr = cpp_ex::makeSafeShared<Person>("John", 30);
     * ```
     */
    template <typename T, typename... Args>
    SafeSharedPtr<T> makeSafeShared(Args &&...args)
    {
        return SafeSharedPtr<T>(std::make_shared<T>(std::forward<Args>(args)...));
    }

    /**
     * @brief Perform static_cast on a SafeSharedPtr
     *
     * Similar to std::static_pointer_cast but works with SafeSharedPtr.
     *
     * @tparam T Type to cast to
     * @tparam U Original type
     * @param r SafeSharedPtr to cast
     * @return SafeSharedPtr<T> A SafeSharedPtr of the new type
     */
    template <typename T, typename U>
    SafeSharedPtr<T> safeStaticPointerCast(const SafeSharedPtr<U> &r) noexcept
    {
        return SafeSharedPtr<T>(std::static_pointer_cast<T>(r.toSharedPtr()));
    }

    /**
     * @brief Perform dynamic_cast on a SafeSharedPtr
     *
     * Similar to std::dynamic_pointer_cast but works with SafeSharedPtr.
     *
     * @tparam T Type to cast to
     * @tparam U Original type
     * @param r SafeSharedPtr to cast
     * @return SafeSharedPtr<T> A SafeSharedPtr of the new type (may be null if cast fails)
     */
    template <typename T, typename U>
    SafeSharedPtr<T> safeDynamicPointerCast(const SafeSharedPtr<U> &r) noexcept
    {
        return SafeSharedPtr<T>(std::dynamic_pointer_cast<T>(r.toSharedPtr()));
    }

    /**
     * @brief Perform const_cast on a SafeSharedPtr
     *
     * Similar to std::const_pointer_cast but works with SafeSharedPtr.
     *
     * @tparam T Type to cast to
     * @tparam U Original type
     * @param r SafeSharedPtr to cast
     * @return SafeSharedPtr<T> A SafeSharedPtr of the new type
     */
    template <typename T, typename U>
    SafeSharedPtr<T> safeConstPointerCast(const SafeSharedPtr<U> &r) noexcept
    {
        return SafeSharedPtr<T>(std::const_pointer_cast<T>(r.toSharedPtr()));
    }

    /*
    // Usage example
    void example()
    {
        // Create a SafeSharedPtr with an object
        auto ptr = makeSafeShared<int>(42);
        std::cout << "Value: " << *ptr << std::endl; // OK

        // Create a null SafeSharedPtr
        SafeSharedPtr<int> nullPtr;

        try
        {
            // This will throw an exception
            std::cout << "Value: " << *nullPtr << std::endl;
        }
        catch (const exceptions::NullPointerAccessException &e)
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

        auto person = makeSafeShared<Person>("John");
        person->greet(); // OK

        SafeSharedPtr<Person> nullPerson;
        try
        {
            // This will throw an exception
            nullPerson->greet();
        }
        catch (const exceptions::NullPointerAccessException &e)
        {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
    }
    */

} // namespace cpp_ex

#endif // CPPEX_SAFE_SHARED_PTR_HPP
