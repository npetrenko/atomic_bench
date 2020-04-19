#pragma once

#include <any>
#include <cassert>
#include <cstdint>

namespace v1 {

template <class T, class From>
T* AssertAnyCast(From* data) {
    auto* t = std::any_cast<T>(data);
    assert(t);
    return t;
}

template <class T>
class VFIterator {
public:
    using reference = T&;
    using pointer = T*;

    template <class Iterator>
    explicit VFIterator(Iterator iter)
        : obj_{iter}, manager_{AbstractManager::template CreateManager<Iterator>()} {
    }

    VFIterator(VFIterator&&) noexcept = default;
    VFIterator& operator=(VFIterator&&) noexcept = default;

    VFIterator(const VFIterator& other) : manager_{other.manager_} {
        obj_ = manager_.Clone(other.obj_);
    }

    VFIterator& operator=(const VFIterator& other) {
        manager_ = other.manager_;
        obj_ = manager_.Clone(other.obj_);
        return *this;
    }

    VFIterator& operator++() {
        manager_.Increment(obj_);
        return *this;
    }

    VFIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    T& operator*() const {
        return manager_.Deref(obj_);
    }

    T* operator->() const {
        return &**this;
    }

    bool operator==(const VFIterator& other) const {
        return manager_.Equal(obj_, other.obj_);
    }

    bool operator!=(const VFIterator& other) const {
        return !(*this == other);
    }

private:
    enum class OpCode {
        Deref,
        Increment,
        IsEqual,
        Clone,

        Dynamic,
    };

    struct ManagerState {
        // returns:
        // 2 if code == Increment (placeholder value)
        // 3 if code == Clone (placeholder value)
        // 0 or 1 if code == IsEqual
        // address of the element if code == Deref
        using OpFunc = uint64_t (*)(OpCode code, std::any& iterator, void* payload);

        OpFunc deref_func_;
        OpFunc inc_func_;
        OpFunc iseq_func_;
        OpFunc dyn_func_;
    };

    class AbstractManager : ManagerState {
        using Base = ManagerState;

    public:
        template <class Iterator>
        static AbstractManager CreateManager() {
            return AbstractManager{
                Manage<Iterator, OpCode::Deref>,
                Manage<Iterator, OpCode::Increment>,
                Manage<Iterator, OpCode::IsEqual>,
                Manage<Iterator, OpCode::Dynamic>,
            };
        }
        __attribute__((always_inline)) void Increment(std::any& iterator) const {
            this->inc_func_(OpCode::Increment, iterator, nullptr);
        }

        __attribute__((always_inline)) bool Equal(const std::any& first,
                                                  const std::any& second) const {
            return this->iseq_func_(OpCode::IsEqual, const_cast<std::any&>(first),
                                    const_cast<std::any*>(&second));
        }

        std::any Clone(const std::any& iterator) const {
            std::any result;
            this->dyn_func_(OpCode::Clone, const_cast<std::any&>(iterator), &result);
            return result;
        }

        __attribute__((always_inline)) reference Deref(const std::any& iterator) const {
            uint64_t addr =
                this->deref_func_(OpCode::Deref, const_cast<std::any&>(iterator), nullptr);
            auto* ptr = reinterpret_cast<pointer>(addr);
            return *ptr;
        }

    private:
        using IncrementPayload = void;
        using IsEqualPayload = const std::any;
        using ClonePayload = std::any;

    private:
        template <class... Args>
        explicit AbstractManager(Args&&... args) noexcept : Base{std::forward<Args>(args)...} {
        }

        template <class Iterator, OpCode ccode = OpCode::Dynamic>
        static uint64_t Manage(const OpCode dcode, std::any& iterator, void* payload) {
            auto* const iter = AssertAnyCast<Iterator>(&iterator);
            const auto code = ccode == OpCode::Dynamic ? dcode : ccode;

            switch (code) {
                case OpCode::Deref: {
                    assert(!payload);
                    pointer ptr = &**iter;
                    return reinterpret_cast<uint64_t>(ptr);
                }
                case OpCode::Increment: {
                    assert(!payload);
                    ++(*iter);
                    // placeholder value
                    return 2;
                }
                case OpCode::IsEqual: {
                    auto* other =
                        AssertAnyCast<const Iterator>(static_cast<IsEqualPayload*>(payload));
                    return (*other) == (*iter);
                }
                case OpCode::Clone: {
                    auto* typedPayload = static_cast<ClonePayload*>(payload);
                    *typedPayload = *iter;
                    return 3;
                }

                case OpCode::Dynamic:
                    break;
            }
            std::terminate();
        }

    private:
    };

    std::any obj_;
    AbstractManager manager_;
};

}  // namespace v1
