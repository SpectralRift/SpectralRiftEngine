#ifdef ENGINE_TARGET_LINUX
#pragma once

#include <api.hpp>
#include <windows/base_app_window.hpp>

#include <X11/X.h>
#include <X11/Xlib.h>

namespace engine::windows {
	struct X11AppWindow : public BaseAppWindow {
		bool init();
		void destroy();

		void set_title(std::string_view title);
		void set_position(utils::IVector2 position);
		void set_size(utils::IVector2 size);

		utils::IVector2 get_size();
		utils::IVector2 get_position();

		void process_events();
		void update();

		void show();
		void hide();

		uintptr_t get_device_ctx();
		uintptr_t get_window_handle();
	private:
		Display* display{ nullptr };
		Window window;

		Atom wm_delete_message;
	};
}
#endif