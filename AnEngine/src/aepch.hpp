#ifndef AEPCH_HPP
#define AEPCH_HPP

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#if defined(AE_WIN)
    #include <Shlwapi.h>
    #include <Windows.h>
    #include <commdlg.h>
    #include <io.h>

    #define NOMINMAX
    #undef near
    #undef far
#elif defined(AE_LINUX)
    #include <libgen.h>
    #include <limits.h>
    #include <signal.h>
    #include <unistd.h>
#endif

#endif
