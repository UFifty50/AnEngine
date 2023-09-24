#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

#include "Core/Concepts.hpp"

#undef min
#undef max


namespace AnEngine {
    class Random {
    public:
        static void init() { engine.seed(std::random_device()()); }

        static float getFloat() { return getFloat(0.0f, 1.0f); }

        static float getFloat(float min, float max) {
            std::uniform_real_distribution<float> distribution(min, max);
            return distribution(engine);
        }

        static int getInt() { return getInt(0, std::numeric_limits<int>::max()); }

        static int getInt(int min, int max) {
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(engine);
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
    };
}  // namespace AnEngine

#endif
