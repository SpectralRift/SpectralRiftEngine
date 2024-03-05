#ifdef ENGINE_TARGET_LINUX
#pragma once

#include <api.hpp>
#include <windows/base_app_window.hpp>

#include <X11/X.h>
#include <X11/Xlib.h>

namespace engine::windows {
	struct ENGINE_API X11AppWindow : public BaseAppWindow {
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

		void* get_device_ctx();
		void* get_window_handle();
	private:
        static utils::enums::input::MouseButton map_xorg_mouse_button(int button);

		Display* display{ nullptr };
		Window window{};

		Atom wm_delete_message{};

        static utils::enums::input::KeyType map_xorg_key_type(KeySym key);
    };
}
#endif