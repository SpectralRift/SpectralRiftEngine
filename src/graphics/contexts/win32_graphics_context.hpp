#pragma once

#include <api.hpp>
#include <graphics/interfaces/i_graphics_context.hpp>

#include <Windows.h>

namespace engine::graphics {
	struct ENGINE_API Win32GraphicsContext : public IGraphicsContext {
		bool init(windows::BaseAppWindowSPtr window);
		void use();
		void present();
		void discard();
		void destroy();
		uintptr_t get_proc_func(std::string_view name);
	private:
		HGLRC gl_ctx{};
		HDC window_device_ctx_handle{};
	};
}
