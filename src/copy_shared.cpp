#include <memory>

#include "single_shared.hpp"

static std::shared_ptr<int> val = std::make_shared<int>(0);
static single_shared<int> single_val = single_shared<int>(new int(0));
static mutex_shared<int> mutex_val = mutex_shared<int>(new int(0));
static atomic_shared<int> atomic_val = atomic_shared<int>(new int(0));

std::shared_ptr<int> getCopy() {
    return val;
}

single_shared<int> getSingleCopy() {
    return single_val;
}

mutex_shared<int> getMutexCopy() {
    return mutex_val;
}

atomic_shared<int> getAtomicCopy() {
    return atomic_val;
}
