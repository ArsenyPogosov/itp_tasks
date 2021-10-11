#pragma once

#include <list>
#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

using Zipped = std::list<ZippedPair>;

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
