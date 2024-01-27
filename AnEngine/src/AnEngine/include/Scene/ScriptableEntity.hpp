#ifndef SCRIPTABLEENTITY_HPP
#define SCRIPTABLEENTITY_HPP

#include "Scene/Entity.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class ScriptableEntity {
    public:
        virtual ~ScriptableEntity() {}

        template <typename T>
        bool hasComponent() {
            return entity.hasComponent<T>();
        }

        template <typename T>
        T& getComponent() {
            return entity.getComponent<T>();
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args) {
            return entity.addComponent<T>(std::forward<Args>(args)...);
        }

        template <typename... Ts>
        void removeComponent() {
            entity.removeComponent<Ts...>();
        }

    protected:
        virtual void onCreate() {}
        virtual void onDestroy() {}
        virtual void onUpdate(TimeStep deltaTime) {}

    private:
        Entity entity;
        friend class Scene2D;
        friend class Scene3D;
    };
};  // namespace AnEngine

#endif
