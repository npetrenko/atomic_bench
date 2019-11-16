#include <memory>

#include "single_shared.hpp"

static std::shared_ptr<int> val = std::make_shared<int>(0);
static single_shared<int> single_val = single_shared<int>(new int(0));

std::shared_ptr<int> getCopy() {
    return val;
}

single_shared<int> getSingleCopy() {
    return single_val;
}
