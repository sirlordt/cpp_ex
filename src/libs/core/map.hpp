/**
 * @file map.hpp
 * @brief Enhanced map implementation with additional utility methods
 * @author cpp_ex team
 * @date 2025-05-18
 */

#ifndef CPPEX_MAP_HPP
#define CPPEX_MAP_HPP

#include <map>
#include <functional>
#include <utility>
#include <initializer_list>
#include "vector.hpp" // Include Vector class

namespace cpp_ex
{

    /**
     * @brief Enhanced wrapper for std::map with additional utility methods
     *
     * This class provides a wrapper around std::map with additional utility methods
     * for common operations like getting keys, values, filtering, mapping values,
     * and set operations (union, intersection, difference).
     *
     * @tparam Key Type of the keys
     * @tparam Value Type of the mapped values
     * @tparam Compare Comparison function object type, defaults to std::less<Key>
     *
     * @example
     * ```cpp
     * // Create a map with some values
     * cpp_ex::Map<std::string, int> scores = {
     *     {"Alice", 95},
     *     {"Bob", 87},
     *     {"Charlie", 92}
     * };
     *
     * // Get all keys
     * auto names = scores.getKeys();
     *
     * // Filter entries
     * auto highScores = scores.filterEntries([](const std::string& name, int score) {
     *     return score >= 90;
     * });
     * ```
     */
    template <typename Key, typename Value, typename Compare = std::less<Key>>
    class Map
    {
    private:
        std::map<Key, Value, Compare> data;

        // Declare friendship with all other Map instantiations
        template <typename K, typename V, typename C>
        friend class Map;

    public:
        // Tipos (aliases)
        using key_type = typename std::map<Key, Value, Compare>::key_type;
        using mapped_type = typename std::map<Key, Value, Compare>::mapped_type;
        using value_type = typename std::map<Key, Value, Compare>::value_type;
        using size_type = typename std::map<Key, Value, Compare>::size_type;
        using difference_type = typename std::map<Key, Value, Compare>::difference_type;
        using key_compare = typename std::map<Key, Value, Compare>::key_compare;
        using allocator_type = typename std::map<Key, Value, Compare>::allocator_type;
        using reference = typename std::map<Key, Value, Compare>::reference;
        using const_reference = typename std::map<Key, Value, Compare>::const_reference;
        using pointer = typename std::map<Key, Value, Compare>::pointer;
        using const_pointer = typename std::map<Key, Value, Compare>::const_pointer;
        using iterator = typename std::map<Key, Value, Compare>::iterator;
        using const_iterator = typename std::map<Key, Value, Compare>::const_iterator;
        using reverse_iterator = typename std::map<Key, Value, Compare>::reverse_iterator;
        using const_reverse_iterator = typename std::map<Key, Value, Compare>::const_reverse_iterator;

        // Constructores
        Map() = default;

        explicit Map(const Compare &comp) : data(comp) {}

        template <typename InputIt>
        Map(InputIt first, InputIt last) : data(first, last) {}

        template <typename InputIt>
        Map(InputIt first, InputIt last, const Compare &comp) : data(first, last, comp) {}

        Map(std::initializer_list<value_type> init) : data(init) {}

        Map(std::initializer_list<value_type> init, const Compare &comp) : data(init, comp) {}

        Map(const Map &other) : data(other.data) {}

        Map(Map &&other) noexcept : data(std::move(other.data)) {}

        Map(const std::map<Key, Value, Compare> &stdMap) : data(stdMap) {}

        // Operadores de asignación
        Map &operator=(const Map &other)
        {
            if (this != &other)
            {
                data = other.data;
            }
            return *this;
        }

        Map &operator=(Map &&other) noexcept
        {
            data = std::move(other.data);
            return *this;
        }

        Map &operator=(std::initializer_list<value_type> ilist)
        {
            data = ilist;
            return *this;
        }

        // Conversión a std::map
        operator std::map<Key, Value, Compare>() const
        {
            return data;
        }

        std::map<Key, Value, Compare> &getStdMap()
        {
            return data;
        }

        const std::map<Key, Value, Compare> &getStdMap() const
        {
            return data;
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

        // Acceso a elementos
        mapped_type &at(const key_type &key)
        {
            return data.at(key);
        }

        const mapped_type &at(const key_type &key) const
        {
            return data.at(key);
        }

        mapped_type &operator[](const key_type &key)
        {
            return data[key];
        }

        mapped_type &operator[](key_type &&key)
        {
            return data[std::move(key)];
        }

        // Modificadores
        void clear() noexcept
        {
            data.clear();
        }

        std::pair<iterator, bool> insert(const value_type &value)
        {
            return data.insert(value);
        }

        std::pair<iterator, bool> insert(value_type &&value)
        {
            return data.insert(std::move(value));
        }

        template <typename P>
        std::pair<iterator, bool> insert(P &&value)
        {
            return data.insert(std::forward<P>(value));
        }

        iterator insert(const_iterator hint, const value_type &value)
        {
            return data.insert(hint, value);
        }

        iterator insert(const_iterator hint, value_type &&value)
        {
            return data.insert(hint, std::move(value));
        }

        template <typename P>
        iterator insert(const_iterator hint, P &&value)
        {
            return data.insert(hint, std::forward<P>(value));
        }

        template <typename InputIt>
        void insert(InputIt first, InputIt last)
        {
            data.insert(first, last);
        }

        void insert(std::initializer_list<value_type> ilist)
        {
            data.insert(ilist);
        }

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args &&...args)
        {
            return data.emplace(std::forward<Args>(args)...);
        }

        template <typename... Args>
        iterator emplaceHint(const_iterator hint, Args &&...args)
        {
            return data.emplace_hint(hint, std::forward<Args>(args)...);
        }

        iterator erase(const_iterator pos)
        {
            return data.erase(pos);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            return data.erase(first, last);
        }

        size_type erase(const key_type &key)
        {
            return data.erase(key);
        }

        void swap(Map &other)
        {
            data.swap(other.data);
        }

        // Lookup
        size_type count(const key_type &key) const
        {
            return data.count(key);
        }

        iterator find(const key_type &key)
        {
            return data.find(key);
        }

        const_iterator find(const key_type &key) const
        {
            return data.find(key);
        }

        bool contains(const key_type &key) const
        {
            return data.find(key) != data.end();
        }

        std::pair<iterator, iterator> equalRange(const key_type &key)
        {
            return data.equal_range(key);
        }

        std::pair<const_iterator, const_iterator> equalRange(const key_type &key) const
        {
            return data.equal_range(key);
        }

        iterator lowerBound(const key_type &key)
        {
            return data.lower_bound(key);
        }

        const_iterator lowerBound(const key_type &key) const
        {
            return data.lower_bound(key);
        }

        iterator upperBound(const key_type &key)
        {
            return data.upper_bound(key);
        }

        const_iterator upperBound(const key_type &key) const
        {
            return data.upper_bound(key);
        }

        // Observadores
        key_compare keyComp() const
        {
            return data.key_comp();
        }

        typename std::map<Key, Value, Compare>::value_compare valueComp() const
        {
            return data.value_comp();
        }

        // Operadores de comparación
        bool operator==(const Map &other) const
        {
            return data == other.data;
        }

        bool operator!=(const Map &other) const
        {
            return data != other.data;
        }

        bool operator<(const Map &other) const
        {
            return data < other.data;
        }

        bool operator<=(const Map &other) const
        {
            return data <= other.data;
        }

        bool operator>(const Map &other) const
        {
            return data > other.data;
        }

        bool operator>=(const Map &other) const
        {
            return data >= other.data;
        }

        // Métodos adicionales que usan cppex::Vector

        // Obtener todas las claves como un Vector
        Vector<Key> getKeys() const
        {
            Vector<Key> keys;
            keys.reserve(data.size());
            for (const auto &pair : data)
            {
                keys.pushBack(pair.first);
            }
            return keys;
        }

        // Obtener todos los valores como un Vector
        Vector<Value> getValues() const
        {
            Vector<Value> values;
            values.reserve(data.size());
            for (const auto &pair : data)
            {
                values.pushBack(pair.second);
            }
            return values;
        }

        // Obtener todos los pares como un Vector
        Vector<std::pair<Key, Value>> getEntries() const
        {
            Vector<std::pair<Key, Value>> entries;
            entries.reserve(data.size());
            for (const auto &pair : data)
            {
                entries.pushBack(pair);
            }
            return entries;
        }

        // Mapear valores a un nuevo tipo
        template <typename ResultType, typename UnaryFunc>
        Map<Key, ResultType, Compare> mapValues(UnaryFunc func) const
        {
            Map<Key, ResultType, Compare> result;
            for (const auto &pair : data)
            {
                result.insert({pair.first, func(pair.second)});
            }
            return result;
        }

        // Filtrar entradas según un predicado
        template <typename BinaryPredicate>
        Map filterEntries(BinaryPredicate pred) const
        {
            Map result;
            for (const auto &pair : data)
            {
                if (pred(pair.first, pair.second))
                {
                    result.insert(pair);
                }
            }
            return result;
        }

        // Ejecutar una función para cada par clave-valor
        template <typename BinaryFunc>
        void forEach(BinaryFunc func)
        {
            for (auto &pair : data)
            {
                func(pair.first, pair.second);
            }
        }

        template <typename BinaryFunc>
        void forEach(BinaryFunc func) const
        {
            for (const auto &pair : data)
            {
                func(pair.first, pair.second);
            }
        }

        // Unión de dos mapas (keys en ambos tomará los valores del mapa actual)
        Map merge(const Map &other) const
        {
            Map result = *this;
            for (const auto &pair : other.data)
            {
                if (!result.contains(pair.first))
                {
                    result.insert(pair);
                }
            }
            return result;
        }

        // Diferencia: entradas de este mapa que no están en el otro
        Map difference(const Map &other) const
        {
            Map result;
            for (const auto &pair : data)
            {
                if (!other.contains(pair.first))
                {
                    result.insert(pair);
                }
            }
            return result;
        }

        // Intersección: entradas con claves en ambos mapas (valores del mapa actual)
        Map intersection(const Map &other) const
        {
            Map result;
            for (const auto &pair : data)
            {
                if (other.contains(pair.first))
                {
                    result.insert(pair);
                }
            }
            return result;
        }
    };

    // Funciones de utilidad fuera de la clase
    template <typename Key, typename Value, typename Compare>
    void swap(Map<Key, Value, Compare> &lhs, Map<Key, Value, Compare> &rhs)
    {
        lhs.swap(rhs);
    }

} // namespace cppex

#endif // CPPEX_MAP_HPP
