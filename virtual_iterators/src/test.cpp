#include "forward_iterator.hpp"

#include <gtest/gtest.h>

TEST(Basic, First) {
    std::vector<int> values{1,2,3};
    VFIterator<int> begin{values.begin()};
    VFIterator<int> end{values.end()};

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