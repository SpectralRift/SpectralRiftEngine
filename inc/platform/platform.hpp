#pragma once
#include <api.hpp>

// Platform-specific conditional imports begin here.
#ifdef ENGINE_TARGET_WIN32
#include <platform/win32/win32_platform.hpp>
#endif
// Platform-specific conditional imports end here.

namespace engine::platform {
	void ENGINE_API init();

	namespace graphics {
		uintptr_t get_gl_func(const char* name);
	}
}