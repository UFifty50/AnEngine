#include "aepch.hpp"

#include "Core/Random.hpp"

#include <random>

namespace AnEngine {
    std::mt19937 Random::engine;
    std::mt19937_64 Random::engine64;
}  // namespace AnEngine
