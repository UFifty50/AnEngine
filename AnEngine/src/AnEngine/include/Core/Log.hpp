#ifndef LOG_HPP
#define LOG_HPP

#include "aepch.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include "Core/Core.hpp"


namespace AnEngine {
    class AE_API Log {
    private:
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;

    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger>& getCoreLogger() {
            return coreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& getClientLogger() {
            return clientLogger;
        }
    };
}  // namespace AnEngine

// core engine logging macros
// clang-format off
#define AE_CORE_TRACE(str, ...)     ::AnEngine::Log::getCoreLogger()->trace(str VAOPTC __VA_ARGS__)
#define AE_CORE_INFO(str, ...)      ::AnEngine::Log::getCoreLogger()->info(str VAOPTC __VA_ARGS__)
#define AE_CORE_WARN(str, ...)      ::AnEngine::Log::getCoreLogger()->warn(str VAOPTC __VA_ARGS__)
#define AE_CORE_ERROR(str, ...)     ::AnEngine::Log::getCoreLogger()->error(str VAOPTC __VA_ARGS__)
#define AE_CORE_CRITICAL(str, ...)  ::AnEngine::Log::getCoreLogger()->critical(str VAOPTC __VA_ARGS__)

#ifdef AE_DEBUG_FLAG
    #define AE_CORE_DEBUG(str, ...) ::AnEngine::Log::getCoreLogger()->debug(str VAOPTC __VA_ARGS__)
#else
    #define AE_CORE_DEBUG
#endif


// client logging maros
#define AE_TRACE(str, ...)     ::AnEngine::Log::getClientLogger()->trace(str VAOPTC __VA_ARGS__)
#define AE_INFO(str, ...)      ::AnEngine::Log::getClientLogger()->info(str VAOPTC __VA_ARGS__)
#define AE_WARN(str, ...)      ::AnEngine::Log::getClientLogger()->warn(str VAOPTC __VA_ARGS__)
#define AE_ERROR(str, ...)     ::AnEngine::Log::getClientLogger()->error(str VAOPTC __VA_ARGS__)
#define AE_CRITICAL(str, ...)  ::AnEngine::Log::getClientLogger()->critical(str VAOPTC __VA_ARGS__)

#ifdef _DEBUG
    #define AE_DEBUG(str, ...) ::AnEngine::Log::getClientLogger()->debug(str VAOPTC __VA_ARGS__)
#else
    #define AE_DEBUG
#endif
// clang-format on

#endif
