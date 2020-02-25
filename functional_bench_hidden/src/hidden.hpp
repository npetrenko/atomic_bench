#pragma once

#include <functional>

enum class UseBind {
    T,
    F,
};

template <UseBind useBind>
std::function<void()> GetIncrementer(size_t* val);

void NoinlineIncrement(size_t* val);
