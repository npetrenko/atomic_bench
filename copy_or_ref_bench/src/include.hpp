#pragma once

#include <stdint.h>

struct Skinny {
    uint64_t a;

    auto Sum() const {
        return a;
    }
};

struct Fat {
    uint64_t a, b;

    auto Sum() const {
        return a + b;
    }
};

struct VeryFat {
    uint64_t a, b, c, d;

    auto Sum() const {
        return a + b + c + d;
    }
};

template <class T>
uint64_t ByValue(T val);

template <class T>
uint64_t ByRef(const T& val);

struct ByValueExecutor {
    template <class T>
    auto operator()(T&& val) const {
        return RunByValue(val);
    }
};

struct ByRefExecutor {
    template <class T>
    auto operator()(T&& val) const {
        return RunByRef(val);
    }
};
