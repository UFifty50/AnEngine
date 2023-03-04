#ifndef LOG_HPP
#define LOG_HPP

#include <memory>

#include "Core.hpp"
#include "spdlog/spdlog.h"

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
}

// core engine logging macros
#define AE_CORE_TRACE(...)    ::AnEngine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define AE_CORE_INFO(...)     ::AnEngine::Log::getCoreLogger()->info(__VA_ARGS__)
#define AE_CORE_WARN(...)     ::AnEngine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define AE_CORE_ERROR(...)    ::AnEngine::Log::getCoreLogger()->error(__VA_ARGS__)
#define AE_CORE_CRITICAL(...) ::AnEngine::Log::getCoreLogger()->critical(__VA_ARGS__)

#ifdef AE_DEBUG
   #define AE_CORE_DEBUG(...) ::AnEngine::Log::getCoreLogger()->debug(__VA_ARGS__)
#endif


//client logging maros
#define AE_TRACE(...)         ::AnEngine::Log::getClientLogger()->trace(__VA_ARGS__)
#define AE_INFO(...)          ::AnEngine::Log::getClientLogger()->info(__VA_ARGS__)
#define AE_WARN(...)          ::AnEngine::Log::getClientLogger()->warn(__VA_ARGS__)
#define AE_ERROR(...)         ::AnEngine::Log::getClientLogger()->error(__VA_ARGS__)
#define AE_CRITICAL(...)      ::AnEngine::Log::getClientLogger()->critical(__VA_ARGS__)

#ifdef _DEBUG
   #define AE_DEBUG(...)      ::AnEngine::Log::getClientLogger()->debug(__VA_ARGS__)
#endif


#endif
