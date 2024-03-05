#pragma once

#include <vector>
#include <memory>
#include <map>

#include <utils/vector2.hpp>
#include <utils/enums/input.hpp>
#include "i_controller_input.hpp"

#define MAX_INPUT_AXES 16

namespace engine::input {
    struct InputManager {
#ifdef ENGINE_BUILD_LIBRARY
#pragma region Internal functions

        void _set_key_value(engine::utils::enums::input::KeyType type, bool value);
        void _set_axis_value(int axis_id, const utils::FVector2 &value);

        static void _set_touch_state(uint8_t finger_id, const utils::FVector2 &pos, bool is_down);

#pragma endregion
#endif

        [[nodiscard]] const utils::FVector2 &get_axis_value(int axis_id);

        static uint8_t GetTouchFingerCount();

        static int IsTouching(uint8_t finger);

        static utils::FVector2 GetTouchAxis(uint8_t finger);

        static void ConfigureInputMapping(std::string_view map_name, std::vector<std::string_view> mappings);

        static bool IsKeyDown(engine::utils::enums::input::KeyType type);

    protected:
        utils::FVector2 axes[MAX_INPUT_AXES];
        static std::map<unsigned int, utils::FVector2> touch_pos;
    };
}
