#pragma once

#include "generator.hpp"

#include <utility>
#include <vector>

class InlineSequence {
 public:
    Generator<int> Iterate(int first, int last) {
        for (int i = first; i != last; ++i) {
            co_yield i;
        }
    }
};


class InlineIRange {
 public:
    using iterator = std::vector<int>::const_iterator;
    using irange = std::pair<iterator, iterator>;

    irange Iterate(int first, int last) {
        storage_.clear();
        for (int i = first; i != last; ++i) {
            storage_.push_back(i);
        }
        return {storage_.begin(), storage_.end()};
    }

 private:
    std::vector<int> storage_;
};

template <class Iterator>
inline int ReduceSum(std::pair<Iterator, Iterator> range) {
    auto [first, last] = range;
    int value = 0;
    for (auto it = first; it != last; ++it) {
        value += *it;
    }

    return value;
}

inline int ReduceSum(Generator<int>&& generator) {
    int value = 0;
    while (true) {
        auto next = generator.Next();
        if (!next) {
            break;
        }
        value += *next;
    }

    return value;
}
