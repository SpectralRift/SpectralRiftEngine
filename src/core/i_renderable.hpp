#pragma once

#include <graphics/rendering/frame_render_context.hpp>

namespace engine::core {
    struct IRenderable {
        virtual void update(graphics::FrameRenderContextRef ctx) {}

        virtual void render(graphics::FrameRenderContextRef ctx) {};

        virtual void post_render(graphics::FrameRenderContextRef ctx) {}
    };
}