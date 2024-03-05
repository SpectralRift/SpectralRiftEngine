#if ANDROID
#pragma once

#include <api.hpp>
#include <windows/base_app_window.hpp>

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/input.h>

namespace engine::windows {
	struct ENGINE_API AndroidJNIAppWindow : public BaseAppWindow {
        AndroidJNIAppWindow(ANativeWindow* window) : window(window) {}

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

        bool handle_input_event(int action, int source, const utils::IVector2& pos);
	private:
        ANativeWindow* window;
	};
}
#endif