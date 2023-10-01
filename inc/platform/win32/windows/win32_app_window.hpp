#pragma once

#include <api.hpp>
#include <windows/base_app_window.hpp>

#include <Windows.h>

namespace engine::windows {
	struct Win32AppWindow : public BaseAppWindow {
		bool init();
		void destroy();

		void set_title(std::string_view title);
		void set_position(const utils::IVector2& position);
		void set_size(const utils::IVector2& size);

		utils::IVector2 get_size();
		utils::IVector2 get_position();

		void process_events();
		void update();

		void show();
		void hide();

		uintptr_t get_device_ctx();
	private:
		HWND window_handle{};

		void assert_window_init();

		LRESULT CALLBACK window_processor(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK window_processor_static(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static bool engine_window_class_exists();

		static constexpr const char* default_win_class_name{ "SPECTRAL_DEFWINCLASS" };
		static constexpr const char* default_win_title{ "Spectral Rift Engine" };
	};
}