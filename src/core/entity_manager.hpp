#pragma once

#include <vector>

#include <core/entity.hpp>
#include <core/i_renderable.hpp>

namespace engine::core {
    struct EntityManager : public IRenderable {
        template <typename T, typename std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
        std::shared_ptr<T> create() {
            auto entity = std::make_shared<T>();
            entities.push_back(entity);
            return entity;
        }

        template <typename T, typename std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
        void destroy(std::shared_ptr<T> entity) {
            auto it = std::find_if(entities.begin(), entities.end(),
                                   [entity](const auto &ptr) { return ptr == entity; });

            if (it != entities.end()) {
                entities.erase(it);
            }
        }

        void update(graphics::FrameRenderContextRef ctx) {
            for (auto &entity: entities) {
                entity->update(ctx);
            }
        }

        void render(graphics::FrameRenderContextRef ctx) {
            for (auto &entity: entities) {
                entity->render(ctx);
            }
        }

        void post_render(graphics::FrameRenderContextRef ctx) {
            for (auto &entity: entities) {
                entity->post_render(ctx);
            }
        }

    private:
        std::vector<EntitySPtr> entities;
    };
}