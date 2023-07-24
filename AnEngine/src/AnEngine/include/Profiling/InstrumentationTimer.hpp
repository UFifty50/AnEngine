#ifndef INSTRUMENTATIONTIMER_HPP
#define INSTRUMENTATIONTIMER_HPP

#include <chrono>
#include <string>

#include "Profiling/Instrumentor.hpp"


namespace AnEngine {
    class InstrumentationTimer {
    public:
        InstrumentationTimer(const std::string& name);
        ~InstrumentationTimer();

        void stop();

    private:
        ProfileResult result;
        bool stopped;
        std::chrono::time_point<std::chrono::steady_clock> startPoint;
    };
};  // namespace AnEngine

#endif
