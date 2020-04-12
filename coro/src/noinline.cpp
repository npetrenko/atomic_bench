#include "noinline.hpp"

Generator<int> Sequence::Iterate(int first, int last) {
    return Impl_.Iterate(first, last);
}

Generator<int> SequenceOverRange::Iterate(int first, int last) {
    return Impl_.Iterate(first, last);
}

IRange::irange IRange::Iterate(int first, int last) {
    return Impl_.Iterate(first, last);
}
