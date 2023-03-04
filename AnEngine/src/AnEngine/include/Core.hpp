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

#endif
