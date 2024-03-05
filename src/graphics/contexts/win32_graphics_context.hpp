#ifdef ENGINE_TARGET_WIN32
#pragma once

#include <api.hpp>
#include <graphics/interfaces/i_graphics_context.hpp>

#include <Windows.h>

namespace engine::graphics {
    struct ENGINE_API Win32GraphicsContext : public IGraphicsContext {
        explicit Win32GraphicsContext(HWND _window_handle) : window_handle(_window_handle), gl_ctx(nullptr),
                                                             wnd_dev_ctx(nullptr) {

        }

        bool init();

        void use();

        void present();

        void discard();

        void destroy();

    private:
        static uintptr_t win32_gl_loader(std::string_view name);

        HGLRC gl_ctx;
        HWND window_handle;
        HDC wnd_dev_ctx;
    };
}
#endif