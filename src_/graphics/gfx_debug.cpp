#include <graphics/gfx_debug.hpp>

namespace engine::graphics::debug {
    static std::shared_ptr<GraphicsDebugData> g_gfx_dbg{nullptr};

    std::shared_ptr<GraphicsDebugData> get_graphics_debug() {
        if(g_gfx_dbg == nullptr) {
            g_gfx_dbg = std::make_shared<GraphicsDebugData>();
        }

        return g_gfx_dbg;
    }
}