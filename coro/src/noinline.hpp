#pragma once

#include "generator.hpp"
#include "inline.hpp"

class Sequence {
public:
    Generator<int> Iterate(int first, int last);
};

class IRange {
 public:
    using irange = typename InlineIRange::irange;
    irange Iterate(int first, int last);

private:
    InlineIRange Impl_;
};
