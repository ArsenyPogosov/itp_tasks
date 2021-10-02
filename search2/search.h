#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class SearchEngine {
private:
    class StringViewHashCaseIndependent {
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

    class StringViewCompCaseIndependent {
    public:
        bool operator()(std::string_view str1, std::string_view str2) const {
            if (str1.length() != str2.length()) {
                return false;
            }
            return std::equal(str1.begin(), str1.end(), str2.begin(),
                              [](auto a, auto b) { return std::tolower(a) == std::tolower(b); });
        }
    };

    class ParsedDocument {
    public:
        unsigned int word_count_;
        std::unordered_map<std::string_view, int, StringViewHashCaseIndependent, StringViewCompCaseIndependent>
            content_;

        explicit ParsedDocument(std::string_view document);
        void ParseFrom(std::string_view document);
    };

    class ParsedQuery {
    public:
        std::unordered_set<std::string_view, StringViewHashCaseIndependent, StringViewCompCaseIndependent> content_;

        explicit ParsedQuery(std::string_view query);
        void ParseFrom(std::string_view query);
    };

    std::vector<std::string_view> documents_;
    std::vector<ParsedDocument> parsed_documents_;

    std::vector<double> ComputeScores(ParsedQuery query) const;

public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;
};
