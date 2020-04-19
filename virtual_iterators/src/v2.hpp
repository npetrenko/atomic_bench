#pragma once

#include <cstdint>
#include <type_traits>
#include <cassert>
#include <iterator>

namespace v2 {
using FunctorPtr = uint64_t (*)(void* iterator, void* payload);

template <class Functor>
struct FunctorBase {
    static auto* Get(void* iterator) {
        return static_cast<typename Functor::iterator*>(iterator);
    }

    static uint64_t Call(void* iterator, void* payload) {
        return Functor{}(iterator, payload);
    }

    static FunctorPtr AsFunctorPtr() {
        return Call;
    }
};

template <class Iterator>
struct DerefFunctor : FunctorBase<DerefFunctor<Iterator>> {
    using iterator = Iterator;

    uint64_t operator()(void* iterator, void* payload) const {
        (void)payload;
        assert(!payload);
        using value_type = typename std::iterator_traits<Iterator>::value_type;
        using pointer = value_type*;
        pointer ptr = const_cast<pointer>(&**this->Get(iterator));
        return reinterpret_cast<uint64_t>(ptr);
    }
};

template <class Iterator>
struct IncrementFunctor : FunctorBase<IncrementFunctor<Iterator>> {
    using iterator = Iterator;

    uint64_t operator()(void* iterator, void* payload) const {
        (void)payload;
        assert(!payload);
        ++(*this->Get(iterator));
        // reserved return code
        return 2;
    }
};

template <class Iterator>
struct IsEqualFunctor : FunctorBase<IsEqualFunctor<Iterator>> {
    using iterator = Iterator;

    uint64_t operator()(void* iterator, void* payload) const {
        return (*this->Get(iterator)) == (*this->Get(payload));
    }
};

enum class OpCode {
    Deref,
    Increment,
    IsEqual,
};

namespace NOpCodeTag {
struct Deref {};
struct Increment {};
struct IsEqual {};
}  // namespace NOpCodeTag

template <OpCode>
struct OpCodeTag;

template <>
struct OpCodeTag<OpCode::Deref> {
    using type = NOpCodeTag::Deref;
};
template <>
struct OpCodeTag<OpCode::Increment> {
    using type = NOpCodeTag::Increment;
};
template <>
struct OpCodeTag<OpCode::IsEqual> {
    using type = NOpCodeTag::IsEqual;
};

template <OpCode op>
using OpCodeTag_t = typename OpCodeTag<op>::type;

struct Storage {
public:
    void* Get() {
        return &storage_;
    }

    template <class T>
    T* Get() {
        return static_cast<T*>(Get());
    }

    const void* Get() const {
        return &storage_;
    }

    template <class T>
    const T* Get() const {
        return static_cast<const T*>(Get());
    }

    template <class T>
    void Emplace(T&& value) noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T&&>) {
        using Decayed = std::decay_t<T>;
        AssertCanStore<Decayed>();
        new (&storage_) Decayed(std::forward<T>(value));
    }

private:
    template <class T>
    void AssertCanStore() {
        static_assert(sizeof(T) <= Capacity && alignof(T) <= Alignment &&
                      Alignment % alignof(T) == 0 && std::is_trivially_destructible_v<T> &&
                      std::is_trivially_copy_assignable_v<T> &&
                      std::is_trivially_copy_constructible_v<T>);
    }

    static constexpr size_t Capacity = sizeof(void*) * 3;
    static constexpr size_t Alignment = alignof(void*);

    std::aligned_storage_t<Capacity, Alignment> storage_;
};

class InlineDispatcher {
public:
    template <class Iterator>
    static InlineDispatcher Create() noexcept {
        InlineDispatcher dsp;
        dsp.deref_functor_ = DerefFunctor<Iterator>::AsFunctorPtr();
        dsp.increment_functor_ = IncrementFunctor<Iterator>::AsFunctorPtr();
        dsp.is_equal_functor_ = IsEqualFunctor<Iterator>::AsFunctorPtr();
        return dsp;
    }

    template <OpCode code>
    uint64_t Dispatch(void* iterator, void* payload) const {
        return Dispatch(OpCodeTag_t<code>{}, iterator, payload);
    }

private:
    uint64_t Dispatch(NOpCodeTag::Deref, void* iterator, void* payload) const {
        return deref_functor_(iterator, payload);
    }
    uint64_t Dispatch(NOpCodeTag::Increment, void* iterator, void* payload) const {
        return increment_functor_(iterator, payload);
    }
    uint64_t Dispatch(NOpCodeTag::IsEqual, void* iterator, void* payload) const {
        return is_equal_functor_(iterator, payload);
    }

    FunctorPtr deref_functor_;
    FunctorPtr increment_functor_;
    FunctorPtr is_equal_functor_;
};

class CompressedDispatcher {
public:
    template <class Iterator>
    static CompressedDispatcher Create() noexcept {
        CompressedDispatcher dsp;
        dsp.impl_ = Impl<Iterator>;
        return dsp;
    }

    template <OpCode code>
    uint64_t Dispatch(void* iterator, void* payload) const {
        return impl_(code, iterator, payload);
    }

private:
    using ImplFunc = uint64_t (*)(OpCode, void* iterator, void* payload);

    template <class Iterator>
    static uint64_t Impl(OpCode code, void* iterator, void* payload) {
        switch (code) {
            case OpCode::Deref: {
                return DerefFunctor<Iterator>{}(iterator, payload);
            }
            case OpCode::Increment: {
                return IncrementFunctor<Iterator>{}(iterator, payload);
            }
            case OpCode::IsEqual: {
                return IsEqualFunctor<Iterator>{}(iterator, payload);
            }
        }
#ifndef NDEBUG
        std::terminate();
#endif
    }

    ImplFunc impl_;
};

template <class T, class Dispatcher = InlineDispatcher>
class Manager {
public:
    template <class Iterator>
    Manager(Iterator&& iterator) noexcept(
        noexcept(Storage{}.Emplace(std::forward<Iterator>(iterator)))) {
        using Decayed = std::decay_t<Iterator>;
        storage_.Emplace(std::forward<Iterator>(iterator));
        dispatcher_ = Dispatcher::template Create<Decayed>();
    }

    T& Deref() const {
        uint64_t raw = dispatcher_.template Dispatch<OpCode::Deref>(
            const_cast<void*>(storage_.Get()), nullptr);
        auto* ptr = reinterpret_cast<T*>(raw);
        return *ptr;
    }

    void Increment() {
        auto ret = dispatcher_.template Dispatch<OpCode::Increment>(storage_.Get(), nullptr);
        assert(ret == 2);
        (void)ret;
    }

    bool IsEqual(const Manager& other) const {
        auto ret = dispatcher_.template Dispatch<OpCode::IsEqual>(
            const_cast<void*>(storage_.Get()), const_cast<void*>(other.storage_.Get()));
        assert(ret == 0 || ret == 1);
        return ret;
    }

private:
    Storage storage_;
    Dispatcher dispatcher_;
};
template <class T>
class VFIterator {
public:
    VFIterator() = default;

    template <class Iterator, std::enable_if_t<!std::is_same_v<std::decay_t<Iterator>, VFIterator>,
                                               std::nullptr_t> = nullptr>
    VFIterator(Iterator&& iterator) noexcept(
        std::is_nothrow_constructible_v<Manager<T>, Iterator&&>)
        : manager_{std::forward<Iterator>(iterator)} {
    }

    VFIterator(const VFIterator& other) = default;
    VFIterator& operator=(const VFIterator& other) = default;

    T& operator*() const {
        return manager_.Deref();
    }

    VFIterator& operator++() {
        manager_.Increment();
        return *this;
    }

    VFIterator operator++(int) {
        auto copy = *this;
        this->operator++();
        return copy;
    }

    bool operator==(const VFIterator& other) const {
        return manager_.IsEqual(other.manager_);
    }

    bool operator!=(const VFIterator& other) const {
        return !(operator==(other));
    }

private:
    Manager<T> manager_;
};
}  // namespace v2
