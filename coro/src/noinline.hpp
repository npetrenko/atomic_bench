#pragma once

#include "generator.hpp"
#include "inline.hpp"

class Sequence {
public:
    Generator<int> Iterate(int first, int last);

private:
    InlineSequence Impl_;
};

class SequenceOverRange {
public:
    Generator<int> Iterate(int first, int last);

private:
    InlineSequenceOverRange Impl_;
};

class IRange {
public:
    using irange = typename InlineIRange::irange;
    irange Iterate(int first, int last);

private:
    InlineIRange Impl_;
};
