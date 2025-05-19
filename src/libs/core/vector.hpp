/**
 * @file vector.hpp
 * @brief Enhanced vector implementation with additional utility methods
 * @author cpp_ex team
 * @date 2025-05-18
 */

#ifndef CPPEX_VECTOR_HPP
#define CPPEX_VECTOR_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <initializer_list>
#include <numeric> // Para std::accumulate

namespace cpp_ex
{

    /**
     * @brief Enhanced wrapper for std::vector with additional utility methods
     *
     * This class provides a wrapper around std::vector with additional utility methods
     * for common operations like filtering, mapping, reducing, and other functional
     * programming patterns.
     *
     * @tparam T Type of the elements
     *
     * @example
     * ```cpp
     * // Create a vector with some values
     * cpp_ex::Vector<int> numbers = {1, 2, 3, 4, 5};
     *
     * // Filter even numbers
     * auto evenNumbers = numbers.filter([](int n) { return n % 2 == 0; });
     *
     * // Map to squares
     * auto squares = numbers.map([](int n) { return n * n; });
     *
     * // Reduce to sum
     * int sum = numbers.reduce(0, [](int acc, int n) { return acc + n; });
     * ```
     */
    template <typename T>
    class Vector
    {
    private:
        std::vector<T> data;

        // Declare friendship with all other Vector instantiations
        template <typename U>
        friend class Vector;

    public:
        // Tipos (aliases)
        using value_type = typename std::vector<T>::value_type;
        using size_type = typename std::vector<T>::size_type;
        using difference_type = typename std::vector<T>::difference_type;
        using reference = typename std::vector<T>::reference;
        using const_reference = typename std::vector<T>::const_reference;
        using pointer = typename std::vector<T>::pointer;
        using const_pointer = typename std::vector<T>::const_pointer;
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        using reverse_iterator = typename std::vector<T>::reverse_iterator;
        using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

        // Constructores
        Vector() = default;

        explicit Vector(size_type count) : data(count) {}

        Vector(size_type count, const T &value) : data(count, value) {}

        Vector(std::initializer_list<T> init) : data(init) {}

        template <typename InputIt>
        Vector(InputIt first, InputIt last) : data(first, last) {}

        Vector(const Vector &other) : data(other.data) {}

        Vector(Vector &&other) noexcept : data(std::move(other.data)) {}

        Vector(const std::vector<T> &stdVector) : data(stdVector) {}

        // Operadores de asignación
        Vector &operator=(const Vector &other)
        {
            if (this != &other)
            {
                data = other.data;
            }
            return *this;
        }

        Vector &operator=(Vector &&other) noexcept
        {
            data = std::move(other.data);
            return *this;
        }

        Vector &operator=(std::initializer_list<T> ilist)
        {
            data = ilist;
            return *this;
        }

        // Conversión a std::vector
        operator std::vector<T>() const
        {
            return data;
        }

        std::vector<T> &getStdVector()
        {
            return data;
        }

        const std::vector<T> &getStdVector() const
        {
            return data;
        }

        // Métodos de acceso a elementos
        reference at(size_type pos)
        {
            return data.at(pos);
        }

        const_reference at(size_type pos) const
        {
            return data.at(pos);
        }

        reference operator[](size_type pos)
        {
            return data[pos];
        }

        const_reference operator[](size_type pos) const
        {
            return data[pos];
        }

        reference getFront()
        {
            return data.front();
        }

        const_reference getFront() const
        {
            return data.front();
        }

        reference getBack()
        {
            return data.back();
        }

        const_reference getBack() const
        {
            return data.back();
        }

        pointer getData()
        {
            return data.data();
        }

        const_pointer getData() const
        {
            return data.data();
        }

        // Iteradores
        iterator begin() noexcept
        {
            return data.begin();
        }

        const_iterator begin() const noexcept
        {
            return data.begin();
        }

        const_iterator cbegin() const noexcept
        {
            return data.cbegin();
        }

        iterator end() noexcept
        {
            return data.end();
        }

        const_iterator end() const noexcept
        {
            return data.end();
        }

        const_iterator cend() const noexcept
        {
            return data.cend();
        }

        reverse_iterator rbegin() noexcept
        {
            return data.rbegin();
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return data.rbegin();
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return data.crbegin();
        }

        reverse_iterator rend() noexcept
        {
            return data.rend();
        }

        const_reverse_iterator rend() const noexcept
        {
            return data.rend();
        }

        const_reverse_iterator crend() const noexcept
        {
            return data.crend();
        }

        // Capacidad
        bool isEmpty() const noexcept
        {
            return data.empty();
        }

        size_type getSize() const noexcept
        {
            return data.size();
        }

        size_type getMaxSize() const noexcept
        {
            return data.max_size();
        }

        void reserve(size_type newCap)
        {
            data.reserve(newCap);
        }

        size_type getCapacity() const noexcept
        {
            return data.capacity();
        }

        void shrinkToFit()
        {
            data.shrink_to_fit();
        }

        // Modificadores
        void clear() noexcept
        {
            data.clear();
        }

        iterator insert(const_iterator pos, const T &value)
        {
            return data.insert(pos, value);
        }

        iterator insert(const_iterator pos, T &&value)
        {
            return data.insert(pos, std::move(value));
        }

        iterator insert(const_iterator pos, size_type count, const T &value)
        {
            return data.insert(pos, count, value);
        }

        template <typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            return data.insert(pos, first, last);
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist)
        {
            return data.insert(pos, ilist);
        }

        template <typename... Args>
        iterator emplace(const_iterator pos, Args &&...args)
        {
            return data.emplace(pos, std::forward<Args>(args)...);
        }

        iterator erase(const_iterator pos)
        {
            return data.erase(pos);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            return data.erase(first, last);
        }

        void pushBack(const T &value)
        {
            data.push_back(value);
        }

        void pushBack(T &&value)
        {
            data.push_back(std::move(value));
        }

        template <typename... Args>
        reference emplaceBack(Args &&...args)
        {
            return data.emplace_back(std::forward<Args>(args)...);
        }

        void popBack()
        {
            data.pop_back();
        }

        void resize(size_type count)
        {
            data.resize(count);
        }

        void resize(size_type count, const value_type &value)
        {
            data.resize(count, value);
        }

        void swap(Vector &other)
        {
            data.swap(other.data);
        }

        // Operaciones adicionales
        bool contains(const T &value) const
        {
            return std::find(data.begin(), data.end(), value) != data.end();
        }

        size_type countValue(const T &value) const
        {
            return std::count(data.begin(), data.end(), value);
        }

        template <typename Predicate>
        size_type countIf(Predicate pred) const
        {
            return std::count_if(data.begin(), data.end(), pred);
        }

        template <typename UnaryFunc>
        Vector<typename std::result_of<UnaryFunc(T)>::type> map(UnaryFunc func) const
        {
            Vector<typename std::result_of<UnaryFunc(T)>::type> result;
            result.reserve(data.size());
            std::transform(data.begin(), data.end(), std::back_inserter(result.data), func);
            return result;
        }

        template <typename Predicate>
        Vector<T> filter(Predicate pred) const
        {
            Vector<T> result;
            std::copy_if(data.begin(), data.end(), std::back_inserter(result.data), pred);
            return result;
        }

        void forEach(const std::function<void(T &)> &func)
        {
            std::for_each(data.begin(), data.end(), func);
        }

        void forEach(const std::function<void(const T &)> &func) const
        {
            std::for_each(data.begin(), data.end(), func);
        }

        template <typename BinaryOp>
        T reduce(const T &init, BinaryOp op) const
        {
            return std::accumulate(data.begin(), data.end(), init, op);
        }

        void sort()
        {
            std::sort(data.begin(), data.end());
        }

        template <typename Compare>
        void sort(Compare comp)
        {
            std::sort(data.begin(), data.end(), comp);
        }

        void reverse()
        {
            std::reverse(data.begin(), data.end());
        }

        size_type findFirstIndex(const T &value) const
        {
            auto it = std::find(data.begin(), data.end(), value);
            return it != data.end() ? std::distance(data.begin(), it) : static_cast<size_type>(-1);
        }

        template <typename Predicate>
        size_type findFirstIndexIf(Predicate pred) const
        {
            auto it = std::find_if(data.begin(), data.end(), pred);
            return it != data.end() ? std::distance(data.begin(), it) : static_cast<size_type>(-1);
        }

        bool equals(const Vector<T> &other) const
        {
            return data == other.data;
        }

        // Operadores de comparación
        bool operator==(const Vector &other) const
        {
            return data == other.data;
        }

        bool operator!=(const Vector &other) const
        {
            return data != other.data;
        }

        bool operator<(const Vector &other) const
        {
            return data < other.data;
        }

        bool operator<=(const Vector &other) const
        {
            return data <= other.data;
        }

        bool operator>(const Vector &other) const
        {
            return data > other.data;
        }

        bool operator>=(const Vector &other) const
        {
            return data >= other.data;
        }
    };

    // Funciones de utilidad fuera de la clase
    template <typename T>
    void swap(Vector<T> &lhs, Vector<T> &rhs)
    {
        lhs.swap(rhs);
    }

} // namespace cppex

#endif // CPPEX_VECTOR_HPP
