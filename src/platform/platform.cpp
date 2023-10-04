#include <platform/platform.hpp>

namespace engine::platform {
	void init() {
		// Platform-specific conditional initialization begins here.
#ifdef ENGINE_TARGET_WIN32
		win32::init();
#endif
		// Platform-specific conditional initialization ends here.
	}

	namespace graphics {
		uintptr_t get_gl_func(const char* name) {
#ifdef ENGINE_TARGET_WIN32
			return win32::graphics::get_gl_func(name);
#else
			return NULL;
#endif
		}
	}
}