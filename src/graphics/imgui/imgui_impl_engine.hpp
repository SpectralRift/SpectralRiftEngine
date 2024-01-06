#pragma once

#include <api.hpp>
#include <utils/version.hpp>

#include <imgui.h>

namespace engine::graphics::imgui {
    ENGINE_API bool ImGui_ImplEngine_Init();

    ENGINE_API void ImGui_ImplEngine_Shutdown();

    ENGINE_API void ImGui_ImplEngine_NewFrame();

    ENGINE_API void ImGui_ImplEngine_Render();
}