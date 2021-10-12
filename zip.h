#pragma once

#include <iterator>

template <typename Iterator1, typename Iterator2>
class Zipped {
public:
    class ConstIterator {
    public:
        using Value = std::pair<decltype(*Iterator1{}), decltype(*Iterator2{})>;

        const Value operator*() const {
            return Value{*first_, *second_};
        }
        const Value operator->() const {
            return Value{*first_, *second_};
        }

        ConstIterator& operator++() {
            ++first_;
            ++second_;
            return *this;
        }
        ConstIterator operator++(int) {
            ConstIterator result = *this;
            ++first_;
            ++second_;
            return result;
        }

        bool operator==(const ConstIterator& second) const {
            return (first_ == second.first_) || (second_ == second.second_);
        }
        bool operator!=(const ConstIterator& second) const {
            return (first_ != second.first_) && (second_ != second.second_);
        }

    private:
        Iterator1 first_;
        Iterator2 second_;

        ConstIterator(Iterator1 first, Iterator2 second) : first_(first), second_(second) {
        }

        friend Zipped;
    };

    Zipped(Iterator1 a_begin, Iterator1 a_end, Iterator2 b_begin, Iterator2 b_end)
        : begin_(a_begin, b_begin), end_(a_end, b_end){};

    ConstIterator begin() const {
        return begin_;
    };

    ConstIterator end() const {
        return end_;
    }

private:
    ConstIterator begin_;
    ConstIterator end_;
};

template <typename Sequence1, typename Sequence2>
auto Zip(const Sequence1& sequence1, const Sequence2& sequence2) {
    return Zipped(std::begin(sequence1), std::end(sequence1), std::begin(sequence2), std::end(sequence2));
}
