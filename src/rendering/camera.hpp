#pragma once

#include <api.hpp>
#include <utils/vector2.hpp>

#include <glm/ext/matrix_float4x4.hpp>

namespace engine::rendering {
    struct Camera {
        [[nodiscard]] const glm::mat4 &get_projection() const { return m_projection; }
        [[nodiscard]] const glm::mat4 &get_view() const { return m_view; }

        virtual void update() = 0;
    protected:
        glm::mat4 m_projection{1.0f};
        glm::mat4 m_view{1.0f};
    };
}