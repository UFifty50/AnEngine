#include "aepch.hpp"

#include "Platform/Linux/window/LinuxWindow.hpp"

#include <GLFW/glfw3.h>

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Platform/OpenGL/OpenGLContext.hpp"


namespace AnEngine {
    static bool GLFWinitialised = false;
    float Window::HighDPIScaleFactor = 1.0f;

    static void GLFWerrorCallback(int error, const char* description) {
        AE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::create(const WindowProperties& props) {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProperties& props) { init(props); }

    LinuxWindow::~LinuxWindow() { shutdown(); }

    void LinuxWindow::init(const WindowProperties& props) {
        AE_PROFILE_FUNCTION()

        data.title = props.title;
        data.width = props.width;
        data.height = props.height;

        AE_CORE_INFO("Creating window {0} (width {1}; height {2})", props.title,
                     props.width, props.height);


        if (!GLFWinitialised) {
            int success = glfwInit();
            AE_CORE_ASSERT(success, "Could not initialise GLFW!");
            glfwSetErrorCallback(GLFWerrorCallback);
            GLFWinitialised = true;
        }

        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        float xscale, yscale;
        glfwGetMonitorContentScale(primaryMonitor, &xscale, &yscale);

        if (xscale > 1.0f || yscale > 1.0f) {
            HighDPIScaleFactor = xscale;
            glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        }

        window = glfwCreateWindow((int)props.width, (int)props.height,
                                  props.title.c_str(), nullptr, nullptr);

        graphicsContext = std::make_shared<OpenGLContext>(window);
        graphicsContext->init();

        glfwSetWindowUserPointer(window, &data);
        setVSync(true);

        // GLFW callbacks
        glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* win) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
            WindowCloseEvent event;
            data.eventCallback(event);
        });

        glfwSetKeyCallback(
            window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent event(key, 0);
                        data.eventCallback(event);
                        break;
                    }

                    case GLFW_RELEASE: {
                        KeyReleasedEvent event(key);
                        data.eventCallback(event);
                        break;
                    }

                    case GLFW_REPEAT: {
                        KeyPressedEvent event(key, 1);
                        data.eventCallback(event);
                        break;
                    }
                }
            });

        glfwSetCharCallback(window, [](GLFWwindow* win, unsigned int keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
            KeyTypedEvent event(keycode);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(
            window, [](GLFWwindow* win, int button, int action, int mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

                switch (action) {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent event(button);
                        data.eventCallback(event);
                        break;
                    }

                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent event(button);
                        data.eventCallback(event);
                        break;
                    }
                }
            });

        glfwSetScrollCallback(
            window, [](GLFWwindow* win, double Xoffset, double Yoffset) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

                MouseScrolledEvent event((float)Xoffset, (float)Yoffset);
                data.eventCallback(event);
            });

        glfwSetCursorPosCallback(window, [](GLFWwindow* win, double Xpos, double Ypos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

            MouseMovedEvent event((float)Xpos, (float)Ypos);
            data.eventCallback(event);
        });
    }

    void LinuxWindow::shutdown() { glfwDestroyWindow(window); }

    void LinuxWindow::onUpdate() {
        glfwPollEvents();
        graphicsContext->swapBuffers();
    }

    void LinuxWindow::setVSync(bool isEnabled) {
        isEnabled ? glfwSwapInterval(1) : glfwSwapInterval(0);

        data.vSync = isEnabled;
    }

    bool LinuxWindow::VSyncEnabled() const { return data.vSync; }
}  // namespace AnEngine
