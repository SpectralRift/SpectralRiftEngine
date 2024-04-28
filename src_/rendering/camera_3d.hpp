#pragma once

#include <api.hpp>
#include <core/base_component.hpp>
#include <graphics/rendering/shader_program.hpp>
#include <rendering/camera.hpp>

#include <utils/vector2.hpp>
#include <utils/vector3.hpp>

namespace engine::rendering {
    struct ENGINE_API Camera3D : public Camera {
        void update();

        float fov{45.f};
        float near_clip{0.1f};
        float far_clip{2000.0f};

        engine::utils::FVector3 position{0.f, 0.f, 0.f};

        // yaw, pitch
        engine::utils::FVector2 rotation{-90.f, 0.f};

        utils::IVector2 viewport{1280, 720};
    };
}