#include <memory>

std::shared_ptr<int> val = std::make_shared<int>(0);

std::shared_ptr<int> getCopy() {
    return val;
}
