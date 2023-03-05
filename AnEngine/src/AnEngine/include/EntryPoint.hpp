#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP


#ifdef AE_WIN

extern AnEngine::Application* AnEngine::CreateApplication();

int main(int argc, char** argv) {
    AnEngine::Log::init();
    AE_CORE_DEBUG("Core debug");
    AE_ERROR("Client error");

    auto app = AnEngine::CreateApplication();
    app->Run();
    while (true);
    delete app;
}

#endif

#endif
