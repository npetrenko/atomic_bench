#include <variant>
#include <vector>
#include <random>

struct UnionVariant {
    union {
        float f_value;
        int i_value;
    };
    bool is_float;

    explicit UnionVariant(int ival) noexcept
        : is_float(false)
    {
        new (&i_value) int(ival);
    }

    explicit UnionVariant(float fval) noexcept
        : is_float(true)
    {
        new(&f_value) int(fval);
    }

    template <class Visitor>
    void Accept(Visitor&& visitor) {
        if (is_float) {
            visitor(f_value);
        } else {
            visitor(i_value);
        }
    }
};

struct SimpleVariant {
    float f_value;
    int i_value;
    bool is_float;

    explicit SimpleVariant(int ival) noexcept
        : i_value(ival)
        , is_float(false)
    {
    }

    explicit SimpleVariant(float fval) noexcept
        : f_value(fval)
        , is_float(true)
    {
    }

    template <class Visitor>
    void Accept(Visitor&& visitor) {
        if (is_float) {
            visitor(f_value);
        } else {
            visitor(i_value);
        }
    }
};

struct StlVariant : private std::variant<float, int> {
    using TBase = std::variant<float, int>;

    explicit StlVariant(int ival) noexcept
        : TBase(ival)
    {
    }

    explicit StlVariant(float fval) noexcept
        : TBase(fval)
    {
    }

    template <class Visitor>
    void Accept(Visitor&& visitor) {
        std::visit(std::forward<Visitor>(visitor), static_cast<TBase&>(*this));
    }
};

namespace NTags {
    struct Random {
    };
    struct Ints {
    };
    struct Floats {
    };
}

template <class Variant>
struct InstantiationGroup {
    static std::vector<Variant> CreateRandom(size_t, std::mt19937*);
    static std::vector<Variant> CreateInts(size_t);
    static std::vector<Variant> CreateFloats(size_t);

    static std::vector<Variant> OverloadedCreate(size_t size, std::mt19937* rd, NTags::Random) {
        return CreateRandom(size, rd);
    }

    static std::vector<Variant> OverloadedCreate(size_t size, std::mt19937*, NTags::Ints) {
        return CreateInts(size);
    }

    static std::vector<Variant> OverloadedCreate(size_t size, std::mt19937*, NTags::Floats) {
        return CreateFloats(size);
    }
};
