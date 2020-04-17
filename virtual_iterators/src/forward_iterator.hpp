#pragma once

#include <any>
#include <cassert>
#include <cstdint>

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
        : obj_{iter}
        , manager_{AbstractManager::template CreateManager<Iterator>()}
    {
    }

    VFIterator(VFIterator&&) noexcept = default;
    VFIterator& operator=(VFIterator&&) noexcept = default;

    VFIterator(const VFIterator& other)
        : manager_{other.manager_}
    {
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
    class AbstractManager {
    public:
        template <class Iterator>
        static AbstractManager CreateManager() {
            return AbstractManager{[](const OpCode code, std::any& iterator, void* payload) -> uint64_t {
                auto* iter = AssertAnyCast<Iterator>(&iterator);
                switch (code) {
                    case OpCode::Increment: {
                        assert(!payload);
                        ++(*iter);
                        // placeholder value
                        return 2;
                    }
                    case OpCode::IsEqual: {
                        auto* other = AssertAnyCast<const Iterator>(static_cast<IsEqualPayload*>(payload));
                        return (*other) == (*iter);
                    }
                    case OpCode::Clone: {
                        auto* typedPayload = static_cast<ClonePayload*>(payload);
                        *typedPayload = *iter;
                        return 3;
                    }
                    case OpCode::Deref: {
                        assert(!payload);
                        pointer ptr = &**iter;
                        return reinterpret_cast<uint64_t>(ptr);
                    }
                }
            }};
        }


        void Increment(std::any& iterator) const {
            op_func_(OpCode::Increment, iterator, nullptr);
        }

        bool Equal(const std::any& first, const std::any& second) const {
            return op_func_(OpCode::IsEqual,
                            const_cast<std::any&>(first),
                            const_cast<std::any*>(&second));
        }

        std::any Clone(const std::any& iterator) const {
            std::any result;
            op_func_(OpCode::Clone, const_cast<std::any&>(iterator), &result);
            return result;
        }

        reference Deref(const std::any& iterator) const {
            uint64_t addr = op_func_(OpCode::Deref, const_cast<std::any&>(iterator), nullptr);
            auto* ptr = reinterpret_cast<pointer>(addr);
            return *ptr;
        }

    private:
        enum class OpCode {
            Increment,
            IsEqual,
            Clone,
            Deref,
        };
        using IncrementPayload = void;
        using IsEqualPayload = const std::any;
        using ClonePayload = std::any;

        // returns:
        // 2 if code == Increment (placeholder value)
        // 3 if code == Clone (placeholder value)
        // 0 or 1 if code == IsEqual
        // address of the element if code == Deref
        using OpFunc = uint64_t (*)(OpCode code, std::any& iterator, void* payload);

        explicit AbstractManager(OpFunc func) noexcept
            : op_func_{func}
        {
        }

        OpFunc op_func_;
    };

    std::any obj_;
    AbstractManager manager_;
};
