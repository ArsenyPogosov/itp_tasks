#include "zip.h"

const ZippedPair Zipped::ConstIterator::operator*() const {
    return ZippedPair{*first_, *second_};
}

const ZippedPair Zipped::ConstIterator::operator->() const {
    return ZippedPair{*first_, *second_};
}

Zipped::ConstIterator& Zipped::ConstIterator::operator++() {
    ++first_;
    ++second_;
    return *this;
}

Zipped::ConstIterator Zipped::ConstIterator::operator++(int) {
    ConstIterator result = *this;
    ++first_;
    ++second_;
    return result;
}

bool Zipped::ConstIterator::operator==(const Zipped::ConstIterator& second) const {
    return (first_ == second.first_) || (second_ == second.second_);
}

bool Zipped::ConstIterator::operator!=(const Zipped::ConstIterator& second) const {
    return (first_ != second.first_) && (second_ != second.second_);
}

Zipped::ConstIterator::ConstIterator(Iterator first, Iterator second) : first_(first), second_(second) {
}

Zipped::Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end)
    : begin_(a_begin, b_begin), end_(a_end, b_end) {
}

Zipped::ConstIterator Zipped::begin() const {
    return begin_;
}

Zipped::ConstIterator Zipped::end() const {
    return end_;
}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    return Zipped(a_begin, a_end, b_begin, b_end);
}