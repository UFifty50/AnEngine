// for use by the end user; not internal to the API

#ifndef ANENGINE_HPP
#define ANENGINE_HPP

// ---virtual base application---
#include "AnEngine/include/Application.hpp"

// ---Rendering---
#include "AnEngine/include/Renderer/Renderer.hpp"
#include "AnEngine/include/Renderer/RenderCommandQueue.hpp"
#include "AnEngine/include/Renderer/RenderAPI.hpp"

#include "AnEngine/include/Renderer/Shader.hpp"
#include "AnEngine/include/Renderer/VertexArray.hpp"

#include "AnEngine/include/Renderer/Buffers/VertexBuffer.hpp"
#include "AnEngine/include/Renderer/Buffers/IndexBuffer.hpp"
#include "AnEngine/include/Renderer/Buffers/BufferLayout.hpp"

// ---Layers---
#include "AnEngine/include/Layer.hpp"
#include "AnEngine/include/ImGui/ImGuiLayer.hpp"

// ---Events---
#include "AnEngine/include/Events/Event.hpp"
#include "AnEngine/include/Events/ApplicationEvent.hpp"
#include "AnEngine/include/Events/KeyEvent.hpp"
#include "AnEngine/include/Events/MouseEvent.hpp"

// ---Input---
#include "AnEngine/include/Input.hpp"

// ---KeyCodes---
#include "AnEngine/include/KeyCodes.hpp"
#include "AnEngine/include/MouseButtonCodes.hpp"
#include "AnEngine/include/JoystickButtonCodes.hpp"
#include "AnEngine/include/GamepadCodes.hpp"

// ---Log---
#include "AnEngine/include/Log.hpp"

// ---Entry Point---
#include "AnEngine/include/EntryPoint.hpp"

#endif