#ifndef LOG_HPP
#define LOG_HPP

#include "CoreConfig.hpp"


#if USE_FMT == 1 || !defined(__cpp_lib_format)
    #include <fmt/format.h>
    #define SPDLOG_FMT_EXTERNAL
    #define SPDLOG_WCHAR_TO_UTF8_SUPPORT
    #define AE_FMT_STR(str, ...) fmt::vformat(str, fmt::make_format_args(__VA_ARGS__))
    #define AE_FMT_WSTR(locale, str, ...) \
        fmt::vformat(locale, std::wstring_view(str), fmt::make_wformat_args(__VA_ARGS__))
#else
    #include <format>
    #define SPDLOG_USE_STD_FORMAT
    #define SPDLOG_WCHAR_TO_UTF8_SUPPORT
    #define AE_FMT_STR(str, ...) std::vformat(str, std::make_format_args(__VA_ARGS__))
    #define AE_FMT_WSTR(locale, str, ...) \
        std::vformat(locale, std::wstring_view(str), std::make_wformat_args(__VA_ARGS__))
#endif

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>


namespace AnEngine {
    class Log {
    private:
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;

    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }

        inline static std::shared_ptr<spdlog::logger>& getClientLogger() {
            return clientLogger;
        }
    };
}  // namespace AnEngine


// core engine logging macros
// clang-format off
#define AE_CORE_TRACE(str, ...)     ::AnEngine::Log::getCoreLogger()->trace(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_CORE_INFO(str, ...)      ::AnEngine::Log::getCoreLogger()->info(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_CORE_WARN(str, ...)      ::AnEngine::Log::getCoreLogger()->warn(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_CORE_ERROR(str, ...)     ::AnEngine::Log::getCoreLogger()->error(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_CORE_CRITICAL(str, ...)  ::AnEngine::Log::getCoreLogger()->critical(str  __VA_OPT__(,) __VA_ARGS__)

#ifdef AE_DEBUG_FLAG
    #define AE_CORE_DEBUG(str, ...) ::AnEngine::Log::getCoreLogger()->debug(str  __VA_OPT__(,) __VA_ARGS__)
#else
    #define AE_CORE_DEBUG
#endif


// client logging maros
#define AE_TRACE(str, ...)     ::AnEngine::Log::getClientLogger()->trace(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_INFO(str, ...)      ::AnEngine::Log::getClientLogger()->info(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_WARN(str, ...)      ::AnEngine::Log::getClientLogger()->warn(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_ERROR(str, ...)     ::AnEngine::Log::getClientLogger()->error(str  __VA_OPT__(,) __VA_ARGS__)
#define AE_CRITICAL(str, ...)  ::AnEngine::Log::getClientLogger()->critical(str  __VA_OPT__(,) __VA_ARGS__)

#ifdef _DEBUG
    #define AE_DEBUG(str, ...) ::AnEngine::Log::getClientLogger()->debug(str  __VA_OPT__(,) __VA_ARGS__)
#else
    #define AE_DEBUG
#endif
// clang-format on

#endif
