#include "noinline.hpp"
#include "v1.hpp"
#include "v2.hpp"

NonErased::irange NonErased::CreateRange(int last) {
    return NonErasedInline::CreateRange(last);
}

template <template <class> class Iterator>
typename Erased<Iterator>::irange
Erased<Iterator>::CreateRange(int last) {
    return ErasedInline<Iterator>::CreateRange(last);
}

template class Erased<v1::VFIterator>;
template class Erased<v2::VFIterator>;
