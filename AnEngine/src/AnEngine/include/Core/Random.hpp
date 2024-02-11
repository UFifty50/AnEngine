#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

#include "Core/Concepts.hpp"

#undef min
#undef max


namespace AnEngine {
    class Random {
    public:
        static void init() {
            engine.seed(std::random_device()());
            engine64.seed(std::random_device()());
        }

        static float getFloat() { return getFloat(0.0f, 1.0f); }

        static float getFloat(float min, float max) {
            std::uniform_real_distribution<float> distribution(min, max);
            return distribution(engine);
        }

        static uint32_t getInt(uint32_t min, uint32_t max) {
            std::uniform_int_distribution<uint32_t> distribution(min, max);
            return distribution(engine);
        }

        static uint32_t getInt() { return getInt(0, std::numeric_limits<uint32_t>::max()); }

        static uint64_t getInt64(uint64_t min, uint64_t max) {
            std::uniform_int_distribution<uint64_t> distribution(min, max);
            return distribution(engine64);
        }

        static uint64_t getInt64() {
            return getInt64(0, std::numeric_limits<uint64_t>::max());
        }

        template <typename Container>
            requires IndexableContainer<typename Container::value_type, Container>
        static const typename Container::value_type& choice(const Container& container) {
            if (container.size() == 0) {
                throw std::invalid_argument("Container is empty");
            }

            std::uniform_int_distribution<size_t> distribution(0, container.size() - 1);
            size_t randomIndex = distribution(engine);
            return container[randomIndex];
        }

    private:
        static std::mt19937 engine;
        static std::mt19937_64 engine64;
    };
}  // namespace AnEngine

#endif
