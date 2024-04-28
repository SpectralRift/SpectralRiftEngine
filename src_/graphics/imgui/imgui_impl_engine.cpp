// dear imgui: Renderer Backend for the SpectralRift engine
// selects the according implementation

#include <graphics/imgui/imgui_impl_engine.hpp>
#include <graphics/imgui/imgui_engine.hpp>
#include <graphics/imgui/imgui_impl_engine_window.hpp>

// #define USE_IMGUI_IMPL

#if defined(ENGINE_BACKEND_OPENGL) || defined(ENGINE_BACKEND_OPENGL_ES)

#if defined(USE_IMGUI_IMPL)

#include <graphics/imgui/imgui_impl_opengl3.h>

#else
#include <graphics/imgui/imgui_impl_engine_gl3.hpp>
#endif

#endif

namespace engine::graphics::imgui {
    bool ImGui_ImplEngine_Init() {
#if defined(ENGINE_BACKEND_OPENGL) || defined(ENGINE_BACKEND_OPENGL_ES)
#if defined(USE_IMGUI_IMPL)
        return ImGui_ImplOpenGL3_Init();
#else
        return ImGui_ImplEngineGL3_Init();
#endif
#endif
    }

    void ImGui_ImplEngine_Shutdown() {
#if defined(ENGINE_BACKEND_OPENGL) || defined(ENGINE_BACKEND_OPENGL_ES)
#if defined(USE_IMGUI_IMPL)
        ImGui_ImplOpenGL3_Shutdown();
#else
        ImGui_ImplEngineGL3_Shutdown();
#endif
#endif
    }

    void ImGui_ImplEngine_NewFrame() {
#if defined(ENGINE_BACKEND_OPENGL) || defined(ENGINE_BACKEND_OPENGL_ES)
#if defined(USE_IMGUI_IMPL)
        ImGui_ImplOpenGL3_NewFrame();
#else
        ImGui_ImplEngineGL3_NewFrame();
#endif
#endif
    }

    void ImGui_ImplEngine_Render() {
#if defined(ENGINE_BACKEND_OPENGL) || defined(ENGINE_BACKEND_OPENGL_ES)
#if defined(USE_IMGUI_IMPL)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
        ImGui_ImplEngineGL3_Render();
#endif
#endif
    }
}