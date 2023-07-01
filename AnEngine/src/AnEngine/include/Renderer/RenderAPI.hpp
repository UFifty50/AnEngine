#ifndef RENDERAPI_HPP
#define RENDERAPI_HPP

#include <cstdint>
#include <ostream>
#include <memory>
#include <glm/glm.hpp>

#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class RenderAPI {
    public:
        enum API : uint8_t {
            NoAPI = 0,
            OpenGL,
            DirectX11,
            DirectX12,
            Vulkan
        };

        inline static API getAPI() { return currAPI; }
        inline static void setAPI(RenderAPI::API api) { currAPI = api; }

        static RenderAPI* create();

        virtual void clearColour(const glm::vec4& colour) = 0;
        virtual void clear() = 0;
        virtual void clearDepth() = 0;
        virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

    private:
        static RenderAPI::API currAPI;
    };

    std::ostream& operator<<(std::ostream& os, const RenderAPI::API& api);
}

#endif
