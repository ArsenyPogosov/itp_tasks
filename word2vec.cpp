#include "word2vec.h"
#include <vector>

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    int64_t max = INT64_MIN;
    std::vector<size_t> max_ind;

    for (size_t i = 1; i < words.size(); ++i) {
        int64_t curr = 0;
        for (size_t j = 0; j < vectors[0].size(); ++j) {
            curr += vectors[0][j] * vectors[i][j];
        }

        if (max < curr) {
            max = curr;
            max_ind.clear();
        }

        if (max == curr) {
            max_ind.push_back(i);
        }
    }

    std::vector<std::string> res;
    for (auto& i : max_ind) {
        res.push_back(words[i]);
    }

    return res;
}
