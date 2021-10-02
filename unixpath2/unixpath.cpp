#include "unixpath.h"

#include <algorithm>

UnixPath::ParsedPath::ParsedPath(std::string_view path) : content_(0) {
    ParseFrom(path);
}

void UnixPath::ParsedPath::ParseFrom(std::string_view path) {
    content_.clear();
    for (auto block_begin = path.begin(); block_begin < path.end(); ++block_begin) {
        auto block_end = std::find(block_begin, path.end(), '/');
        *this += FromBlock(Block(path.substr(block_begin - path.begin(), block_end - block_begin)));
    }
}

std::string UnixPath::ParsedPath::Compose() const {
    std::string result;
    for (auto& block : content_) {
        result += static_cast<std::string>(block) + '/';
    }
    if (content_.size() > 1) {
        result.pop_back();
    }

    return result;
}

UnixPath::ParsedPath UnixPath::ParsedPath::FromBlock(const Block& block) {
    UnixPath::ParsedPath result;
    result.content_ = {block};

    return result;
}

UnixPath::ParsedPath UnixPath::ParsedPath::RootPath() {
    return FromBlock("");
}

void UnixPath::ParsedPath::operator+=(const ParsedPath& second_part) {
    for (const auto& block : second_part.content_) {
        if (block.empty()) {
            continue;
        } else if (block == ".") {
            continue;
        } else if (block == "..") {
            if (content_.size() > 1) {
                content_.pop_back();
            }
        } else {
            content_.push_back(block);
        }
    }
}

bool UnixPath::ParsedPath::IsAbsolute() const {
    return content_[0].empty();
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
    ParsedPath parsed_relative_path = UnixPath::ParsedPath::RootPath();
    size_t n = base_path_.content_.size();
    size_t m = current_path_.content_.size();

    size_t i1 = 0, i2 = 0;
    for (; i1 < n && i2 < m; ++i1, ++i2) {
        if (base_path_.content_[i1] != current_path_.content_[i1]) {
            break;
        }
    }
    while (i1 < n || i2 < m) {
        if (i2 >= m) {
            parsed_relative_path += UnixPath::ParsedPath::FromBlock("..");
            continue;
        }
        if (i1 >= n) {
            parsed_relative_path += UnixPath::ParsedPath::FromBlock(current_path_.content_[i2++]);
            continue;
        }
    }

    std::string result = parsed_relative_path.Compose();
    return (result.empty() ? "." : result);
}
