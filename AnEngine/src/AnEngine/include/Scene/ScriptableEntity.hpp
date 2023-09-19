#ifndef SCRIPTABLEENTITY_HPP
#define SCRIPTABLEENTITY_HPP

#include "Scene/Entity.hpp"


namespace AnEngine {
    class ScriptableEntity {
    public:
        template <typename T>
        T& getComponent() {
            return entity.getComponent<T>();
        }

    private:
        Entity entity;
        friend class Scene;
    };
};  // namespace AnEngine

#endif
