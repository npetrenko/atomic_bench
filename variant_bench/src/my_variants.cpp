#include "my_variants.hpp"

template <class Variant>
std::vector<Variant> InstantiationGroup<Variant>::CreateRandom(size_t size, std::mt19937* rd) {
    std::bernoulli_distribution distr;
    std::vector<Variant> result;
    result.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        if (distr(*rd)) {
            result.emplace_back((int)0);
        } else {
            result.emplace_back((float)0);
        }
    }
    return result;
}

template <class Variant, class T>
std::vector<Variant> CreateTypes(size_t size) {
    std::vector<Variant> result;
    result.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        result.emplace_back((T)0);
    }

    return result;
}

template <class Variant>
std::vector<Variant> InstantiationGroup<Variant>::CreateInts(size_t size) {
    return CreateTypes<Variant, int>(size);
}

template <class Variant>
std::vector<Variant> InstantiationGroup<Variant>::CreateFloats(size_t size) {
    return CreateTypes<Variant, float>(size);
}

template struct InstantiationGroup<SimpleVariant>;
template struct InstantiationGroup<StlVariant>;
template struct InstantiationGroup<UnionVariant>;
