#include "aepch.hpp"

#include <string>

#include "Core/Utils/PlatformUtils.hpp"


namespace AnEngine {
    static std::string OpenFileDialogue(const char* filter) {
        static_assert(false, "Linux file dialogues not supported yet.");
    }

    static std::string SaveFileDialogue(const char* filter) {
        static_assert(false, "Linux file dialogues not supported yet.");
    }
}  // namespace AnEngine
