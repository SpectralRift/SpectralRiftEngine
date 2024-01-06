#if defined(ENGINE_FEATURE_EGL)
#pragma once

#include <api.hpp>
#include <graphics/interfaces/i_graphics_context.hpp>

#include <glad/egl.h>

#include <string_view>

namespace engine::graphics
{
	struct ENGINE_API EGLGraphicsContext : public IGraphicsContext
	{
		bool init(windows::BaseAppWindowSPtr window);
		void use();
		void present();
		void discard();
		void destroy();
		uintptr_t get_proc_func(std::string_view name);

	private:
		EGLDisplay display{nullptr};
		EGLSurface surface{nullptr};
		EGLContext context{nullptr};
		static std::string_view egl_get_error_string(int error);
        static uintptr_t egl_loader(std::string_view name);
	};
}
#endif