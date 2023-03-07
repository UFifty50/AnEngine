#ifndef AEPCH_HPP
#define AEPCH_HPP


#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include <algorithm>

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#ifdef AE_WIN
    #include <Windows.h>
#endif


#include "Log.hpp"
#include "Core.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

#endif