#pragma once

#include<string>
#include<string_view>
#include<iterator>

class CowString {
private:
    template<typename CowStringPointer, typename CharReference>
    class IteratorTemplate : public std::iterator<std::forward_iterator_tag, char, size_t, std::pair<CowStringPointer, size_t>, CharReference>
    {
    public:
        using Category = std::forward_iterator_tag;
        using Type = char;
        using Distance = size_t;
        using Pointer = std::pair<CowStringPointer, size_t>;
        using Reference = CharReference;

        IteratorTemplate(): ptr_({nullptr, 0}) {}
        IteratorTemplate(Pointer ptr) : ptr_(ptr) {}

        Reference operator*() const { return CharReference(ptr_.first, ptr_.second); }

        IteratorTemplate& operator++() { ptr_.second++; return *this; }
        IteratorTemplate operator++(int) { IteratorTemplate tmp = *this; ++(*this); return tmp; }
        IteratorTemplate& operator--() { ptr_.second--; return *this; }
        IteratorTemplate operator--(int) { IteratorTemplate tmp = *this; --(*this); return tmp; }

        friend IteratorTemplate operator+(const IteratorTemplate& first, Distance second) { return IteratorTemplate({first.ptr_.first, first.ptr_.second + second}); }
        IteratorTemplate& operator+=(Distance second) { ptr_.second += second; return *this; }
        friend IteratorTemplate operator-(const IteratorTemplate& first, Distance second) { return IteratorTemplate({first.ptr_.first, first.ptr_.second - second}); }
        IteratorTemplate& operator-=(Distance second) { ptr_.second -= second; return *this; }

        friend bool operator== (const IteratorTemplate& a, const IteratorTemplate& b) { return a.ptr_ == b.ptr_; }
        friend bool operator!= (const IteratorTemplate& a, const IteratorTemplate& b) { return a.ptr_ != b.ptr_; }
        friend bool operator< (const IteratorTemplate& a, const IteratorTemplate& b) { return a.ptr_ < b.ptr_; }
        friend bool operator> (const IteratorTemplate& a, const IteratorTemplate& b) { return a.ptr_ > b.ptr_; }
        friend bool operator<= (const IteratorTemplate& a, const IteratorTemplate& b) { return a.ptr_ <= b.ptr_; }
        friend bool operator>= (const IteratorTemplate& a, const IteratorTemplate& b) { return a.ptr_ >= b.ptr_; }

    private:
        Pointer ptr_;
    };

public:
    class CharProxy;
    class ConstCharProxy;
    using Iterator = IteratorTemplate<CowString*, CharProxy>;
    using ConstIterator = IteratorTemplate<const CowString*, ConstCharProxy>;

    class CharProxy {
    public:
        char operator=(char second);
        operator char() const;
    private:
        CharProxy(CowString* ptr, size_t i);
        CowString *ptr_;
        size_t i_;

        friend Iterator;
        friend CowString;
    };

    class ConstCharProxy {
    public:
        operator char() const;

    private:
        ConstCharProxy(const CowString* ptr, size_t i);
        const CowString* ptr_;
        size_t i_;

        friend ConstIterator;
        friend CowString;
    };

public:
    CowString(std::string_view string);
    CowString(const CowString& from);
    CowString(CowString&& from);

    ~CowString();

    CowString& operator=(const CowString& from);
    CowString& operator=(CowString&& from);

    char* GetData() const;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

    CharProxy operator[](size_t i);

    ConstCharProxy At(size_t i) const;

    friend CowString operator+(const CowString& first, const CowString& second);
    friend CowString operator+(const CowString& first, std::string_view second);
    friend CowString operator+(std::string_view first, const CowString& second);

    CowString& operator+=(const CowString& second);
    CowString& operator+=(std::string_view second);

    friend bool operator==(const CowString& first, const CowString& second);
    friend bool operator==(const CowString& first, std::string_view second);
    friend bool operator==(std::string_view first, const CowString& second);

    friend bool operator!=(const CowString& first, const CowString& second);
    friend bool operator!=(const CowString& first, std::string_view second);
    friend bool operator!=(std::string_view first, const CowString& second);

    operator std::string_view() const;

private:
    struct SharedString {
        std::string string_;
        size_t sharesCount_;

        SharedString(std::string string);
    };

    SharedString *data_;
};
