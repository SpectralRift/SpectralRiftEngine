#pragma once

#include <factories/app_window_factory.hpp>
#ifndef ENGINE_TARGET_ANDROID
#include <windows/x11_app_window.hpp>
#endif

#include <cstdint>
#include <string_view>

namespace engine::platform {
	inline void init_unix() {
		// register app window factories
#ifndef ENGINE_TARGET_ANDROID
		factories::app_window::register_type("X11AppWindow", []()
			{
				return std::make_shared<windows::X11AppWindow>();
			}
		);
#endif
	}

    uintptr_t ENGINE_API load_library_unix(std::string_view lib_name);
    uintptr_t ENGINE_API get_library_func_unix(uintptr_t lib_handle, std::string_view func);
}
