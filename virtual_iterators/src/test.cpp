#include "v1.hpp"
#include "v2.hpp"

#include <gtest/gtest.h>

template <template <class> class Iterator>
void BasicFirst() {
    std::vector<int> values{1,2,3};
    Iterator<int> begin{values.begin()};
    Iterator<int> end{values.end()};

    ASSERT_NE(begin, end);
    ASSERT_EQ(*begin, 1);

    ++begin;
    ASSERT_NE(begin, end);
    ASSERT_EQ(*begin, 2);

    ++begin;
    ASSERT_NE(begin, end);
    ASSERT_EQ(*begin, 3);

    ++begin;
    ASSERT_EQ(begin, end);
}

TEST(Basic, v1) {
    BasicFirst<v1::VFIterator>();
}

TEST(Basic, v2) {
    BasicFirst<v2::VFIterator>();
}
