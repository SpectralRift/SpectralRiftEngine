#pragma once

#include <api.hpp>
#include <utils/version.hpp>

#include <imgui.h>

namespace engine::graphics::imgui {
    ENGINE_API bool ImGui_ImplEngineGL3_Init();

    ENGINE_API void ImGui_ImplEngineGL3_Shutdown();

    ENGINE_API void ImGui_ImplEngineGL3_NewFrame();

    ENGINE_API void ImGui_ImplEngineGL3_Render();
}