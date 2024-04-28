#pragma once

#include <factories/app_window_factory.hpp>
#include <windows/win32_app_window.hpp>

#include <cstdint>
#include <string_view>

namespace engine::platform {
	inline void init_win32() {
		// register app window factories
		factories::app_window::register_type("Win32AppWindow", []()
			{
				return std::make_shared<windows::Win32AppWindow>();
			}
		);
	}

    uintptr_t ENGINE_API load_library_win32(std::string_view lib_name);
    uintptr_t ENGINE_API get_library_func_win32(uintptr_t lib_handle, std::string_view func);
}