#include "aepch.hpp"

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <optional>
#include <string>

#include "Application.hpp"
#include "Core/Utils/PlatformUtils.hpp"


namespace AnEngine {
    std::optional<std::string> Dialogues::OpenFileDialogue(const char* filter) {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = {0};  // if using TCHAR macros
        CHAR currentDir[256] = {0};

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner =
            glfwGetWin32Window((GLFWwindow*)Application::getWindow().getNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

        return std::nullopt;
    }

    std::optional<std::string> Dialogues::SaveFileDialogue(const char* filter) {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = {0};  // if using TCHAR macros
        CHAR currentDir[256] = {0};

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner =
            glfwGetWin32Window((GLFWwindow*)Application::getWindow().getNativeWindow());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.lpstrDefExt = std::strchr(filter, '\0') + 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        if (GetSaveFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

        return std::nullopt;
    }
}  // namespace AnEngine
