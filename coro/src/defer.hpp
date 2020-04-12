#pragma once

#include <type_traits>
#include <utility>

template <class Func>
class [[nodiscard]] Defer {
public:
    static_assert(std::is_nothrow_invocable_v<Func&>);
    
    template <class F>
    explicit Defer(F&& func)
        noexcept(std::is_nothrow_constructible_v<Func, F&&>)
        : Func_{std::forward<F>(func)}
    {
    }

    Defer(Defer&&) = delete;
    Defer& operator=(const Defer&&) = delete;

    ~Defer() {
        Func_();
    }

 private:
    Func Func_;
};

template <class T>
Defer(T) -> Defer<T>;
