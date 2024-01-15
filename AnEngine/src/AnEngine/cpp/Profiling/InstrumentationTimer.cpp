#include "aepch.hpp"

#include "Profiling/InstrumentationTimer.hpp"

#include <algorithm>
#include <chrono>
#include <string>
#include <thread>

#include "Profiling/Instrumentor.hpp"


namespace AnEngine {
    InstrumentationTimer::InstrumentationTimer(const std::string& name)
        : result({name, 0, 0, 0}), stopped(false) {
        startPoint = std::chrono::steady_clock::now();
    }

    InstrumentationTimer::~InstrumentationTimer() {
        if (!stopped) stop();
    }

    void InstrumentationTimer::stop() {
        auto endPoint = std::chrono::steady_clock::now();

        result.start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint)
                           .time_since_epoch()
                           .count();
        result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint)
                         .time_since_epoch()
                         .count();

        result.threadID =
            (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());

        Instrumentor::Get().writeProfile(result);

        stopped = true;
    }
};  // namespace AnEngine
