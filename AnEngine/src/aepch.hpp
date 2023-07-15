#ifndef AEPCH_HPP
#define AEPCH_HPP


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
    #include <io.h>
#elif defined(AE_LINUX)
    #include <libgen.h>
    #include <limits.h>
    #include <unistd.h>
#endif

#include "Core.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Log.hpp"

#endif
