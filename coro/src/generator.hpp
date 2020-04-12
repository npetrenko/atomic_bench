#pragma once

#include "defer.hpp"

#include <experimental/coroutine>
#include <optional>

using std::experimental::coroutine_handle;

template <class T>
class YieldPromise;

template <class T>
class Generator {
    using Storage = T*;
    using Handle = coroutine_handle<>;

public:
    using promise_type = YieldPromise<T>;
    
    explicit Generator(Storage storage, Handle handle)
        : storage_(storage)
        , handle_(std::move(handle))
    {
    }

    Generator(Generator&&) = default;
    Generator& operator=(Generator&&) = default;

    ~Generator() {
        if (handle_) {
            handle_.destroy();
        }
    }

    T* Next() {
        if (!handle_) {
            return nullptr;
        }
        handle_();
        auto defer_cleanup = Defer([&]() noexcept {
            if (handle_.done()) {
                DoDestroy();
            }
        });

        return storage_;
    }

private:
    void DoDestroy() {
        handle_.destroy();
        handle_ = Handle{};
    }

    Storage storage_{};
    Handle handle_{};
};

template <class T>
class YieldPromise {
    struct Storage {
    public:
        T* get() {
            return &Value_;
        }

        Storage() = default;
        Storage(Storage&&) = delete;
        Storage& operator=(Storage&&) = delete;

        template <class S>
        void emplace(S&& val) noexcept(std::is_nothrow_constructible_v<T,S&&>) {
            Destroy();
            new (&Value_) T(std::forward<S>(val));
            isEmpty_ = false;
        }

        ~Storage() {
            Destroy();
        }
        
    private:
        void Destroy() {
            if (!isEmpty_) {
                Value_.~T();
            }
            isEmpty_ = true;
        }

        bool isEmpty_ = true;
        union {
            T Value_;
        };
    };

    using Handle = coroutine_handle<YieldPromise<T>>;

public:
    auto initial_suspend() {
        return std::experimental::suspend_always{};
    }

    auto final_suspend() {
        return std::experimental::suspend_always{};
    }

    void unhandled_exception() {
        throw;
    }

    Generator<T> get_return_object() {
        return Generator<T>{storage_.get(), Handle::from_promise(*this)};
    }

    template <class S>
    auto yield_value(S&& value) {
        StoreValue(std::forward<S>(value));
        return std::experimental::suspend_always{};
    }

    void return_void() {
    }

private:
    template <class S>
    void StoreValue(S&& value) {
        storage_.emplace(std::forward<S>(value));
    }

    bool empty_ = true;
    Storage storage_;
};
