#ifndef CPPEX_SAFE_UNIQUE_PTR_HPP
#define CPPEX_SAFE_UNIQUE_PTR_HPP

#include <memory>
#include <string>
#include <utility>

#include "common.hpp"
#include <iostream>

namespace cpp_ex
{

    /**
     * @brief Safe wrapper class for std::unique_ptr that throws exceptions
     *        when attempting to access a null pointer.
     *
     * @tparam T Type of the object managed by the pointer
     * @tparam Deleter The custom deleter type, defaults to std::default_delete<T>
     */
    template <typename T, typename Deleter = std::default_delete<T>>
    class SafeUniquePtr
    {
    private:
        std::unique_ptr<T, Deleter> ptr;

    public:
        // Constructors
        SafeUniquePtr() noexcept = default;

        explicit SafeUniquePtr(T *p) noexcept : ptr(p) {}

        SafeUniquePtr(T *p, const Deleter &d) noexcept : ptr(p, d) {}

        SafeUniquePtr(T *p, Deleter &&d) noexcept : ptr(p, std::move(d)) {}

        SafeUniquePtr(std::nullptr_t) noexcept : ptr(nullptr) {}

        SafeUniquePtr(SafeUniquePtr &&other) noexcept : ptr(std::move(other.ptr)) {}

        template <typename U, typename E>
        SafeUniquePtr(SafeUniquePtr<U, E> &&other) noexcept : ptr(std::move(other.ptr)) {}

        // Deleted copy constructor (unique_ptr is move-only)
        SafeUniquePtr(const SafeUniquePtr &) = delete;

        // Assignment operators
        SafeUniquePtr &operator=(SafeUniquePtr &&other) noexcept
        {
            ptr = std::move(other.ptr);
            return *this;
        }

        template <typename U, typename E>
        SafeUniquePtr &operator=(SafeUniquePtr<U, E> &&other) noexcept
        {
            ptr = std::move(other.ptr);
            return *this;
        }

        SafeUniquePtr &operator=(std::nullptr_t) noexcept
        {
            ptr = nullptr;
            return *this;
        }

        // Deleted copy assignment (unique_ptr is move-only)
        SafeUniquePtr &operator=(const SafeUniquePtr &) = delete;

        // Access operators (with null check)
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

        // Methods for unique_ptr compatibility
        void reset() noexcept
        {
            ptr.reset();
        }

        void reset(T *p) noexcept
        {
            ptr.reset(p);
        }

        template <typename U>
        void reset(U *p) noexcept
        {
            ptr.reset(p);
        }

        T *release() noexcept
        {
            return ptr.release();
        }

        // Swap with another SafeUniquePtr
        void swap(SafeUniquePtr &other) noexcept
        {
            ptr.swap(other.ptr);
        }

        // Get the deleter
        const Deleter &getDeleter() const noexcept
        {
            return ptr.get_deleter();
        }

        Deleter &getDeleter() noexcept
        {
            return ptr.get_deleter();
        }

        // Convert to std::unique_ptr (transfers ownership)
        std::unique_ptr<T, Deleter> toUniquePtr() && noexcept
        {
            return std::move(ptr);
        }
    };

    // Helper functions to create SafeUniquePtr
    template <typename T, typename... Args>
    SafeUniquePtr<T> makeSafeUnique(Args &&...args)
    {
        return SafeUniquePtr<T>(new T(std::forward<Args>(args)...));
    }

    // C++14 array specialization helper
    template <typename T>
    SafeUniquePtr<T[]> makeSafeUnique(size_t size)
    {
        return SafeUniquePtr<T[]>(new T[size]);
    }

    // Specialization of SafeUniquePtr for arrays
    template <typename T>
    class SafeUniquePtr<T[]>
    {
    private:
        std::unique_ptr<T[]> ptr;

    public:
        // Constructors
        SafeUniquePtr() noexcept = default;

        explicit SafeUniquePtr(T *p) noexcept : ptr(p) {}

        SafeUniquePtr(std::nullptr_t) noexcept : ptr(nullptr) {}

        SafeUniquePtr(SafeUniquePtr &&other) noexcept : ptr(std::move(other.ptr)) {}

        // Deleted copy constructor (unique_ptr is move-only)
        SafeUniquePtr(const SafeUniquePtr &) = delete;

        // Assignment operators
        SafeUniquePtr &operator=(SafeUniquePtr &&other) noexcept
        {
            ptr = std::move(other.ptr);
            return *this;
        }

        SafeUniquePtr &operator=(std::nullptr_t) noexcept
        {
            ptr = nullptr;
            return *this;
        }

        // Deleted copy assignment (unique_ptr is move-only)
        SafeUniquePtr &operator=(const SafeUniquePtr &) = delete;

        // Array access operator (with null check)
        T &operator[](size_t i) const
        {
            if (!ptr)
            {
                throw exceptions::NullPointerAccessException();
            }
            return ptr[i];
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

        // Methods for unique_ptr compatibility
        void reset() noexcept
        {
            ptr.reset();
        }

        void reset(T *p) noexcept
        {
            ptr.reset(p);
        }

        T *release() noexcept
        {
            return ptr.release();
        }

        // Swap with another SafeUniquePtr
        void swap(SafeUniquePtr &other) noexcept
        {
            ptr.swap(other.ptr);
        }
    };

    /*
    // Usage example
    void example()
    {
        // Create a SafeUniquePtr with an object
        auto ptr = makeSafeUnique<int>(42);
        std::cout << "Value: " << *ptr << std::endl; // OK

        // Create a null SafeUniquePtr
        SafeUniquePtr<int> nullPtr;

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

        auto person = makeSafeUnique<Person>("John");
        person->greet(); // OK

        SafeUniquePtr<Person> nullPerson;
        try
        {
            // This will throw an exception
            nullPerson->greet();
        }
        catch (const exceptions::NullPointerAccessException &e)
        {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }

        // Array example
        auto arr = makeSafeUnique<int[]>(5);
        arr.get()[0] = 10;
        std::cout << "Array value: " << arr.get()[0] << std::endl;
    }
    */

} // namespace cpp_ex

#endif // CPPEX_SAFE_UNIQUE_PTR_HPP
