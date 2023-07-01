#ifndef RENDERER_HPP
#define RENDERER_HPP


namespace AnEngine {
    enum class RenderAPI {
        NoAPI,
        OpenGL,
        DirectX11,
        DirectX12,
        Vulkan
    };

    class Renderer {
    private:
        static RenderAPI currAPI;

    public:
        static RenderAPI getAPI();
        static void setAPI(RenderAPI api);
    };

    std::ostream& operator<<(std::ostream& os, const RenderAPI& api);
}

#endif
