#ifndef CORE_HPP
#define CORE_HPP


#ifdef AE_WIN
    #ifdef AE_DLL
        #define AE_API __declspec(dllexport)
    #else
        #define AE_API __declspec(dllimport)
    #endif
#else
    #error AnEngine only supports windows (currently)
#endif

#if defined(AE_DEBUG_FLAG) and not defined(AE_ENABLE_ASSERTS)
    #define AE_ENABLE_ASSERTS
#endif

#ifdef AE_ENABLE_ASSERTS
    #define AE_ASSERT(x, ...) { if(!(x)) { AE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define AE_CORE_ASSERT(x, ...) { if(!(x)) { AE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define AE_ASSERT(x, ...)
    #define AE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&Application::fn, this, std::placeholders::_1)

#endif
