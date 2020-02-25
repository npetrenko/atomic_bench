#include "hidden.hpp"

void NoinlineIncrement(size_t* val) {
    *val += 1;
}

inline void Increment(size_t* val) {
    *val += 1;
}

template <>
std::function<void()> GetIncrementer<UseBind::T>(size_t* val) {
    return std::bind(Increment, val);
}

template <>
std::function<void()> GetIncrementer<UseBind::F>(size_t* val) {
    return [=] { *val += 1; };
}
