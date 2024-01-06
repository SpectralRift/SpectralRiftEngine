#if ANDROID
#pragma once

#include <api.hpp>
#include <windows/base_app_window.hpp>

#include <android/native_window.h>
#include <android/native_window_jni.h>

namespace engine::windows {
	struct AndroidJNIAppWindow : public BaseAppWindow {
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

		uintptr_t get_device_ctx();
		uintptr_t get_window_handle();
	private:
        ANativeWindow* window;
	};
}
#endif