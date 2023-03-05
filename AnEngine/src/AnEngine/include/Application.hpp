#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include "Core.hpp"
#include "Events/Event.hpp"

namespace AnEngine {
    class AE_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // user defined
    Application* CreateApplication();
}

#endif
