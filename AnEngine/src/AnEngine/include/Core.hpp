#ifndef CORE_HPP
#define CORE_HPP


#if defined(AE_WIN)
    #define DEBUG_BREAK() __debugbreak()
    #if defined(AE_DYN_LINK)
        #if defined(AE_DLL)
            #define AE_API __declspec(dllexport)
        #else
            #define AE_API __declspec(dllimport)
        #endif
    #else
        #define AE_API
    #endif
#elif defined(AE_LINUX)
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
  //  #if defined(AE_DYN_LINK)
        #if defined(AE_DLL)
            #define AE_API __attribute__((visibility("default")))
        #else
            #define AE_API
        #endif
 //   #else
  //      #define AE_API
 //   #endif
#else 
    #error AnEngine only supports Windows and Linux!
#endif

#if defined(AE_DEBUG_FLAG) and not defined(AE_ENABLE_ASSERTS)
    #define AE_ENABLE_ASSERTS
#endif

#if defined(AE_ENABLE_ASSERTS)
    #define AE_ASSERT(x, ...) { if(!(x)) { AE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } }
    #define AE_CORE_ASSERT(x, ...) { if(!(x)) { AE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } }
#else
    #define AE_ASSERT(x, ...)
    #define AE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#endif
