#ifndef RENDEROBJECT_HPP
#define RENDEROBJECT_HPP

#include <exception>
#include "Renderer/VertexBuffer.hpp"
#include "Renderer/IndexBuffer.hpp"


namespace AnEngine {
    class RenderObject {
        RenderObject(VertexBuffer vBuf, IndexBuffer iBuf) {
            throw std::exception("Not implemented yet");
        }
    };
}

#endif
