#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "Application.hpp"


extern AnEngine::Application* AnEngine::CreateApplication();

#if defined(AE_WIN) && defined(AE_DIST)
    #include <Windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return AnEngine::Application::AEmain(__argc, __argv);
}
#endif

int main(int argc, char** argv) { return AnEngine::Application::AEmain(argc, argv); }

#endif
