#pragma once

#include <string>
#include <string_view>
#include <vector>

class UnixPath {
private:
    class ParsedPath {
    public:
        using Block = std::string;

        std::vector<Block> content_;

        ParsedPath(){};
        explicit ParsedPath(std::string_view path);

        void ParseFrom(std::string_view path);
        std::string Compose() const;

        static ParsedPath FromBlock(const Block& block);
        static ParsedPath RootPath();

        void operator+=(const ParsedPath& second_part);

        bool IsAbsolute() const;
    };

    ParsedPath base_path_;
    ParsedPath current_path_;

public:
    UnixPath(std::string_view initial_dir) : base_path_(initial_dir){};

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;
};
