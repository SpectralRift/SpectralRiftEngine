#pragma once

#include <api.hpp>
#include <windows/base_app_window.hpp>

#include <imgui.h>

namespace engine::graphics::imgui {
    ENGINE_API bool ImGui_ImplEngineWindow_Init(const windows::BaseAppWindowSPtr& app_window);

    ENGINE_API void ImGui_ImplEngineWindow_Shutdown();

    ENGINE_API void ImGui_ImplEngineWindow_NewFrame(double delta);
}