#include "aepch.hpp"

#include "Core/Utils/Utils.hpp"


namespace AnEngine {
    size_t hash_combine(size_t hash1, size_t hash2) {
        hash2 += 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
        return hash1 ^ hash2;
    }
};  // namespace AnEngine
