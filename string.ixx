export module MyModule.string;


namespace MySTL
{
    export class String final
    {
    public:
        String();
        explicit String(const char* str);
        String(const String& other);
        String(String &&other) noexcept;
        ~String();

        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;

        [[nodiscard]] bool empty() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] size_t capacity() const;
        [[nodiscard]] size_t length() const;

    private:
        char* data;
        size_t len{};
        size_t cap{};
    };
}