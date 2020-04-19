#pragma once

#include "common.hpp"

#include <vector>
#include <utility>
#include <deque>
#include <list>

class NonErasedInline {
    using container = std::vector<int>;
public:
    using iterator = container::const_iterator;
    using irange = std::pair<iterator, iterator>;

    irange CreateRange(int last) {
        values_.clear();
        for (int i = 0; i < last; ++i) {
            values_.push_back(i);
        }
        return {values_.begin(), values_.end()};
    }

private:
    container values_;
};

template <template <class> class Iterator>
class ErasedInline {
public:
    using iterator = Iterator<const int>;
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
        result += benchmarks::common::Identity(*it);
    }
    return result;
}
