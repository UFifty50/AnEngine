#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "Application.hpp"


extern AnEngine::Application* AnEngine::CreateApplication();

int main(int argc, char** argv) {
    AnEngine::Log::init();

    auto app = AnEngine::CreateApplication();
    app->Run();
    delete app;
}

#endif
