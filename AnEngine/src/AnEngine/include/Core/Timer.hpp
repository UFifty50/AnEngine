#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>


namespace AnEngine {
    class Timer {
    public:
        Timer() { reset(); }

        void reset() { start = std::chrono::high_resolution_clock::now(); }

        float elapsedMs() {
            return (float)std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::high_resolution_clock::now() - start)
                .count();
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    };
}  // namespace AnEngine

#endif
