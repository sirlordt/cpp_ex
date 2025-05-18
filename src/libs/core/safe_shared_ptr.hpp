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
     * @tparam T Type of the object managed by the pointer
     */
    template <typename T>
    class SafeSharedPtr
    {
    private:
        std::shared_ptr<T> ptr;

    public:
        // Constructors
        SafeSharedPtr() noexcept = default;

        template <typename U>
        explicit SafeSharedPtr(U *p) : ptr(p) {}

        SafeSharedPtr(std::nullptr_t) noexcept : ptr(nullptr) {}

        SafeSharedPtr(const SafeSharedPtr &other) noexcept : ptr(other.ptr) {}

        template <typename U>
        SafeSharedPtr(const SafeSharedPtr<U> &other) noexcept : ptr(other.ptr) {}

        SafeSharedPtr(SafeSharedPtr &&other) noexcept : ptr(std::move(other.ptr)) {}

        template <typename U>
        SafeSharedPtr(SafeSharedPtr<U> &&other) noexcept : ptr(std::move(other.ptr)) {}

        template <typename U>
        SafeSharedPtr(const std::shared_ptr<U> &other) noexcept : ptr(other) {}

        // Assignment operators
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

        // Methods for shared_ptr compatibility
        void reset() noexcept
        {
            ptr.reset();
        }

        template <typename U>
        void reset(U *p)
        {
            ptr.reset(p);
        }

        long useCount() const noexcept
        {
            return ptr.use_count();
        }

        // Swap with another SafeSharedPtr
        void swap(SafeSharedPtr &other) noexcept
        {
            ptr.swap(other.ptr);
        }

        // Get the underlying shared_ptr
        std::shared_ptr<T> toSharedPtr() const noexcept
        {
            return ptr;
        }
    };

    // Helper functions to create SafeSharedPtr
    template <typename T, typename... Args>
    SafeSharedPtr<T> makeSafeShared(Args &&...args)
    {
        return SafeSharedPtr<T>(std::make_shared<T>(std::forward<Args>(args)...));
    }

    template <typename T, typename U>
    SafeSharedPtr<T> safeStaticPointerCast(const SafeSharedPtr<U> &r) noexcept
    {
        return SafeSharedPtr<T>(std::static_pointer_cast<T>(r.toSharedPtr()));
    }

    template <typename T, typename U>
    SafeSharedPtr<T> safeDynamicPointerCast(const SafeSharedPtr<U> &r) noexcept
    {
        return SafeSharedPtr<T>(std::dynamic_pointer_cast<T>(r.toSharedPtr()));
    }

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
