#include "noinline.hpp"

NonErased::irange NonErased::CreateRange(int last) {
    return NonErasedInline::CreateRange(last);
}

Erased::irange Erased::CreateRange(int last) {
    return ErasedInline::CreateRange(last);
}
