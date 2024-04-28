#include <components/imgui_component.hpp>

#include <graphics/imgui/imgui_impl_engine.hpp>
#include <graphics/imgui/imgui_engine.hpp>
#include <graphics/imgui/imgui_impl_engine_window.hpp>

#include <imgui.h>

// make sure that ImGuiContext is fully declared by importing the internal header as well
#include <imgui_internal.h>

namespace engine::components {
    static bool is_imgui_init = false;

    void ImGuiComponent::create() {
        if(!is_imgui_init) {


            is_imgui_init = true;
        }
    }

    void ImGuiComponent::destroy() {
        if(is_imgui_init) {
            engine::graphics::imgui::ImGui_ImplEngine_Shutdown();
            engine::graphics::imgui::ImGui_ImplEngineWindow_Shutdown();
            ImGui::DestroyContext();

            is_imgui_init = false;
        }
    }

    void ImGuiComponent::render(graphics::FrameRenderContextRef ctx) {
        auto imgui_ctx = ctx.get().window->user_data_get<ImGuiContext>("ImGuiContext");

        // if our imgui context is not set, we create it
        if(imgui_ctx == nullptr) {
            imgui_ctx = std::shared_ptr<ImGuiContext>(ImGui::CreateContext());

            ctx.get().window->user_data_put<ImGuiContext>("ImGuiContext", imgui_ctx);
            ImGui::SetCurrentContext(imgui_ctx.get());

            ImGui::StyleColorsDark();

            engine::graphics::imgui::ImGui_ImplEngine_Init();
            engine::graphics::imgui::ImGui_ImplEngineWindow_Init(ctx.get().window);
        } else {
            ImGui::SetCurrentContext(imgui_ctx.get());
        }

        engine::graphics::imgui::ImGui_ImplEngine_NewFrame();
        engine::graphics::imgui::ImGui_ImplEngineWindow_NewFrame(ctx.get().frame_delta);
        ImGui::NewFrame();
        {
            ImGui::ShowDemoWindow();
            ev_imgui_render.invoke();
        }
        ImGui::Render();
        engine::graphics::imgui::ImGui_ImplEngine_Render();
    }
}