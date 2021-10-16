#pragma once

#include <iostream>
#include <string>
#include <charconv>
#include <exception>
#include <vector>

class CsvParserError : public std::exception {};
class CsvParserIteratorError : public std::exception {};

template <typename... T>
class CsvParser {
public:
    class Iterator {
    public:
        std::tuple<T...> operator*() {
            if (was_dereferenced_) {
                throw CsvParserIteratorError();
            }
            was_dereferenced_ = true;

            std::string current;
            getline(data_->in_, current);

            bool quotes_opened = false;
            std::vector<std::string> values{""};
            for (size_t i = 0; i < current.size(); ++i) {
                if (current[i] == data_->separator_ && !quotes_opened) {
                    values.push_back("");
                    continue;
                }

                if (current[i] == '"') {
                    size_t count = 1;
                    while (++i < current.size() && current[i] == '"') {
                        ++count;
                    }
                    --i;

                    if (!quotes_opened) {
                        --count;
                        quotes_opened = true;
                    }

                    for (size_t j = 0; j < count / 2; ++j) {
                        values.back() += '"';
                    }

                    if (count % 2) {
                        quotes_opened = !quotes_opened;
                    }

                    continue;
                }

                values.back() += current[i];
            }

            values.resize(sizeof...(T));

            std::tuple<T...> result;
            try {
                size_t i = 0;
                const auto encrement = [](size_t& i) { return i++; };
                result = std::make_tuple((Convert<T>(values[encrement(i)]))...);
            } catch (...) {
                throw CsvParserError();
            }

            return result;
        }

        Iterator& operator++() {
            if (!was_dereferenced_) {
                std::string temp;
                getline(data_->in_, temp);
            }
            was_dereferenced_ = false;

            if (data_->in_.peek() == std::istream::traits_type::eof()) {
                data_ = nullptr;
            }

            return *this;
        }

        Iterator operator++(int) {
            Iterator result = *this;
            ++*this;

            return result;
        }

        bool operator==(const Iterator& second) const {
            return data_ == second.data_;
        }
        bool operator!=(const Iterator& second) const {
            return data_ != second.data_;
        }

    private:
        bool was_dereferenced_;
        CsvParser* data_;

        Iterator(CsvParser* data) : was_dereferenced_(false), data_(data) {
        }

        friend CsvParser;
    };

    CsvParser(std::istream& in, char separator, int skip = 0) : in_(in), separator_(separator) {
        if (skip < 0) {
            throw CsvParserError();
        }

        while (skip--) {
            std::string temp;
            getline(in_, temp);
        }
    }

    Iterator begin() {
        if (in_.peek() == std::istream::traits_type::eof()) {
            return Iterator(nullptr);
        }

        return Iterator(this);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

private:
    std::istream& in_;
    char separator_;

    template <typename R>
    static typename std::enable_if<!std::is_arithmetic<R>::value, R>::type Convert(const std::string& from) {
        return static_cast<R>(from);
    }

    template <typename R>
    static typename std::enable_if<std::is_arithmetic<R>::value, R>::type Convert(const std::string& from) {
        if (from.empty()) {
            return static_cast<R>(0);
        }

        R result;
        if (std::from_chars(from.data(), from.data() + from.size(), result).ptr != from.data() + from.size()) {
            throw CsvParserError();
        }
        return result;
    }
};

template <typename... T>
std::ostream& operator<<(std::ostream& out, std::tuple<T...> t) {
    std::apply(
        [&out](auto& first, auto&... args) {
            out << first;
            ((out << ", " << args), ...);
        },
        t);

    return out;
}