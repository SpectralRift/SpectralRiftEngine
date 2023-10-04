#pragma once
#include <graphics/i_graphics_context.hpp>

#include <Windows.h>

#include <api.hpp>

namespace engine::graphics {
	struct ENGINE_API Win32GraphicsContext : public IGraphicsContext {
		bool init(windows::BaseAppWindowSPtr window);
		void use();
		void present();
		void discard();
		void destroy();
	private:
		HGLRC gl_ctx{};
		HDC window_device_ctx_handle{};
	};
}