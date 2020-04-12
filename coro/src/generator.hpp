#pragma once

#include "defer.hpp"

#include <experimental/coroutine>
#include <optional>

using std::experimental::coroutine_handle;

template <class T>
class YieldPromise;

template <class T>
class Generator {
    using Storage = std::optional<T>;
    using Handle = coroutine_handle<>;

public:
    using promise_type = YieldPromise<T>;
    
    explicit Generator(Storage* storage, Handle handle)
        : storage_(storage)
        , handle_(std::move(handle))
    {
    }

    Generator(Generator&&) = default;

    std::optional<T> Next() {
        if (!handle_) {
            return std::nullopt;
        }
        handle_();
        auto defer_cleanup = Defer([&]() noexcept {
            if (handle_.done()) {
                DoDestroy();
            } else {
                storage_->reset();
            }
        });

        return std::move(*storage_);
    }

private:
    void DoDestroy() {
        assert(handle_);
        handle_.destroy();
        handle_ = Handle{};
    }

    Storage* storage_;
    Handle handle_;
};

template <class T>
class YieldPromise {
    using Storage = std::optional<T>;
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
        return Generator<T>{&storage_, Handle::from_promise(*this)};
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

    Storage storage_;
};
