#include "field.h"

Field::Field(std::vector<std::vector<bool>> data) {
    data_ = data;
}

bool Field::IsFree(Point p) const {
    return p.y < data_.size() && p.x < data_[p.y].size() && !data_[p.y][p.x];
}

size_t Field::Height() const {
    return data_.size();
}

size_t Field::Width() const {
    return (data_.empty() ? 0 : data_[0].size());
}
