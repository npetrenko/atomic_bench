#pragma once

#include "inline.hpp"

class NonErased : NonErasedInline {
public:
    irange CreateRange(int last);
};

template <template <class> class Iterator>
class Erased : ErasedInline<Iterator> {
    using Base = ErasedInline<Iterator>;
public:
    using irange = typename Base::irange;
    irange CreateRange(int last);
};
