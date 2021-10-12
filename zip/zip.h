#pragma once

#include <list>
#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class Zipped {
public:
    class ConstIterator {
    public:
        const ZippedPair operator*() const;
        const ZippedPair operator->() const;

        ConstIterator& operator++();
        ConstIterator operator++(int);

        bool operator==(const ConstIterator& second) const;
        bool operator!=(const ConstIterator& second) const;

    private:
        Iterator first_, second_;

        ConstIterator(Iterator first, Iterator second);

        friend Zipped;
    };

    Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);

    ConstIterator begin() const;
    ConstIterator end() const;

private:
    ConstIterator begin_;
    ConstIterator end_;
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
