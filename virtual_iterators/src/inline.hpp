#pragma once

#include "forward_iterator.hpp"

#include <vector>
#include <utility>

class NonErasedInline {
public:
    using iterator = std::vector<int>::const_iterator;
    using irange = std::pair<iterator, iterator>;

    irange CreateRange(int last) {
        values_.clear();
        for (int i = 0; i < last; ++i) {
            values_.push_back(i);
        }
        return {values_.begin(), values_.end()};
    }

private:
    std::vector<int> values_;
};

class ErasedInline {
public:
    using iterator = VFIterator<const int>;
    using irange = std::pair<iterator, iterator>;

    irange CreateRange(int last) {
        auto range = range_.CreateRange(last);
        return {iterator{range.first}, iterator{range.second}};
    }

private:
    NonErasedInline range_;
};

template <class Iterator>
int ReduceSum(std::pair<Iterator, Iterator> range) {
    int result = 0;
    for (auto it = range.first; it != range.second; ++it) {
        result += *it;
    }
    return result;
}
