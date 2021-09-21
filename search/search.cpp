#include <cctype>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include "search.h"

struct StringViewHashCaseIndep {
public:
    size_t operator()(const std::string_view& str) const {
        std::string s(str.size(), 'a');
        std::transform(str.begin(), str.end(), s.begin(), [](char a) { return std::tolower(a); });
        return std::hash<std::string>()(s);
    }
};

struct StringViewCompCaseIndep {
public:
    bool operator()(const std::string_view& str1, const std::string_view& str2) const {
        if (str1.length() != str2.length())
            return false;
        return std::equal(str1.begin(), str1.end(), str2.begin(),
                          [](auto a, auto b) { return std::tolower(a) == std::tolower(b); });
    }
};

void ReadWord(std::string_view::const_iterator begin, std::string_view::const_iterator end, std::string_view& word,
              std::string_view::iterator& word_end) {
    std::string_view::iterator word_begin = begin;
    for (; word_begin != end && !std::isalpha(*word_begin); ++word_begin) {
    }
    for (word_end = word_begin; word_end != end && std::isalpha(*word_end); ++word_end) {
    }
    word = std::string_view(&*word_begin, std::max(0, word_end - word_begin));
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::vector<std::string_view> text_block;
    std::vector<std::unordered_map<std::string_view, int, StringViewHashCaseIndep, StringViewCompCaseIndep>>
        text_parsed;
    std::vector<int> text_block_size;
    for (size_t line_begin = 0; line_begin < text.size();) {
        ++line_begin;
        size_t line_end = text.find('\n', line_begin);
        std::string_view line = text.substr(line_begin, line_end - line_begin);
        line_begin = line_end;

        text_block.push_back(line);
        text_parsed.push_back({});
        text_block_size.push_back(0);
        for (std::string_view::iterator word_begin = line.begin(); word_begin != line.end();) {
            std::string_view to_add;
            ReadWord(word_begin, line.end(), to_add, word_begin);
            if (to_add.size()) {
                ++text_parsed.back()[to_add];
                ++text_block_size.back();
            }
        }
    }

    std::unordered_set<std::string_view, StringViewHashCaseIndep, StringViewCompCaseIndep> query_parsed;
    for (auto word_begin = query.begin(); word_begin != query.end();) {
        std::string_view to_add;
        ReadWord(word_begin, query.end(), to_add, word_begin);
        if (!to_add.empty()) {
            query_parsed.insert(to_add);
        }
    }

    int n = text_parsed.size();
    std::vector<double> score(n);
    for (auto& word : query_parsed) {
        double idf = 0;
        for (auto& i : text_parsed) {
            idf += (i[word] > 0);
        }
        idf = log(n / idf);
        for (size_t i = 0; i < static_cast<size_t>(n); ++i) {
            score[i] += static_cast<double>(text_parsed[i][word]) / text_block_size[i] * idf;
        }
    }

    std::vector<int> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::sort(ord.begin(), ord.end(), [&](int a, int b) { return score[a] > score[b]; });

    std::vector<std::string_view> res;
    for (size_t i = 0; i < results_count && i < static_cast<size_t>(n); ++i) {
        if (score[ord[i]] > 0) {
            res.push_back(text_block[ord[i]]);
        }
    }

    return res;
}
