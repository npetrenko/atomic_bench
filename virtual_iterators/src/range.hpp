#pragma once

#include <any>
#include <type_traits>

struct IRangeBase {
    virtual ~IRangeBase() = default;

    template <class Impl>
    Impl& As() {
        return static_cast<Impl&>(*this);
    }
};

class IRangeBaseStorage {
public:
    IRangeBase& Get() {
        return reinterpret_cast<IRangeBase&>(storage_);
    }

    template <class Derived>
    void Emplace(Derived&& derived) {
        using Decayed = std::decay_t<Derived>;
        static_assert(Capacity <= sizeof(Decayed) &&
                      Alignment <= alignof(Decayed) &&
                      Alignment % alignof(Decayed) == 0);
        static_assert(std::is_base_of_v<IRangeBase, Decayed>);

        MaybeDestroy();
        new (&storage_) Decayed(std::forward<Derived>(derived));
    }

private:
    void MaybeDestroy() {
        if (!empty_) {
            empty_ = true;
            Get().~IRangeBase();
        }
    }

    static constexpr Capacity = 4 * sizeof(void*);
    static constexpr Alignment = alignof(void*);

    std::aligned_storage_t<Capacity, Alignment> storage_;
    bool empty_{true};
};

template <class Iterator>
struct IRange : IRangeBase {
    T* Next() {
        ++first;
        if (first == last) {
            return nullptr;
        }
        T* ptr = &*first;
        return ptr;
    }

    T* InitialNext() {
        if (first == last) {
            return nullptr;
        }
        T* ptr = &*first;
        return ptr;
    }

    Iterator first, last;
};

using AnyFunc = void(*)(void);

template <class T>
class RawIRange {
public:
    T* Next() {
    }

private:
    using Invoker = T* (*)(IRangeBase&, AnyFunc*);

    Invoker invoker_;

    T* RunInvoker() {
        return invoker_(, &invoker_);
    }

    template <class RangeImpl>
    static Invoker CreateInvoker() {
        return InitialInvoker<RangeImpl>;
    }

    template <class RangeImpl>
    static T* InitialInvoker(IRangeBase& base, AnyFunc* self) {
        auto* ptr = base.As<RangeImpl>().InitialNext();
        *self = FinalInvoker<RangeImpl>;
        return ptr;
    }

    template <class RangeImpl>
    static T* FinalInvoker(IRangeBase& base, AnyFunc*) {
        return base.As<RangeImpl>().Next();
    };
};
