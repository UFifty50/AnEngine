#include "aepch.hpp"

#include "Core/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace AnEngine {
    std::shared_ptr<spdlog::logger> Log::coreLogger;
    std::shared_ptr<spdlog::logger> Log::clientLogger;

    void Log::init() {
        spdlog::set_pattern("%^%T [%l] %n: %v%$");

        coreLogger = spdlog::stdout_color_mt("AnEngine");
        coreLogger->set_level(spdlog::level::trace);

        clientLogger = spdlog::stdout_color_mt("App");
        clientLogger->set_level(spdlog::level::trace);
    }
}  // namespace AnEngine
