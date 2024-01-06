#include <graphics/imgui/imgui_impl_engine_window.hpp>

namespace engine::graphics::imgui {
    bool ImGui_ImplEngineWindow_Init() {
        return true;
    }

    void ImGui_ImplEngineWindow_Shutdown() {}

    void ImGui_ImplEngineWindow_NewFrame(windows::BaseAppWindowSPtr appWindow) {
        ImGuiIO& io = ImGui::GetIO();
        auto size = appWindow->get_size();

        io.DisplaySize = ImVec2((float) size.x, (float) size.y);
        io.DeltaTime = (float) (1.f / 60.f);
    }
}