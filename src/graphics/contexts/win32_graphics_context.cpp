#ifdef ENGINE_TARGET_WIN32
#include <system_error>

#include <utils/macros/debug.hpp>
#include <graphics/contexts/win32_graphics_context.hpp>

#pragma comment (lib, "opengl32.lib")

namespace engine::graphics {
	bool Win32GraphicsContext::init(windows::BaseAppWindowSPtr window) {
		ASSERT_MSG(window, "window is NULL");
		ASSERT_MSG(window->get_device_ctx(), "the window's device context is NULL");

		window_device_ctx_handle = (HDC)window->get_device_ctx();

		// we build our pixel format descriptor now
		PIXELFORMATDESCRIPTOR pfd{};

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;

		// we let Windows to choose the pixel format for this
		int pixel_format = ChoosePixelFormat(window_device_ctx_handle, &pfd);
		if (!SetPixelFormat(window_device_ctx_handle, pixel_format, &pfd)) {
			DEBUG_MSG("SetPixelFormat failed: %s\n", std::system_category().message(GetLastError()).c_str());
			return false;
		}

		gl_ctx = wglCreateContext(window_device_ctx_handle);
		if (!gl_ctx) {
			DEBUG_MSG("wglCreateContext failed: %s\n", std::system_category().message(GetLastError()).c_str());
			destroy();
			return false;
		}

		return true;
	}

	void Win32GraphicsContext::use() {
		if (window_device_ctx_handle && gl_ctx) {
			wglMakeCurrent(window_device_ctx_handle, gl_ctx);
		}
	}

	void Win32GraphicsContext::discard() {
		wglMakeCurrent(NULL, NULL);
	}

	void Win32GraphicsContext::present() {
		if (window_device_ctx_handle) {
			wglSwapLayerBuffers(window_device_ctx_handle, WGL_SWAP_MAIN_PLANE);
		}
	}

	void Win32GraphicsContext::destroy() {
		if (gl_ctx) {
			wglDeleteContext(gl_ctx);
			gl_ctx = NULL;
		}

		window_device_ctx_handle = NULL;
	}

	uintptr_t Win32GraphicsContext::get_proc_func(std::string_view name) {
		auto func = (uintptr_t)wglGetProcAddress(name.data());

		if (!func) {
            // ToDo: modify to use platform-dependent code.
			auto gl_lib = LoadLibraryA("opengl32.dll");
			ASSERT_MSG(gl_lib, "Could not load OpenGL DLL!");

			func = (uintptr_t)GetProcAddress(gl_lib, name.data());
		}

		return (uintptr_t)func;
	}
}
#endif