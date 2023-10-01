#pragma once
#include <api.hpp>

// Platform-specific conditional imports begin here.
#ifdef ENGINE_TARGET_WIN32
#include <platform/win32/platform.hpp>
#endif
// Platform-specific conditional imports end here.

namespace engine::platform {
	inline void ENGINE_API init();
}