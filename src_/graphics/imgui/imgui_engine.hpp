#pragma once

#include <imgui.h>

#include <api.hpp>

namespace engine::graphics::imgui {
    ENGINE_API ImGuiContext* get_context();
}