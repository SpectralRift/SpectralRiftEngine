#pragma once

#include <windows/base_app_window.hpp>

namespace engine::graphics {
    struct FrameRenderContext {
        double frame_delta;
        windows::BaseAppWindowSPtr window;
    };

    using FrameRenderContextRef = std::reference_wrapper<FrameRenderContext>;
}