#pragma once

#include <unordered_map>
#include <memory>

#include <api.hpp>
#include <core/base_component.hpp>

namespace engine::core {
    struct ENGINE_API Entity {
        template<typename T>
        std::shared_ptr<T> get_component() {
            auto it = components.find(typeid(T).hash_code());

            if (it != components.end()) {
                return std::static_pointer_cast<T>(it->second);
            }

            return nullptr;
        }

        template<typename T, typename... Args>
        std::shared_ptr<T> add_component(Args &&... args) {
            size_t type_id = typeid(T).hash_code();

            if (components.find(type_id) == components.end()) {
                auto component = std::make_shared<T>(std::forward<Args>(args)...);
                components[type_id] = component;
                return component;
            }

            return nullptr;
        }

        template<typename T>
        void remove_component() {
            auto it = components.find(typeid(T).hash_code());

            if (it != components.end()) {
                components.erase(it);
            }
        }

        void update(graphics::FrameRenderContextRef ctx) {
            for (auto& component : components) {
                component.second->update(ctx);
            }
        }

        void render(graphics::FrameRenderContextRef ctx) {
            for (auto& component : components) {
                component.second->render(ctx);
            }
        }

        void post_render(graphics::FrameRenderContextRef ctx) {
            for (auto& component : components) {
                component.second->post_render(ctx);
            }
        }
    private:
        std::unordered_map<size_t, BaseComponentSPtr> components{};
    };

    using EntitySPtr = std::shared_ptr<Entity>;
}