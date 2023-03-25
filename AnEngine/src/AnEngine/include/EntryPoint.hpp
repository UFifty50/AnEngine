#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP

#include "Application.hpp"


extern AnEngine::Application* AnEngine::CreateApplication();

int main(int argc, char** argv) {
    AnEngine::Log::init();
    AE_CORE_DEBUG("Core debug");
    AE_ERROR("Client error");

    auto app = AnEngine::CreateApplication();
    app->Run();
    delete app;
}

#endif
