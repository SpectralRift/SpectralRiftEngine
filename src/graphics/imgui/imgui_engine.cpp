#include <graphics/imgui/imgui_engine.hpp>

namespace engine::graphics::imgui {
    static ImGuiContext* imgui_ctx = 0;

    ImGuiContext* get_context() {
        if(imgui_ctx == 0) {
            imgui_ctx = ImGui::CreateContext();
        }

        return imgui_ctx;
    }
}