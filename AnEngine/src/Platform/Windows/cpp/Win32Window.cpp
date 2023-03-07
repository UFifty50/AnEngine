#include "aepch.hpp"
#include "Win32Window.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"


namespace AnEngine {
    static bool GLFWinitialised = false;

    static void GLFWerrorCallback(int error, const char* description) {
        AE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::create(const WindowProperties& props) {
           return new Win32Window(props);
    }

    Win32Window::Win32Window(const WindowProperties& props) {
        init(props);
    }

    Win32Window::~Win32Window() {
        shutdown();
    }

    void Win32Window::init(const WindowProperties& props) {
        data.title = props.title;
        data.width = props.width;
        data.height = props.height;

        AE_CORE_INFO("Creating window {0} (width {1}; height {2})", props.title, props.width, props.height);

        if (!GLFWinitialised) {
            int success = glfwInit();
            AE_CORE_ASSERT(success, "Could not initialise GLFW!");
            glfwSetErrorCallback(GLFWerrorCallback);
            GLFWinitialised = true;
        }

        window = glfwCreateWindow(
            (int)props.width,
            (int)props.height,
            props.title.c_str(),
            nullptr,
            nullptr
        );
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, &data);
        setVSync(true);

        // GLFW callbacks
        glfwSetWindowSizeCallback(
            window,
            [](GLFWwindow* win, int width, int height) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
                data.width = width;
                data.height = height;
                
                WindowResizeEvent event(width, height);
                data.eventCallback(event);
            }
        );

        glfwSetWindowCloseCallback(
            window,
            [](GLFWwindow* win) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
                WindowCloseEvent event;
                data.eventCallback(event);
            }
        );

        glfwSetKeyCallback(
            window,
            [](GLFWwindow* win, int key, int scancode, int action, int mods) {
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
            }
        );

        glfwSetMouseButtonCallback(
            window,
            [](GLFWwindow* win, int button, int action, int mods) {
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
            }
        );

        glfwSetScrollCallback(
            window,
            [](GLFWwindow* win, double Xoffset, double Yoffset) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

                MouseScrolledEvent event((float)Xoffset, (float)Yoffset);
                data.eventCallback(event);
            }
        );

        glfwSetCursorPosCallback(
            window,
            [](GLFWwindow* win, double Xpos, double Ypos) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);

                MouseMovedEvent event((float)Xpos, (float)Ypos);
                data.eventCallback(event);
            }
        );
    }

    void Win32Window::shutdown() {
        glfwDestroyWindow(window);
    }

    void Win32Window::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    void Win32Window::setVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        }
        else {
            glfwSwapInterval(0);
        }
        data.vSync = enabled;
    }

    bool Win32Window::VSyncEnabled() const {
        return data.vSync;
    }
}
