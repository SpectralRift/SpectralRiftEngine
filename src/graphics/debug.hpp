#pragma once

#include <api.hpp>

namespace engine::graphics::debug {
    ENGINE_API inline int vbo_count;
    ENGINE_API inline int vao_count;

    inline void init() {
        vbo_count = 0;
        vao_count = 0;
    }
}