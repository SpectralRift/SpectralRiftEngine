#ifdef ENGINE_TARGET_WIN32
#include <system_error>

#include <utils/macros/debug.hpp>
#include <graphics/contexts/win32_graphics_context.hpp>

#include <glad/gl.h>

namespace engine::graphics {
	bool Win32GraphicsContext::init() {
		ASSERT_MSG(window_handle != nullptr, "window_handle is NULL");

        wnd_dev_ctx = GetDC(window_handle);

		// we build our pixel format descriptor now
		PIXELFORMATDESCRIPTOR pfd{};

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;

		// we let Windows to choose the pixel format for this
		int pixel_format = ChoosePixelFormat(wnd_dev_ctx, &pfd);
		if (!SetPixelFormat(wnd_dev_ctx, pixel_format, &pfd)) {
			DEBUG_MSG("SetPixelFormat failed: %s\n", std::system_category().message(GetLastError()).c_str());
			return false;
		}

		gl_ctx = wglCreateContext(wnd_dev_ctx);
		if (!gl_ctx) {
			DEBUG_MSG("wglCreateContext failed: %s\n", std::system_category().message(GetLastError()).c_str());
			destroy();
			return false;
		}

        // use the context
        use();

        // load OpenGL Core
        int ret = gladLoadGL((GLADloadfunc) win32_gl_loader);

        if (!ret) {
            DEBUG_MSG("Failed to initialize OpenGL through glad!\n");
            return false;
        }

		return true;
	}

	void Win32GraphicsContext::use() {
		if (wnd_dev_ctx != nullptr && gl_ctx) {
			wglMakeCurrent(wnd_dev_ctx, gl_ctx);
		}
	}

	void Win32GraphicsContext::discard() {
		wglMakeCurrent(nullptr, nullptr);
	}

	void Win32GraphicsContext::present() {
		if (wnd_dev_ctx != nullptr) {
			wglSwapLayerBuffers(wnd_dev_ctx, WGL_SWAP_MAIN_PLANE);
		}
	}

	void Win32GraphicsContext::destroy() {
		if (gl_ctx) {
			wglDeleteContext(gl_ctx);
			gl_ctx = nullptr;
		}

        if (wnd_dev_ctx == nullptr) {
            ReleaseDC(window_handle, wnd_dev_ctx);
            wnd_dev_ctx = nullptr;
        }
	}

	uintptr_t Win32GraphicsContext::win32_gl_loader(std::string_view name) {
		auto func = (uintptr_t)wglGetProcAddress(name.data());

		if (!func) {
			auto gl_lib = LoadLibraryA("opengl32.dll");
			ASSERT_MSG(gl_lib, "Could not load OpenGL DLL!");

			func = (uintptr_t)GetProcAddress(gl_lib, name.data());
		}

		return (uintptr_t)func;
	}
}
#endif