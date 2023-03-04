#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "Application.hpp"

#ifdef AE_WIN

extern AnEngine::Application* AnEngine::CreateApplication();

int main(int argc, char** argv) {
    auto app = AnEngine::CreateApplication();
    app->Run();
    delete app;
}

#endif

#endif
