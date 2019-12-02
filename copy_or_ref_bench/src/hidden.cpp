#include "include.hpp"

template <class T>
uint64_t ByValue(T val) {
    return val.Sum();
}

template <class T>
uint64_t ByRef(const T& val) {
    return val.Sum();
}

template uint64_t ByValue<Fat>(Fat);
template uint64_t ByValue<VeryFat>(VeryFat);

template uint64_t ByRef<Fat>(const Fat&);
template uint64_t ByRef<VeryFat>(const VeryFat&);

