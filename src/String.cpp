#include "../include/String.h"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace MySTL
{
    String::String() : data(new char[1]), cap(1)
    {
        data[0] = 0;
    }

    String::String(const char* str)
    {
        len = strlen(str);
        cap = len + 1;
        data = new char[cap];
        strcpy(data, str);
        data[len] = '\0';
    }

    String::String(const String& other) : data(new char[other.cap]), len(other.len), cap(other.cap)
    {
        memcpy(data, other.data, len + 1);
    }

    String::String(String&& other) noexcept : data(other.data), len(other.len), cap(other.cap)
    {
        other.data = nullptr;
        other.len = 0;
        other.cap = 0;
    }

    String& String::operator=(const String& other)
    {
        if (this == &other) return *this;
        char* new_data = new char[other.len + 1];
        memcpy(new_data, other.data, other.len + 1);

        delete[] data;
        data = new_data;
        len = other.len;
        cap = other.len + 1;
        return *this;
    }

    String& String::operator=(String&& other) noexcept
    {
        if (this == &other) return *this;
        delete[] data;

        data = other.data;
        len = other.len;
        cap = other.cap;
        other.data = nullptr;
        other.len = 0;
        other.cap = 0;
        return *this;
    }

    String::~String()
    {
        delete[] data;
    }

    String String::operator+(const String& other) const
    {
        String result = *this;
        result.append(other);
        return result;
    }


    String String::operator+(const char* str) const
    {
        String result = *this;
        result.append(str);
        return result;
    }

    String& String::operator+=(const String& other)
    {
        return append(other);
    }


    String& String::operator+=(const char* str)
    {
        return append(str);
    }

    bool String::operator==(const String& other) const
    {
        return this->compare(other) == 0;
    }

    bool String::operator!=(const String& other) const
    {
        return !(*this == other);
    }

    bool String::operator<(const String& other) const
    {
        return this->compare(other) < 0;
    }

    bool String::operator<=(const String& other) const
    {
        return *this < other || *this == other;
    }

    bool String::operator>(const String& other) const
    {
        return this->compare(other) > 0;
    }

    bool String::operator>=(const String& other) const
    {
        return *this > other || *this == other;
    }

    const char* String::c_str() const
    {
        return data;
    }

    void String::reserve(size_t sz)
    {
        auto new_data = new char[sz];
        if (data)
            memcpy(new_data, data, len + 1);
        delete[] data;
        data = new_data;
        cap = sz;
    }


    bool String::empty() const
    {
        if (data == nullptr || data[0] == '\0') return true;
        return len == 0;
    }

    size_t String::size() const
    {
        return length();
    }

    size_t String::capacity() const
    {
        return cap;
    }

    size_t String::length() const
    {
        size_t count = 0;
        size_t i = 0;
        while (i < len)
        {
            if ((data[i] & 0xC0) != 0x80) ++count;
            ++i;
        }
        return count;
    }

    void String::clear()
    {
        delete[] data;
        data = new char[1];
        data[0] = 0;
        len = 0;
        cap = 1;
    }

    String& String::append(const String& other)
    {
        size_t new_len = len + other.len;
        if (new_len >= cap)
            reserve(new_len * 2);
        memcpy(data + len, other.data, other.len);
        len = new_len;
        data[len] = '\0';
        return *this;
    }


    String& String::append(const char* str)
    {
        if (str == nullptr) return *this;
        size_t new_len = len + strlen(str);
        if (new_len >= cap)
            reserve(new_len * 2);
        memcpy(data + len, str, strlen(str));
        len = new_len;
        data[len] = '\0';
        return *this;
    }

    String String::substr(const size_t start, const size_t count) const
    {
        if (start >= length()) return {};

        size_t real_start = 0, char_count = 0;
        while (real_start < len && char_count < start)
        {
            if ((data[real_start] & 0xC0) != 0x80) ++char_count;
            ++real_start;
        }

        size_t ed = real_start;
        while (ed < len && char_count < start + count)
        {
            if ((data[ed] & 0xC0) != 0x80) ++char_count;
            ++ed;
        }

        const size_t real_count = ed - real_start;
        String result;
        result.reserve(real_count + 1);
        memcpy(result.data, data + real_start, real_count);
        result.data[real_count] = '\0';
        result.len = real_count;
        return result;
    }

    size_t String::find(const String& substr, size_t pos) const
    {
        if (substr.empty()) return pos > length() ? npos : pos;
        if (pos >= length()) return npos;

        size_t byte_pos = 0;
        size_t chars_count = 0;
        while (byte_pos < len && chars_count < pos)
        {
            if ((data[byte_pos] & 0xC0) != 0x80) ++chars_count;
            if (chars_count <= pos)
                ++byte_pos;
        }

        for (size_t i = byte_pos; i < len; ++i)
        {
            if ((data[i] & 0xC0) == 0x80) continue;

            size_t j = 0, substr_byte = 0;
            bool match = true;
            while (i + j < len && substr_byte < substr.len)
            {
                // check current byte is the start of a character with same index
                if ((data[i+j] & 0xC0) == 0x80 || (substr.data[substr_byte] & 0xC0) == 0x80)
                {
                    if (data[i+j] != substr.data[substr_byte])
                    {
                        match = false;
                        break;
                    }
                    ++j;
                    ++substr_byte;
                } else
                {
                    if (substr_byte >= substr.len || data[i+j] != substr.data[substr_byte])
                    {
                        match = false;
                        break;
                    }
                    ++j;
                    ++substr_byte;
                }
            }
            if (match && substr_byte == substr.len) return chars_count; // return the index of character instead of byte
            if ((data[i] & 0xC0) != 0x80)
                ++chars_count;
        }
        return npos;
    }

    int String::compare(const String& str) const
    {
        const size_t min_len = std::min(len, str.len);
        const int result = memcmp(data, str.data, min_len);
        if (result == 0)
        {
            if (len < str.len) return -1;
            if (len > str.len) return 1;
        }
        return result;
    }

    char32_t String::decodeUtf8Char(const char* bytes) {
        char32_t codepoint = 0;
        int numBytes = 0;

        // 检测首字节并确定 UTF-8 字符的字节数
        if (const auto firstByte = static_cast<unsigned char>(bytes[0]); (firstByte & 0x80) == 0x00) {  // 0xxxxxxx
            codepoint = firstByte;
            numBytes = 1;
        } else if ((firstByte & 0xE0) == 0xC0) {  // 110xxxxx
            codepoint = firstByte & 0x1F;
            numBytes = 2;
        } else if ((firstByte & 0xF0) == 0xE0) {  // 1110xxxx
            codepoint = firstByte & 0x0F;
            numBytes = 3;
        } else if ((firstByte & 0xF8) == 0xF0) {  // 11110xxx
            codepoint = firstByte & 0x07;
            numBytes = 4;
        } else {
            throw std::runtime_error("Invalid UTF-8 encoding");
        }

        // 解析剩余字节
        for (int i = 1; i < numBytes; ++i) {
            const auto byte = static_cast<unsigned char>(bytes[i]);
            if ((byte & 0xC0) != 0x80) {  // 必须是 10xxxxxx
                throw std::runtime_error("Invalid UTF-8 continuation byte");
            }
            codepoint = codepoint << 6 | byte & 0x3F;  // 将 6 位附加到码点
        }

        return codepoint;
    }

    size_t String::getUtf8CharLength(const char first_byte) {
        // 根据 UTF-8 编码的首字节确定整个字符的字节数
        if ((first_byte & 0x80) == 0x00) return 1;
        if ((first_byte & 0xE0) == 0xC0) return 2;
        if ((first_byte & 0xF0) == 0xE0) return 3;
        if ((first_byte & 0xF8) == 0xF0) return 4;
        return 0; // 非法的 UTF-8 编码
    }

    void String::encodeUtf8Char(char* dest, const char32_t codepoint) {
        if (codepoint <= 0x7F) {
            *dest = static_cast<char>(codepoint);
        } else if (codepoint <= 0x7FF) {
            dest[0] = 0xC0 | (codepoint >> 6);
            dest[1] = 0x80 | (codepoint & 0x3F);
        } else if (codepoint <= 0xFFFF) {
            dest[0] = 0xE0 | (codepoint >> 12);
            dest[1] = 0x80 | ((codepoint >> 6) & 0x3F);
            dest[2] = 0x80 | (codepoint & 0x3F);
        } else if (codepoint <= 0x10FFFF) {
            dest[0] = 0xF0 | (codepoint >> 18);
            dest[1] = 0x80 | ((codepoint >> 12) & 0x3F);
            dest[2] = 0x80 | ((codepoint >> 6) & 0x3F);
            dest[3] = 0x80 | (codepoint & 0x3F);
        }
    }

    char32_t String::operator[](const size_t index) const
    {
        return at(index);
    }


    char32_t String::at(const size_t pos) const
    {
        size_t byte_index = 0, char_index = 0;

        while (byte_index < len && char_index < pos)
        {
            byte_index += getUtf8CharLength(data[byte_index]);
            ++char_index;
        }

        if (byte_index < len)
            return decodeUtf8Char(&data[byte_index]);
        throw std::out_of_range("Index out of range");
    }

    String& String::insert(const size_t pos, const String& str)
    {
        if (pos > len) throw std::out_of_range("Position out of range");
        if (len + str.len >= cap) reserve(len + str.len + 1);
        memmove(data + pos + str.len, data + pos, len - pos);
        memcpy(data + pos, str.data, str.len);
        len += str.len;
        data[len] = '\0';
        return *this;
    }

    String& String::erase(const size_t pos, size_t count)
    {
        if (pos > len) throw std::out_of_range("Position out of range");
        if (pos + count > len) count = len - pos;
        memmove(data + pos, data + pos + count, len - pos - count);
        len -= count;
        data[len] = '\0';
        return *this;
    }

    void String::push_back(const char32_t codepoint)
    {
        size_t additional_bytes = getUtf8CharLength(codepoint);
        if (len + additional_bytes >= cap) reserve(len + additional_bytes + 1);

        encodeUtf8Char(data + len, codepoint);
        len += additional_bytes;
        data[len] = '\0';
    }

    void String::pop_back()
    {
            if (len == 0) return;
        size_t pos = len-1;
        while (pos > 0 && (data[pos] & 0xC0) == 0x80)
            --pos;
        data[pos] = '\0';
        len = pos;
    }

    char32_t String::front() const
    {
        if (len == 0) throw std::out_of_range("Empty string");
        return decodeUtf8Char(data);
    }

    char32_t String::back() const
    {
        if (len == 0) throw std::out_of_range("Empty string");
        size_t pos = len - 1;
        while (pos > 0 && (data[pos] & 0xC0) == 0x80)
            --pos;

        return decodeUtf8Char(data + pos);
    }

    void String::formatHelper(std::ostringstream& stream, const char* format)
    {
        while (*format)
        {
            if (*format == '%')
            {
                if (*(format + 1) == '%')
                    ++format;
                else
                    throw std::runtime_error("Invalid format string: missing arguments");
            }
            stream << *format++;
        }
    }

    template <typename T, typename... Args>
    void String::formatHelper(std::ostringstream& stream, const char* format, T value, Args... args)
    {
        while (*format)
        {
            if (*format == '%')
            {
                if (*(format + 1) == '%')
                {
                    ++format;
                    stream << '%';
                } else
                {
                    stream << value;
                    formatHelper(stream, format + 1, args...);
                    return;
                }
            } else
                stream << *format;
            ++format;
        }
        if (sizeof...(args) > 0)
            throw std::runtime_error("Extra arguments provided to format");
    }

    template <typename... Args>
    String String::format(const char* format, Args... args)
    {
        std::ostringstream stream;
        formatHelper(stream, format, args...);
        return String(stream.str().c_str());
    }


}
