#include "search.h"

#include <cctype>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <functional>

struct StringViewHashCaseIndependent {
private:
    const uint_fast64_t p_ = 31;
    const uint_fast64_t m_ = 1e9 + 7;

public:
    size_t operator()(const std::string_view& str) const {
        uint_fast64_t result = 0;
        for (char i : str) {
            result = (result * p_ + tolower(i) - 'a' + 1) % m_;
        }

        return static_cast<size_t>(result);
    }
};

struct StringViewCompCaseIndependent {
public:
    bool operator()(const std::string_view& str1, const std::string_view& str2) const {
        if (str1.length() != str2.length()) {
            return false;
        }
        return std::equal(str1.begin(), str1.end(), str2.begin(),
                          [](auto a, auto b) { return std::tolower(a) == std::tolower(b); });
    }
};

std::string_view ReadBlock(std::string_view& text, std::function<bool(char)> is_block_part) {
    while (!text.empty() && !is_block_part(text.front())) {
        text.remove_prefix(1);
    }
    size_t end;
    for (end = 0; end < text.size() && is_block_part(text[end]); ++end) {
    }

    std::string_view result = text.substr(0, end - 0);
    text.remove_prefix(end - 0);

    return result;
}

using ParsedTextStructure = std::vector<
    std::unordered_map<std::string_view, int, StringViewHashCaseIndependent, StringViewCompCaseIndependent>>;
using ParsedQueryStructure =
    std::unordered_set<std::string_view, StringViewHashCaseIndependent, StringViewCompCaseIndependent>;

void ParseText(std::string_view text, std::vector<std::string_view>& text_block, std::vector<int>& text_block_size,
               ParsedTextStructure& text_parsed) {
    text_block.clear();
    text_block_size.clear();
    text_parsed.clear();

    while (!text.empty()) {
        std::string_view block = ReadBlock(text, [](char ch) { return ch != '\n'; });
        if (block.empty()) {
            break;
        }

        text_block.push_back(block);
        text_block_size.push_back(0);
        text_parsed.push_back({});
        while (!block.empty()) {
            std::string_view word = ReadBlock(block, isalpha);
            if (block.empty()) {
                break;
            }
            ++text_parsed.back()[word];
            ++text_block_size.back();
        }
    }
}

void ParseQuery(std::string_view query, ParsedQueryStructure& query_parsed) {
    while (!query.empty()) {
        std::string_view word;
        word = ReadBlock(query, isalpha);
        if (word.empty()) {
            break;
        }
        query_parsed.insert(word);
    }
}

std::vector<double> GetScore(const std::vector<int>& text_block_size, ParsedTextStructure text_parsed,
                             ParsedQueryStructure query_parsed) {
    size_t n = text_parsed.size();
    std::vector<double> score(n);
    for (auto& word : query_parsed) {
        int count = 0;
        for (auto& i : text_parsed) {
            count += (i[word] > 0);
        }
        if (count == 0) {
            continue;
        }
        double idf = log(static_cast<double>(n) / count);
        for (size_t i = 0; i < n; ++i) {
            score[i] += static_cast<double>(text_parsed[i][word]) / text_block_size[i] * idf;
        }
    }

    return score;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::vector<std::string_view> text_block;
    std::vector<int> text_block_size;
    ParsedTextStructure text_parsed;
    ParseText(text, text_block, text_block_size, text_parsed);

    ParsedQueryStructure query_parsed;
    ParseQuery(query, query_parsed);

    std::vector<double> score = GetScore(text_block_size, text_parsed, query_parsed);

    std::vector<size_t> ordered_score_indexes(score.size());
    std::iota(ordered_score_indexes.begin(), ordered_score_indexes.end(), 0);
    std::sort(ordered_score_indexes.begin(), ordered_score_indexes.end(),
              [&](size_t a, size_t b) { return score[a] > score[b]; });

    std::vector<std::string_view> result;
    for (size_t i = 0; i < results_count && i < score.size(); ++i) {
        if (score[ordered_score_indexes[i]] > 0) {
            result.push_back(text_block[ordered_score_indexes[i]]);
        }
    }

    return result;
}
