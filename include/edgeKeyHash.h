#ifndef EDGEKEYHASH_H
#define EDGEKEYHASH_H

#include <cstddef>
#include <utility>

struct EdgeKeyHash {
    size_t operator()(const std::pair<int,int>& p) const noexcept;
};

#endif // EDGEKEYHASH_H
