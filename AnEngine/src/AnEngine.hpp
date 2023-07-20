// for use by the end user; not internal to the API

#ifndef ANENGINE_HPP
#define ANENGINE_HPP

// ---virtual base application---
#include "AnEngine/include/Application.hpp"

// ---Rendering---
#include "AnEngine/include/Renderer/Buffers/BufferLayout.hpp"
#include "AnEngine/include/Renderer/Buffers/IndexBuffer.hpp"
#include "AnEngine/include/Renderer/Buffers/VertexBuffer.hpp"
#include "AnEngine/include/Renderer/RenderAPI.hpp"
#include "AnEngine/include/Renderer/RenderCommandQueue.hpp"
#include "AnEngine/include/Renderer/Renderer.hpp"
#include "AnEngine/include/Renderer/Shader.hpp"
#include "AnEngine/include/Renderer/ShaderUniform.hpp"
#include "AnEngine/include/Renderer/VertexArray.hpp"

// ---Textures---
#include "AnEngine/include/Texture/Texture2D.hpp"

// ---Cameras---
#include "AnEngine/include/Renderer/Camera/CameraController.hpp"

// ---Time---
#include "AnEngine/include/Time/Time.hpp"
#include "AnEngine/include/Time/TimeStep.hpp"

// ---Layers---
#include "AnEngine/include/Core/Layer.hpp"
#include "AnEngine/include/ImGui/ImGuiLayer.hpp"

// ---Events---
#include "AnEngine/include/Events/ApplicationEvent.hpp"
#include "AnEngine/include/Events/Event.hpp"
#include "AnEngine/include/Events/KeyEvent.hpp"
#include "AnEngine/include/Events/MouseEvent.hpp"

// ---Input---
#include "AnEngine/include/Core/Input.hpp"

// ---KeyCodes---
#include "AnEngine/include/Core/GamepadCodes.hpp"
#include "AnEngine/include/Core/JoystickButtonCodes.hpp"
#include "AnEngine/include/Core/KeyCodes.hpp"
#include "AnEngine/include/Core/MouseButtonCodes.hpp"

// ---Log---
#include "AnEngine/include/Core/Log.hpp"

// ---Entry Point---
#include "AnEngine/include/EntryPoint.hpp"

#endif
