#pragma once

#include "inline.hpp"

class NonErased : NonErasedInline {
public:
    irange CreateRange(int last);
};

class Erased : ErasedInline {
public:
    irange CreateRange(int last);
};
