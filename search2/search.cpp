#include "search.h"

#include <functional>
#include <cmath>
#include <numeric>

size_t SearchEngine::StringViewHashCaseIndependent::operator()(const std::string_view& str) const {
    uint_fast64_t result = 0;
    for (char i : str) {
        result = (result * p_ + std::tolower(i) - 'a' + 1) % m_;
    }

    return static_cast<size_t>(result);
}
bool SearchEngine::StringViewCompCaseIndependent::operator()(std::string_view str1, std::string_view str2) const {
    if (str1.length() != str2.length()) {
        return false;
    }
    return std::equal(str1.begin(), str1.end(), str2.begin(),
                      [](auto a, auto b) { return std::tolower(a) == std::tolower(b); });
}

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

SearchEngine::ParsedDocument::ParsedDocument(std::string_view document) {
    ParseFrom(document);
}

void SearchEngine::ParsedDocument::ParseFrom(std::string_view document) {
    word_count_ = 0;
    while (!document.empty()) {
        std::string_view word = ReadBlock(document, isalpha);
        if (document.empty()) {
            break;
        }
        ++content_[word];
        ++word_count_;
    }
}

SearchEngine::ParsedQuery::ParsedQuery(std::string_view query) {
    ParseFrom(query);
}

void SearchEngine::ParsedQuery::ParseFrom(std::string_view query) {
    while (!query.empty()) {
        std::string_view word;
        word = ReadBlock(query, isalpha);
        if (word.empty()) {
            break;
        }
        content_.insert(word);
    }
}

std::vector<double> SearchEngine::ComputeScores(SearchEngine::ParsedQuery query) const {
    size_t n = documents_.size();
    std::vector<double> scores(n);

    for (auto& word : query.content_) {
        int count = 0;
        for (auto& i : parsed_documents_) {
            count += i.content_.count(word);
        }
        if (count == 0) {
            continue;
        }

        double idf = log(static_cast<double>(n) / count);
        for (size_t i = 0; i < n; ++i) {
            const auto it = parsed_documents_[i].content_.find(word);
            int count = (it != parsed_documents_[i].content_.end() ? it->second : 0);
            scores[i] += static_cast<double>(count) / parsed_documents_[i].word_count_ * idf;
        }
    }

    return scores;
}

void SearchEngine::BuildIndex(std::string_view text) {
    while (!text.empty()) {
        std::string_view document = ReadBlock(text, [](char ch) { return ch != '\n'; });
        if (document.empty()) {
            continue;
        }

        documents_.push_back(document);
        parsed_documents_.push_back(ParsedDocument(document));
    }
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    std::vector<double> scores = ComputeScores(ParsedQuery(query));

    std::vector<size_t> ordered_score_indexes(scores.size());
    std::iota(ordered_score_indexes.begin(), ordered_score_indexes.end(), 0);
    std::sort(ordered_score_indexes.begin(), ordered_score_indexes.end(),
              [&](size_t a, size_t b) { return scores[a] > scores[b]; });

    std::vector<std::string_view> result;
    for (size_t i = 0; i < results_count && i < scores.size(); ++i) {
        if (scores[ordered_score_indexes[i]] > 0) {
            result.push_back(documents_[ordered_score_indexes[i]]);
        }
    }

    return result;
}