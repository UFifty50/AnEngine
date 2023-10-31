#ifndef AEPCH_HPP
#define AEPCH_HPP

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
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
#elif defined(AE_LINUX)
    #include <libgen.h>
    #include <limits.h>
    #include <signal.h>
    #include <unistd.h>
#endif

#include "Core/Core.hpp"
#include "Core/Log.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Profiling/InstrumentationTimer.hpp"
#include "Profiling/Instrumentor.hpp"

#endif
