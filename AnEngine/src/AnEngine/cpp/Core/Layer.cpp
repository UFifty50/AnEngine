#include "aepch.hpp"

#include "Core/Layer.hpp"


namespace AnEngine {
    Layer::Layer(const std::string& name) : debugName(name) {}

    Layer::~Layer() = default;
}  // namespace AnEngine
