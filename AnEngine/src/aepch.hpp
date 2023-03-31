#ifndef AEPCH_HPP
#define AEPCH_HPP


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include <algorithm>
#include <ranges>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#if defined(AE_WIN)
    #include <Windows.h>
    #include <Shlwapi.h>
    #include <io.h> 
#elif defined(AE_LINUX)
    #include <limits.h>
    #include <libgen.h>
    #include <unistd.h>
#endif

#include "Log.hpp"
#include "Core.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

#endif
