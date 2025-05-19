/**
 * @file safe_unique_ptr.hpp
 * @brief Safe unique pointer implementation that throws exceptions on null access
 * @author cpp_ex team
 * @date 2025-05-18
 */

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
     * This class provides a safer alternative to std::unique_ptr by throwing
     * a NullPointerAccessException when attempting to dereference a null pointer
     * instead of causing undefined behavior or program crashes.
     *
     * @tparam T Type of the object managed by the pointer
     * @tparam Deleter The custom deleter type, defaults to std::default_delete<T>
     *
     * @example
     * ```cpp
     * // Create a SafeUniquePtr with an object
     * auto ptr = cpp_ex::makeSafeUnique<int>(42);
     * std::cout << "Value: " << *ptr << std::endl; // OK
     *
     * // Create a null SafeUniquePtr
     * cpp_ex::SafeUniquePtr<int> nullPtr;
     *
     * try {
     *     // This will throw an exception
     *     std::cout << "Value: " << *nullPtr << std::endl;
     * } catch (const cpp_ex::exceptions::NullPointerAccessException &e) {
     *     std::cout << "Exception caught: " << e.what() << std::endl;
     * }
     * ```
     */
    template <typename T, typename Deleter = std::default_delete<T>>
    class SafeUniquePtr
    {
    private:
        std::unique_ptr<T, Deleter> ptr;

        // Declare friendship with all other SafeUniquePtr instantiations
        template <typename U, typename E>
        friend class SafeUniquePtr;

    public:
        /**
         * @brief Constructors
         */

        /**
         * @brief Default constructor, creates a null pointer
         */
        SafeUniquePtr() noexcept = default;

        /**
         * @brief Construct from a raw pointer
         *
         * @param p Raw pointer to manage
         */
        explicit SafeUniquePtr(T *p) noexcept : ptr(p) {}

        /**
         * @brief Construct from a raw pointer and a deleter
         *
         * @param p Raw pointer to manage
         * @param d Deleter to use
         */
        SafeUniquePtr(T *p, const Deleter &d) noexcept : ptr(p, d) {}

        /**
         * @brief Construct from a raw pointer and a movable deleter
         *
         * @param p Raw pointer to manage
         * @param d Deleter to use (will be moved)
         */
        SafeUniquePtr(T *p, Deleter &&d) noexcept : ptr(p, std::move(d)) {}

        /**
         * @brief Construct from nullptr
         *
         * @param nullptr_t nullptr value
         */
        SafeUniquePtr(std::nullptr_t) noexcept : ptr(nullptr) {}

        /**
         * @brief Move constructor
         *
         * @param other Another SafeUniquePtr to move from
         */
        SafeUniquePtr(SafeUniquePtr &&other) noexcept : ptr(std::move(other.ptr)) {}

        /**
         * @brief Converting move constructor
         *
         * @tparam U Type of the other pointer (can be different from T if convertible)
         * @tparam E Type of the other deleter (can be different from Deleter if convertible)
         * @param other Another SafeUniquePtr to move from
         */
        template <typename U, typename E>
        SafeUniquePtr(SafeUniquePtr<U, E> &&other) noexcept : ptr(std::move(other.ptr)) {}

        /**
         * @brief Deleted copy constructor (unique_ptr is move-only)
         */
        SafeUniquePtr(const SafeUniquePtr &) = delete;

        /**
         * @brief Assignment operators
         */
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
         * @brief Methods for unique_ptr compatibility
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
         * @param p Raw pointer to manage
         */
        void reset(T *p) noexcept
        {
            ptr.reset(p);
        }

        /**
         * @brief Reset the pointer to a new raw pointer of a different type
         *
         * @tparam U Type of the pointer (can be different from T if convertible)
         * @param p Raw pointer to manage
         */
        template <typename U>
        void reset(U *p) noexcept
        {
            ptr.reset(p);
        }

        /**
         * @brief Release ownership of the managed object
         *
         * @return T* Pointer to the managed object (now unmanaged)
         */
        T *release() noexcept
        {
            return ptr.release();
        }

        /**
         * @brief Swap the contents with another SafeUniquePtr
         *
         * @param other Another SafeUniquePtr to swap with
         */
        void swap(SafeUniquePtr &other) noexcept
        {
            ptr.swap(other.ptr);
        }

        /**
         * @brief Get the deleter object (const version)
         *
         * @return const Deleter& Reference to the deleter
         */
        const Deleter &getDeleter() const noexcept
        {
            return ptr.get_deleter();
        }

        /**
         * @brief Get the deleter object (non-const version)
         *
         * @return Deleter& Reference to the deleter
         */
        Deleter &getDeleter() noexcept
        {
            return ptr.get_deleter();
        }

        /**
         * @brief Convert to std::unique_ptr (transfers ownership)
         *
         * This is an rvalue reference qualified method, meaning it can only
         * be called on temporary objects or with std::move.
         *
         * @return std::unique_ptr<T, Deleter> The underlying unique_ptr
         */
        std::unique_ptr<T, Deleter> toUniquePtr() && noexcept
        {
            return std::move(ptr);
        }
    };

    /**
     * @brief Helper functions to create SafeUniquePtr objects
     */

    /**
     * @brief Create a SafeUniquePtr with a newly constructed object
     *
     * This function is similar to std::make_unique but creates a SafeUniquePtr instead.
     * It forwards all arguments to the constructor of T.
     *
     * @tparam T Type of object to create
     * @tparam Args Types of arguments to forward to the constructor
     * @param args Arguments to forward to the constructor
     * @return SafeUniquePtr<T> A SafeUniquePtr managing a new instance of T
     *
     * @example
     * ```cpp
     * // Create an integer
     * auto intPtr = cpp_ex::makeSafeUnique<int>(42);
     *
     * // Create a custom class
     * auto personPtr = cpp_ex::makeSafeUnique<Person>("John", 30);
     * ```
     */
    template <typename T, typename... Args>
    std::enable_if_t<!std::is_array_v<T>, SafeUniquePtr<T>> makeSafeUnique(Args &&...args)
    {
        return SafeUniquePtr<T>(new T(std::forward<Args>(args)...));
    }

    /**
     * @brief Create a SafeUniquePtr managing a dynamically allocated array
     *
     * This function is similar to std::make_unique for arrays but creates a SafeUniquePtr instead.
     *
     * @tparam T Type of array elements
     * @param size Number of elements in the array
     * @return SafeUniquePtr<T[]> A SafeUniquePtr managing a new array of T
     *
     * @example
     * ```cpp
     * // Create an array of 5 integers
     * auto arr = cpp_ex::makeSafeUnique<int[]>(5);
     * arr[0] = 10; // Access using the safe array operator
     * ```
     */
    /**
     * @brief Create a SafeUniquePtr managing a dynamically allocated array
     *
     * This function creates a SafeUniquePtr that manages a dynamically allocated array
     * of the specified size. The array is initialized with default values.
     *
     * @tparam T Type of array elements
     * @param size Number of elements in the array
     * @return SafeUniquePtr<T[]> A SafeUniquePtr managing a new array of T
     *
     * @example
     * ```cpp
     * // Create an array of 5 integers
     * auto arr = cpp_ex::makeSafeUniqueArray<int>(5);
     * arr[0] = 10; // Access using the safe array operator
     * ```
     */
    // SafeUniquePtr<T[]> makeSafeUniqueArray(size_t size)
    template <typename T>
    SafeUniquePtr<std::remove_extent_t<T>[]> makeSafeUnique(size_t size)
    {
        // Ensure we're allocating at least one element
        if (size == 0)
        {
            size = 1;
        }

        /*
        // Debug output to verify the size
        std::cout << "makeSafeUnique<T[]>: Allocating array of size " << size
                  << " elements, total size = " << (size * sizeof(std::remove_extent_t<T>)) << " bytes" << std::endl;
        */

        // Allocate the array with default initialization
        std::remove_extent_t<T> *arr = new std::remove_extent_t<T>[size]();

        /*
        // Verify the allocation by checking addresses
        std::cout << "makeSafeUnique<T[]>: Allocated array at address " << static_cast<void *>(arr) << std::endl;
        for (size_t i = 0; i < size && i < 10; ++i)
        {
            std::cout << "  Element " << i << " at address " << static_cast<void *>(&arr[i])
                      << " (offset: " << (i * sizeof(std::remove_extent_t<T>)) << " bytes)" << std::endl;
        }
        */

        // Return a SafeUniquePtr that manages the array
        return SafeUniquePtr<std::remove_extent_t<T>[]>(arr);
    }

    /**
     * @brief Specialization of SafeUniquePtr for arrays
     *
     * This specialization provides array-specific functionality, including
     * the array access operator with null pointer checks.
     *
     * @tparam T Type of array elements
     */
    template <typename T>
    class SafeUniquePtr<T[]>
    {
    private:
        std::unique_ptr<T[]> ptr;

    public:
        /**
         * @brief Constructors for array specialization
         */
        SafeUniquePtr() noexcept = default;

        explicit SafeUniquePtr(T *p) noexcept : ptr(p) {}

        SafeUniquePtr(std::nullptr_t) noexcept : ptr(nullptr) {}

        SafeUniquePtr(SafeUniquePtr &&other) noexcept : ptr(std::move(other.ptr)) {}

        // Deleted copy constructor (unique_ptr is move-only)
        SafeUniquePtr(const SafeUniquePtr &) = delete;

        /**
         * @brief Assignment operators for array specialization
         */
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

        /**
         * @brief Array access operator with null check
         */
        T &operator[](size_t i) const
        {
            if (!ptr)
            {
                throw exceptions::NullPointerAccessException();
            }
            // Note: We can't check array bounds here since std::unique_ptr<T[]>
            // doesn't store the array size. In a real-world application, you might
            // want to store the size separately or use a different container.
            return ptr[i];
        }

        /**
         * @brief Boolean conversion operator
         *
         * @return true if the pointer is not null
         * @return false if the pointer is null
         */
        explicit operator bool() const noexcept
        {
            return static_cast<bool>(ptr);
        }

        /**
         * @brief Get the raw pointer
         *
         * @return T* The raw pointer (can be null)
         */
        T *get() const noexcept
        {
            return ptr.get();
        }

        /**
         * @brief Check if the pointer is null
         *
         * @return true if the pointer is null
         * @return false if the pointer is not null
         */
        bool isNull() const noexcept
        {
            return !ptr;
        }

        /**
         * @brief Methods for unique_ptr compatibility (array version)
         */
        /**
         * @brief Reset the pointer to null
         */
        void reset() noexcept
        {
            ptr.reset();
        }

        /**
         * @brief Reset the pointer to a new raw array pointer
         *
         * @param p Raw array pointer to manage
         */
        void reset(T *p) noexcept
        {
            ptr.reset(p);
        }

        /**
         * @brief Release ownership of the managed array
         *
         * @return T* Pointer to the managed array (now unmanaged)
         */
        T *release() noexcept
        {
            return ptr.release();
        }

        /**
         * @brief Swap the contents with another SafeUniquePtr
         *
         * @param other Another SafeUniquePtr to swap with
         */
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
