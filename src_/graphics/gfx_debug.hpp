#pragma once

#include <memory>

#include <api.hpp>

namespace engine::graphics::debug {
    struct ENGINE_API GraphicsDebugData {
        GraphicsDebugData() : vbo_count(0), vao_count(0), tex_count(0), draw_count(0) {}

        void reset() {
            draw_count = 0;
        }

        int vbo_count;
        int vao_count;
        int tex_count;
        int draw_count;
    };

    std::shared_ptr<GraphicsDebugData> ENGINE_API get_graphics_debug();
}