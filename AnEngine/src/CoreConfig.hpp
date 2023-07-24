#ifndef CORECONFIG_HPP
#define CORECONFIG_HPP


#define USE_FMT 0

#if defined(AE_DEBUG_FLAG)  // Debug
    #define AE_PROFILING 0
    #define AE_ENABLE_ASSERTS

#elif defined(AE_RELEASE)  // Release
    #define AE_PROFILING 1
    #define AE_ENABLE_ASSERTS

#elif defined(AE_DIST)  // Dist
    #define AE_PROFILING 0

#endif

#endif
