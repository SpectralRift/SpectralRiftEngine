#pragma once

#include <core/base_component.hpp>

namespace engine::components {
    struct ENGINE_API ImGuiComponent : core::BaseComponent {
        /// <summary>
        /// Event delegate for the OnImGuiRender() event.
        /// </summary>
        using ImGuiRenderEventDelegate = utils::Event<>;

        const std::string get_name() override {
            return "ImGuiComponent";
        }

        void create() override;
        void destroy() override;

        void render(graphics::FrameRenderContextRef ctx) override;

        /// <summary>
        /// Add a listener to the OnImGuiRender event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_imgui_render_listener(const ImGuiRenderEventDelegate::Listener& listener) {
            ev_imgui_render.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the OnImGuiRender event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_imgui_render_listener(const ImGuiRenderEventDelegate::ListenerID& listener) {
            ev_imgui_render.remove_listener(listener);
        }
    private:
        ImGuiRenderEventDelegate ev_imgui_render;
    };
}