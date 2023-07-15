#ifndef LOG_HPP
#define LOG_HPP

#include "aepch.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include "Core.hpp"


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

// #define AE_FMT_STRING(str, ...) fmt::vformat(str, fmt::make_format_args(__VA_ARGS__))
// #define AE_FMT_STRING(str, ...) str, __VA_ARGS__

// clang-format off
//#define AE_FMT_STRING(str) fmt::runtime(str)
#define AE_FMT_STRING(str) str

// core engine logging macros
#define AE_CORE_TRACE(str, ...)     ::AnEngine::Log::getCoreLogger()->trace(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_CORE_INFO(str, ...)      ::AnEngine::Log::getCoreLogger()->info(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_CORE_WARN(str, ...)      ::AnEngine::Log::getCoreLogger()->warn(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_CORE_ERROR(str, ...)     ::AnEngine::Log::getCoreLogger()->error(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_CORE_CRITICAL(str, ...)  ::AnEngine::Log::getCoreLogger()->critical(AE_FMT_STRING(str), __VA_ARGS__)

#ifdef AE_DEBUG_FLAG
    #define AE_CORE_DEBUG(str, ...) ::AnEngine::Log::getCoreLogger()->debug(AE_FMT_STRING(str), __VA_ARGS__)
#else
    #define AE_CORE_DEBUG
#endif


// client logging maros
#define AE_TRACE(str, ...)     ::AnEngine::Log::getClientLogger()->trace(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_INFO(str, ...)      ::AnEngine::Log::getClientLogger()->info(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_WARN(str, ...)      ::AnEngine::Log::getClientLogger()->warn(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_ERROR(str, ...)     ::AnEngine::Log::getClientLogger()->error(AE_FMT_STRING(str), __VA_ARGS__)
#define AE_CRITICAL(str, ...)  ::AnEngine::Log::getClientLogger()->critical(AE_FMT_STRING(str), __VA_ARGS__)

#ifdef _DEBUG
    #define AE_DEBUG(str, ...) ::AnEngine::Log::getClientLogger()->debug(AE_FMT_STRING(str), __VA_ARGS__)
#else
    #define AE_DEBUG
#endif
// clang-format on

#endif
