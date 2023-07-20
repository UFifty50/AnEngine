#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "Application.hpp"


extern AnEngine::Application* AnEngine::CreateApplication();

#if defined(AE_PLATFORM_WINDOWS) && defined(AE_DIST)
    #include <Windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return AnEngine::main(__argc, __argv);
}
#endif

int main(int argc, char** argv) { return AnEngine::main(argc, argv); }

#endif
