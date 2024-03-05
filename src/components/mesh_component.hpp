#pragma once

#include <api.hpp>
#include <core/base_component.hpp>

#include <graphics/rendering/shader_program.hpp>
#include <graphics/rendering/texture.hpp>
#include <graphics/rendering/vertex_buffer.hpp>

#include <utils/vector2.hpp>

namespace engine::components {
    struct ENGINE_API MeshComponent : core::BaseComponent {
        void render(graphics::FrameRenderContextRef ctx) override;

        const std::string get_name() {
            return "MeshComponent";
        }

        engine::utils::enums::graphics::PrimitiveType primitive_type{engine::utils::enums::graphics::PrimitiveType::TRIANGLES};
        graphics::TexturePtr texture{nullptr};
        graphics::FVertexBuffer3DPtr vertex_buffer{nullptr};
    };
}