#include "edgeKeyHash.h"

size_t EdgeKeyHash::operator()(const std::pair<int,int>& p) const noexcept {
    return (static_cast<size_t>(p.first) << 32) ^ static_cast<size_t>(p.second);
}
