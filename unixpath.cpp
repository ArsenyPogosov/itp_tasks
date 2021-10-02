#include "unixpath.h"

#include <algorithm>

UnixPath::ParsedPath::ParsedPath(std::string_view path) {
    ParseFrom(path);
}

void UnixPath::ParsedPath::ParseFrom(std::string_view path) {
    content_.clear();
    if (!path.empty() && path[0] == '/') {
        content_.push_back("");
    }
    for (auto block_begin = path.begin(); block_begin < path.end(); ++block_begin) {
        auto block_end = std::find(block_begin, path.end(), '/');
        *this += FromBlock(Block(path.substr(block_begin - path.begin(), block_end - block_begin)));
        block_begin = block_end;
    }
}

std::string UnixPath::ParsedPath::Compose() const {
    if (content_ == std::vector<Block>{""}) {
        return "/";
    }

    std::string result;
    for (auto& block : content_) {
        result += block + '/';
    }
    result.pop_back();

    return result;
}

UnixPath::ParsedPath UnixPath::ParsedPath::FromBlock(const Block& block) {
    UnixPath::ParsedPath result;
    result.content_ = {block};

    return result;
}

void UnixPath::ParsedPath::operator+=(const ParsedPath& second_part) {
    for (const auto& block : second_part.content_) {
        if (block.empty()) {
            continue;
        } else if (block == ".") {
            if (!content_.empty()) {
                continue;
            }
            content_.push_back(block);
        } else if (block == "..") {
            while (!content_.empty() && content_.back() == ".") {
                content_.pop_back();
            }
            if (content_.empty()) {
                content_.push_back(block);
                continue;
            }
            if (content_.back().empty()) {
                continue;
            }
            if (content_.back() == "..") {
                content_.push_back(block);
                continue;
            }
            content_.pop_back();
        } else {
            content_.push_back(block);
        }
    }
}

bool UnixPath::ParsedPath::IsAbsolute() const {
    return !content_.empty() && content_[0].empty();
}

void UnixPath::ChangeDirectory(std::string_view path) {
    ParsedPath parsed_path = ParsedPath(path);
    if (parsed_path.IsAbsolute()) {
        current_path_ = parsed_path;
    } else {
        current_path_ += parsed_path;
    }
}

std::string UnixPath::GetAbsolutePath() const {
    return current_path_.Compose();
}

std::string UnixPath::GetRelativePath() const {
    ParsedPath parsed_relative_path = UnixPath::ParsedPath::FromBlock(".");
    size_t n = base_path_.content_.size();
    size_t m = current_path_.content_.size();

    size_t i1 = 0, i2 = 0;
    for (; i1 < n && i2 < m; ++i1, ++i2) {
        if (base_path_.content_[i1] != current_path_.content_[i1]) {
            break;
        }
    }
    while (i1 < n || i2 < m) {
        if (i1 < n) {
            parsed_relative_path += UnixPath::ParsedPath::FromBlock("..");
            ++i1;
            continue;
        }
        parsed_relative_path += UnixPath::ParsedPath::FromBlock(current_path_.content_[i2++]);
    }

    return parsed_relative_path.Compose();
}
