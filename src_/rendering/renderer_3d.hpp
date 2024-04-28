#pragma once

#include <api.hpp>

#include <rendering/camera.hpp>

namespace engine::rendering {
    bool ENGINE_API renderer_3d_init();
    void ENGINE_API renderer_3d_setup_camera(const rendering::Camera& camera);
    void ENGINE_API renderer_3d_setup_model_mtx(const glm::mat4& model_mtx);
    void ENGINE_API renderer_3d_shutdown();
}