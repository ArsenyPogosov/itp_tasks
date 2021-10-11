#include "zip.h"

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    Zipped result;
    for (auto i = a_begin, j = b_begin; i != a_end && j != b_end; ++i, ++j) {
        result.push_back({*i, *j});
    }

    return result;
}