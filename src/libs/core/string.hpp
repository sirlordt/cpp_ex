#ifndef CPPEX_STRING_H
#define CPPEX_STRING_H

#include <string>
#include <algorithm>
#include <cctype>
#include "vector.hpp" // Include cpp_ex::Vector
#include "map.hpp"    // Include cpp_ex::Map

namespace cpp_ex
{

    class String
    {
    private:
        std::string data;

    public:
        // Constructors
        String() : data("") {}
        String(const std::string &str) : data(str) {}
        String(const char *str) : data(str) {}
        String(const String &other) : data(other.data) {}
        String(char c, size_t count = 1) : data(count, c) {}

        // Assignment operators
        String &operator=(const std::string &str)
        {
            data = str;
            return *this;
        }

        String &operator=(const char *str)
        {
            data = str;
            return *this;
        }

        String &operator=(const String &other)
        {
            if (this != &other)
            {
                data = other.data;
            }
            return *this;
        }

        // Implicit conversion to std::string
        operator std::string() const
        {
            return data;
        }

        // Basic methods
        const char *getCString() const
        {
            return data.c_str();
        }

        std::string getString() const
        {
            return data;
        }

        size_t getLength() const
        {
            return data.length();
        }

        bool isEmpty() const
        {
            return data.empty();
        }

        void clear()
        {
            data.clear();
        }

        // Access methods
        char charAt(size_t index) const
        {
            return index < data.length() ? data[index] : '\0';
        }

        void setCharAt(size_t index, char c)
        {
            if (index < data.length())
            {
                data[index] = c;
            }
        }

        // Modification methods
        void append(const String &str)
        {
            data.append(str.data);
        }

        void append(const std::string &str)
        {
            data.append(str);
        }

        void append(const char *str)
        {
            data.append(str);
        }

        void append(char c)
        {
            data.push_back(c);
        }

        String &appendAndReturn(const std::string &str)
        {
            data.append(str);
            return *this;
        }

        void insert(size_t pos, const std::string &str)
        {
            data.insert(pos, str);
        }

        void remove(size_t pos, size_t len)
        {
            data.erase(pos, len);
        }

        // Method to erase characters from the string (similar to std::string::erase)
        String &erase(size_t pos = 0, size_t len = std::string::npos)
        {
            data.erase(pos, len);
            return *this;
        }

        // Overload to erase a single character at the given position
        String &erase(size_t pos)
        {
            data.erase(pos, 1);
            return *this;
        }

        // Overload to erase a range using iterators
        String &erase(std::string::iterator first, std::string::iterator last)
        {
            data.erase(first, last);
            return *this;
        }

        // Overload to erase a single character at the given iterator position
        String &erase(std::string::iterator position)
        {
            data.erase(position);
            return *this;
        }

        void replace(size_t pos, size_t len, const std::string &str)
        {
            data.replace(pos, len, str);
        }

        String substring(size_t pos, size_t len = std::string::npos) const
        {
            return String(data.substr(pos, len));
        }

        // Search methods
        size_t find(const std::string &str, size_t pos = 0) const
        {
            return data.find(str, pos);
        }

        size_t find(char c, size_t pos = 0) const
        {
            return data.find(c, pos);
        }

        size_t findLast(const std::string &str, size_t pos = std::string::npos) const
        {
            return data.rfind(str, pos);
        }

        size_t findLast(char c, size_t pos = std::string::npos) const
        {
            return data.rfind(c, pos);
        }

        bool contains(const std::string &str) const
        {
            return data.find(str) != std::string::npos;
        }

        bool contains(char c) const
        {
            return data.find(c) != std::string::npos;
        }

        bool startsWith(const std::string &prefix) const
        {
            return data.find(prefix) == 0;
        }

        bool endsWith(const std::string &suffix) const
        {
            if (suffix.length() > data.length())
                return false;
            return data.compare(data.length() - suffix.length(), suffix.length(), suffix) == 0;
        }

        // Transformation methods
        String toUpperCase() const
        {
            std::string result = data;
            std::transform(result.begin(), result.end(), result.begin(),
                           [](unsigned char c)
                           { return std::toupper(c); });
            return String(result);
        }

        String toLowerCase() const
        {
            std::string result = data;
            std::transform(result.begin(), result.end(), result.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });
            return String(result);
        }

        String trim() const
        {
            std::string result = data;
            // Remove whitespace from the beginning
            result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char c)
                                                      { return !std::isspace(c); }));
            // Remove whitespace from the end
            result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char c)
                                      { return !std::isspace(c); })
                             .base(),
                         result.end());
            return String(result);
        }

        // Left trim - removes whitespace from the beginning of the string
        String ltrim() const
        {
            std::string result = data;
            result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char c)
                                                      { return !std::isspace(c); }));
            return String(result);
        }

        // Right trim - removes whitespace from the end of the string
        String rtrim() const
        {
            std::string result = data;
            result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char c)
                                      { return !std::isspace(c); })
                             .base(),
                         result.end());
            return String(result);
        }

        // Using cppex::Vector instead of std::vector
        Vector<String> split(const std::string &delimiter) const
        {
            Vector<String> result;
            size_t start = 0;
            size_t end;

            while ((end = data.find(delimiter, start)) != std::string::npos)
            {
                result.pushBack(String(data.substr(start, end - start)));
                start = end + delimiter.length();
            }

            result.pushBack(String(data.substr(start)));
            return result;
        }

        void replace(const std::string &oldStr, const std::string &newStr)
        {
            size_t pos = 0;
            while ((pos = data.find(oldStr, pos)) != std::string::npos)
            {
                data.replace(pos, oldStr.length(), newStr);
                pos += newStr.length();
            }
        }

        // Method to count character occurrences using a Map
        Map<char, size_t> countCharacters() const
        {
            Map<char, size_t> charCount;
            for (char c : data)
            {
                if (charCount.contains(c))
                {
                    charCount[c] += 1;
                }
                else
                {
                    charCount[c] = 1;
                }
            }
            return charCount;
        }

        // Method to get word frequencies using a Map
        Map<String, size_t> getWordFrequencies() const
        {
            Map<String, size_t> wordFreq;

            // Simple word splitting by spaces (can be improved)
            Vector<String> words = split(" ");

            for (const String &word : words)
            {
                String cleanWord = word.trim();
                if (!cleanWord.isEmpty())
                {
                    if (wordFreq.contains(cleanWord))
                    {
                        wordFreq[cleanWord] += 1;
                    }
                    else
                    {
                        wordFreq[cleanWord] = 1;
                    }
                }
            }

            return wordFreq;
        }

        // Method to convert a string to a Map using separator and key-value split tokens
        // Examples:
        // 1. With splitToken="=" and separatorToken=";"
        //    "Name1=Value1;Name2=Value2;Name3=Value3" => {"Name1": "Value1", "Name2": "Value2", "Name3": "Value3"}
        //
        // 2. With splitToken="" and separatorToken=";"
        //    "Name1=Value1;Name2=Value2;Name3=Value3" => {"Name1=Value1": "Name1=Value1", "Name2=Value2": "Name2=Value2", "Name3=Value3": "Name3=Value3"}
        //
        // 3. With splitToken="*" and separatorToken=";"
        //    "Name1=Value1;Name2=Value2;Name3=Value3" => {"Name1=Value1": "Name1=Value1", "Name2=Value2": "Name2=Value2", "Name3=Value3": "Name3=Value3"}
        //
        // 4. With splitToken="" and separatorToken=""
        //    "Name1=Value1;Name2=Value2;Name3=Value3" => {} (empty map)
        //
        // 5. With splitToken="*" and separatorToken="?"
        //    "Name1=Value1;Name2=Value2;Name3=Value3" => {} (empty map)
        //
        // 6. With splitToken="=" and separatorToken=""
        //    "Name1=Value1;Name2=Value2;Name3=Value3" => {} (empty map)
        Map<String, String> toMap(const std::string &splitToken, const std::string &separatorToken) const
        {
            Map<String, String> result;

            // If either token is empty, return an empty map (examples 4 and 6)
            if (separatorToken.empty())
            {
                return result;
            }

            // Split the string by the separator token
            Vector<String> pairs = splitByToken(separatorToken, true);

            // Process each pair
            for (const String &pair : pairs)
            {
                // If splitToken is empty, use the entire segment as both key and value (example 2)
                if (splitToken.empty())
                {
                    String segment = pair.trim();
                    if (!segment.isEmpty())
                    {
                        result[segment] = segment;
                    }
                    continue;
                }

                // Find the position of the split token
                size_t splitPos = pair.find(splitToken);

                // If split token is found, add the key-value pair to the map (example 1)
                if (splitPos != std::string::npos)
                {
                    String key = pair.substring(0, splitPos).trim();
                    String value = pair.substring(splitPos + splitToken.length()).trim();
                    result[key] = value;
                }
                else
                {
                    // If split token not found, use the entire segment as both key and value (example 3 and 5)
                    String segment = pair.trim();
                    if (!segment.isEmpty())
                    {
                        result[segment] = segment;
                    }
                }
            }

            return result;
        }

        // Method to convert a string to a Vector of individual String characters
        Vector<String> toStringVector() const
        {
            Vector<String> result;
            result.reserve(data.length());

            for (char c : data)
            {
                result.pushBack(String(1, c));
            }

            return result;
        }

        // Method to convert a string to a Vector of chars
        Vector<char> toCharVector() const
        {
            Vector<char> result;
            result.reserve(data.length());

            for (char c : data)
            {
                result.pushBack(c);
            }

            return result;
        }

        // Method to split string by a token with optional trimming
        Vector<String> splitByToken(const std::string &token, bool applyTrim = false) const
        {
            Vector<String> result;
            size_t start = 0;
            size_t end;

            while ((end = data.find(token, start)) != std::string::npos)
            {
                String segment(data.substr(start, end - start));
                result.pushBack(applyTrim ? segment.trim() : segment);
                start = end + token.length();
            }

            // Add the last segment
            String lastSegment(data.substr(start));
            result.pushBack(applyTrim ? lastSegment.trim() : lastSegment);

            return result;
        }

        // Additional operators
        bool operator==(const String &other) const
        {
            return data == other.data;
        }

        bool operator!=(const String &other) const
        {
            return data != other.data;
        }

        bool operator<(const String &other) const
        {
            return data < other.data;
        }

        bool operator>(const String &other) const
        {
            return data > other.data;
        }

        bool operator<=(const String &other) const
        {
            return data <= other.data;
        }

        bool operator>=(const String &other) const
        {
            return data >= other.data;
        }

        String operator+(const String &other) const
        {
            return String(data + other.data);
        }

        String &operator+=(const String &other)
        {
            data += other.data;
            return *this;
        }

        char operator[](size_t index) const
        {
            return data[index];
        }

        char &operator[](size_t index)
        {
            return data[index];
        }
    };

} // namespace cppex

#endif // CPPEX_STRING_H
