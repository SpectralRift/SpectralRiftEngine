#if defined(ENGINE_TARGET_SWITCH)
#pragma once

#include <api.hpp>
#include <windows/base_app_window.hpp>

#include <switch.h>
#include <utils/vector2.hpp>

namespace engine::windows {
	struct ENGINE_API SwitchAppWindow : public BaseAppWindow {
		bool init();

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
        utils::IVector2 map_touch_relative(const utils::IVector2& pos);
        static utils::enums::input::PadButton map_switch_pad_buttons(HidNpadButton keys_down);
        static utils::FVector2 map_stick(const utils::FVector2 &pos);

        NWindow* window_handle;
        PadState pad_state;
        uint64_t last_keys_down{0};
        uint16_t last_fingers{0};
        utils::IVector2 last_pos[16]{};
	};
}
#endif