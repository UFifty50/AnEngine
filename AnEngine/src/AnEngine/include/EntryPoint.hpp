#ifndef ENTRYPOINT_HPP
#define ENTRYPOINT_HPP


#ifdef AE_WIN

extern AnEngine::Application* AnEngine::CreateApplication();

int main(int argc, char** argv) {
    AnEngine::Log::init();
    AE_CORE_DEBUG("This is a debug message");
    AE_CORE_INFO("This is an info message");
    AE_CRITICAL("This is a critical message");

    auto app = AnEngine::CreateApplication();
    app->Run();
    delete app;
}

#endif

#endif
