#if defined(ENGINE_FEATURE_EGL)
#pragma once

#include <api.hpp>
#include <graphics/i_graphics_context.hpp>

#include <string_view>

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace engine::graphics {
    struct ENGINE_API EGLGraphicsContext : public IGraphicsContext {
        EGLGraphicsContext(EGLNativeWindowType _window) : window(_window) {

        }

        bool init();

        void use();

        void present();

        void discard();

        void destroy();

        uintptr_t get_proc_func(std::string_view name);

    private:
        EGLDisplay display{nullptr};
        EGLSurface surface{nullptr};
        EGLContext context{nullptr};
        EGLNativeWindowType window;

        static std::string_view egl_get_error_string(int error);
    };
}
#endif