#include <platform/platform.hpp>

namespace engine::platform {
	void init() {
		// Platform-specific conditional initialization begins here.
#ifdef ENGINE_TARGET_WIN32
		win32::init();
#endif
		// Platform-specific conditional initialization ends here.
	}
}