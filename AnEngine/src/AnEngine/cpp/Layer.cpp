#include "aepch.hpp"
#include "Layer.hpp"


namespace AnEngine {
    Layer::Layer(const std::string& name) : debugName(name) {}

    Layer::~Layer() = default;
}
