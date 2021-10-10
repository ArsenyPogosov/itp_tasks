#include "cow_string.h"

char CowString::CharProxy::operator=(char second) {
    if (second == char(*this)) {
        return second;
    }

    if (ptr_->data_->sharesCount_ == 1) {
        return ptr_->data_->string_[i_] = second;
    }

    std::string new_string = ptr_->data_->string_;
    new_string[i_] = second;
    *ptr_ = CowString(new_string);
    return second;
}

CowString::CharProxy::operator char() const {
    return ptr_->data_->string_[i_];
}

CowString::CharProxy::CharProxy(CowString* ptr, size_t i) {
    ptr_ = ptr;
    i_ = i;
}

CowString::ConstCharProxy::operator char() const {
    return ptr_->data_->string_[i_];
}

CowString::ConstCharProxy::ConstCharProxy(const CowString* ptr, size_t i) {
    ptr_ = ptr;
    i_ = i;
}

CowString::CowString(std::string_view string) {
    data_ = new SharedString(std::string(string));
    ++(data_->sharesCount_);
}

CowString::CowString(const CowString& from) {
    data_ = from.data_;
    ++(data_->sharesCount_);
}

CowString::CowString(CowString&& from) {
    data_ = from.data_;
    from.data_ = nullptr;
}

CowString::~CowString() {
    if (data_) {
        --(data_->sharesCount_);
        if (!data_->sharesCount_) {
            delete data_;
        }
    }
}

CowString& CowString::operator=(const CowString& from) {
    if (&from == this) {
        return *this;
    }

    --(data_->sharesCount_);
    if  (!data_->sharesCount_){
        delete data_;
    }
    data_ = from.data_;
    ++(data_->sharesCount_);

    return *this;
}

CowString& CowString::operator=(CowString&& from) {
    if (&from == this) {
        return *this;
    }

    --(data_->sharesCount_);
    if  (!data_->sharesCount_){
        delete data_;
    }
    data_ = from.data_;
    from.data_ = nullptr;
    return *this;
}

char* CowString::GetData() const {
    return data_->string_.data();
}

CowString::Iterator CowString::begin() {
    return Iterator({this, 0});
}

CowString::Iterator CowString::end() {
    return Iterator({this, data_->string_.size()});
}

CowString::ConstIterator CowString::begin() const {
    return ConstIterator({this, 0});
}

CowString::ConstIterator CowString::end() const {
    return ConstIterator({this, data_->string_.size()});
}

CowString::CharProxy CowString::operator[](size_t i) {
    return CharProxy(this, i);
}

CowString::ConstCharProxy CowString::At(size_t i) const {
    return ConstCharProxy(this, i);
}

CowString operator+(const CowString& first, const CowString& second) {
    if (first.data_->string_.empty()) {
        return second;
    }
    if (second.data_->string_.empty()) {
        return first;
    }
    return CowString(first.data_->string_ + second.data_->string_);
}

CowString operator+(const CowString& first, std::string_view second) {
    if (second.empty()) {
        return first;
    }
    return CowString(first.data_->string_ + std::string(second));
}

CowString operator+(std::string_view first, const CowString& second) {
    if (first.empty()) {
        return second;
    }
    return CowString(std::string(first) + second.data_->string_);
}

CowString& CowString::operator+=(const CowString& second) {
    if (second.data_->string_.empty()) {
        return *this;
    }

    *this = CowString(data_->string_ + second);
    return *this;
}

CowString& CowString::operator+=(std::string_view second) {
    if (second.empty()) {
        return *this;
    }

    *this = CowString(data_->string_ + second);
    return *this;
}

bool operator==(const CowString& first, const CowString& second) {
    return first.data_->string_ == second.data_->string_;
}

bool operator==(const CowString& first, std::string_view second) {
    return first.data_->string_ == second;
}

bool operator==(std::string_view first, const CowString& second) {
    return first == second.data_->string_;
}

bool operator!=(const CowString& first, const CowString& second) {
    return first.data_->string_ != second.data_->string_;
}

bool operator!=(const CowString& first, std::string_view second) {
    return first.data_->string_ != second;
}

bool operator!=(std::string_view first, const CowString& second) {
    return first != second.data_->string_;
}

CowString::operator std::string_view() const {
    return std::string_view(data_->string_);
}

CowString::SharedString::SharedString(std::string string) {
    string_ = string;
    sharesCount_ = 0;
}