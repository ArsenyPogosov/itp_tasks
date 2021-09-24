#include <vector>
#include <algorithm>
#include "unixpath.h"

using Block = std::string_view;

std::vector<Block> Parse(std::string_view path) {
    std::vector<Block> res;
    for (auto block_begin = path.begin(); block_begin < path.end(); ++block_begin) {
        auto block_end = std::find(block_begin, path.end(), '/');
        res.push_back(path.substr(block_begin - path.begin(), block_end - block_begin));
        block_begin = block_end;
    }

    return res;
}

std::string Compose(const std::vector<Block>& path) {
    std::string res;
    for (auto& i : path) {
        res += static_cast<std::string>(i) + '/';
    }
    if (path.size() > 1) {
        res.pop_back();
    }

    return res;
}

void Normalize(std::vector<Block>& absolute_path) {
    std::vector<Block> res;
    res.push_back("");
    for (auto& i : absolute_path) {
        if (i == "") {
            continue;
        } else if (i == ".") {
            continue;
        } else if (i == "..") {
            res.pop_back();
            if (res.empty()) {
                absolute_path = res;
                return;
            }
        } else {
            res.push_back(i);
        }
    }

    absolute_path = res;
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<Block> absolute_path = Parse(current_working_dir), relative_path = Parse(path);
    absolute_path.insert(absolute_path.end(), relative_path.begin(), relative_path.end());
    Normalize(absolute_path);

    return Compose(absolute_path);
}
