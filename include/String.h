#ifndef STRING_H_
#define STRING_H_

#include <sstream>

#include "GenericIterator.h"

namespace MySTL {
    class String final {
    public:
        static constexpr size_t npos = -1;

        String();

        explicit String(const char *str);

        String(const String &other);

        String(String &&other) noexcept;

        ~String();

        String &operator=(const String &other);

        String &operator=(String &&other) noexcept;

        String operator+(const String &other) const;

        String operator+(const char *str) const;

        String &operator+=(const String &other);

        String &operator+=(const char *str);

        bool operator==(const String &) const;

        bool operator!=(const String &) const;

        bool operator<(const String &) const;

        bool operator<=(const String &) const;

        bool operator>(const String &) const;

        bool operator>=(const String &) const;

        [[nodiscard]] const char *c_str() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] size_t capacity() const;

        [[nodiscard]] size_t length() const;

        void clear();

        String &append(const String &other);

        String &append(const char *str);

        [[nodiscard]] String substr(size_t start, size_t count) const;

        [[nodiscard]] size_t find(const String &substr, size_t pos = 0) const;

        [[nodiscard]] int compare(const String &str) const;

        void push_back(char32_t codepoint);

        void pop_back();

        [[nodiscard]] char32_t front() const;

        [[nodiscard]] char32_t back() const;

        char32_t operator[](size_t index) const;

        [[nodiscard]] char32_t at(size_t pos) const;

        String &insert(size_t pos, const String &str);

        String &erase(size_t pos, size_t count = npos);

        template<typename... Args>
        static String format(const char *format, Args... args);


        using iterator = GenericIterator<char>;
        using const_iterator = GenericIterator<const char>;
        // todo: implement iterator for char with UTF-8

    private:
        char *data;
        size_t len{};
        size_t cap{};

        void reserve(size_t sz);

        [[nodiscard]] static size_t getUtf8CharLength(char first_byte);

        static void encodeUtf8Char(char *dest, char32_t codepoint);

        static char32_t decodeUtf8Char(const char *bytes);

        static void formatHelper(std::ostringstream &stream, const char *format);

        template<typename T, typename... Args>
        static void formatHelper(std::ostringstream &stream, const char *format, T value, Args... args);
    };
}


#endif // STRING_H_