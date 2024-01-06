#pragma once
#include <api.hpp>
#include <cstdint>
#include <string_view>

// Platform-specific conditional imports begin here.
#if defined(ENGINE_TARGET_WIN32)
#include <platform/win32/win32_platform.hpp>
#elif defined(ENGINE_TARGET_LINUX) || defined(ENGINE_TARGET_ANDROID)
#include <platform/linux/linux_platform.hpp>
#endif
// Platform-specific conditional imports end here.

namespace engine::platform {
	void ENGINE_API init();
    uintptr_t ENGINE_API load_library(std::string_view lib_name);
    uintptr_t ENGINE_API get_library_func(uintptr_t lib_handle, std::string_view func);
}