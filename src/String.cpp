import MyModule.string;
#include <cstring>

namespace MySTL
{
    String::String() : data(new char[1]), cap(1)
    {
        data[0] = '\0';
    }

    String::String(const char* str)
    {
        len = strlen(str);
        cap = len + 1;
        data = new char[cap];
        strcpy(data, str);
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

    String::~String()
    {
        delete[] data;
    }

}
