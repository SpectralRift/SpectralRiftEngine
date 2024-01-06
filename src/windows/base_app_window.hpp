#pragma once

#include <string_view>
#include <utils/vector2.hpp>
#include <memory>

namespace engine::windows {
	struct BaseAppWindow {
		// platform specific window implementations
		virtual bool init() = 0;
		virtual void destroy() = 0;

		virtual void set_title(std::string_view title) = 0;
		virtual void set_position(utils::IVector2 position) = 0;
		virtual void set_size(utils::IVector2 size) = 0;

		virtual utils::IVector2 get_position() = 0;
		virtual utils::IVector2 get_size() = 0;

		virtual void process_events() = 0;
		virtual void update() = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

		virtual uintptr_t get_device_ctx() = 0;
		virtual uintptr_t get_window_handle() = 0;

		// base window functionality
		bool is_quit_requested() {
			return quit_requested;
		};

		void quit() {
			quit_requested = true;
		};
	protected:
		bool quit_requested{ false };
	};

	using BaseAppWindowSPtr = std::shared_ptr<BaseAppWindow>;
}