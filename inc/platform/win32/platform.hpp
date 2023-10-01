#pragma once

#include <factories/app_window_factory.hpp>

#include <platform/win32/windows/win32_app_window.hpp>

namespace engine::platform::win32 {
	inline void init() {
		// register app window factories
		factories::app_window::register_type("Win32AppWindow", []()
			{
				return std::make_shared<windows::Win32AppWindow>();
			}
		);
	}
}