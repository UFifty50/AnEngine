#ifndef RENDERAPI_HPP
#define RENDERAPI_HPP

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>
#include <ostream>

#include "Renderer/VertexArray.hpp"


namespace AnEngine {
    class RenderAPI {
    public:
        enum API : uint8_t { NoAPI = 0, OpenGL, DirectX11, DirectX12, Vulkan };

        inline static API getAPI() { return currAPI; }
        inline static void setAPI(RenderAPI::API api) { currAPI = api; }
        static std::string getAPIName() {
            switch (currAPI) {
                case NoAPI:
                    return "NoAPI";
                case OpenGL:
                    return "OpenGL";
                case DirectX11:
                    return "DirectX11";
                case DirectX12:
                    return "DirectX12";
                case Vulkan:
                    return "Vulkan";
                default:
                    return "Unknown API";
            }
        }

        //    static Ref<RenderAPI> create();
        virtual void init() = 0;
        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) = 0;

        virtual void clearColour(const glm::vec4& colour) = 0;
        virtual void clear() = 0;
        virtual void drawIndexed(const Ref<VertexArray>& vertexArray,
                                 uint32_t indexCount = 0) = 0;

    private:
        static RenderAPI::API currAPI;
    };

    std::ostream& operator<<(std::ostream& os, const RenderAPI::API& api);
}  // namespace AnEngine

#endif
