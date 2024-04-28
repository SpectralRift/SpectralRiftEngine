#include <input/input_manager.hpp>
#include <utils/macros/debug.hpp>

namespace engine::input {
    std::map<unsigned int, utils::FVector2> InputManager::touch_pos;

    const utils::FVector2 &InputManager::get_axis_value(int axis_id) {
        if (axis_id + 1 > MAX_INPUT_AXES) {
            DEBUG_MSG("axis_id > MAX_INPUT_AXES\n")
            ASSERT_MSG(axis_id + 1 < MAX_INPUT_AXES, "axis_id > MAX_INPUT_AXES")
        }

        return axes[axis_id];
    }

    void InputManager::_set_axis_value(int axis_id, const utils::FVector2 &value) {
        if (axis_id + 1 > MAX_INPUT_AXES) {
            DEBUG_MSG("axis_id > MAX_INPUT_AXES\n")
            ASSERT_MSG(axis_id + 1 < MAX_INPUT_AXES, "axis_id > MAX_INPUT_AXES")
        }

        axes[axis_id].x = value.x;
        axes[axis_id].y = value.y;
    }

    void InputManager::_set_touch_state(uint8_t finger_id, const utils::FVector2 &pos, bool is_down) {
        if (is_down) {
            if (!touch_pos.contains(finger_id)) {
//                DEBUG_MSG("FINGER_DOWN: %i %fx%f\n", finger_id, pos.x, pos.y);
                touch_pos.insert({finger_id, pos});
            } else if (touch_pos.at(finger_id) != pos) {
                // if I call the copy constructor it crashes wtf
                touch_pos.at(finger_id).x = pos.x;
                touch_pos.at(finger_id).y = pos.y;

//                DEBUG_MSG("FINGER_MOVE: %i %fx%f\n", finger_id, pos.x, pos.y);
            }
        } else if (touch_pos.contains(finger_id)) {
//            DEBUG_MSG("FINGER_UP: %i %fx%f\n", finger_id, pos.x, pos.y);
            touch_pos.erase(finger_id);
        }
    }

    uint8_t InputManager::GetTouchFingerCount() {
        return touch_pos.size();
    }

    int InputManager::IsTouching(uint8_t finger) {
        return touch_pos.contains(finger);
    }

    utils::FVector2 InputManager::GetTouchAxis(uint8_t finger) {
        if (IsTouching(finger)) return touch_pos.at(finger);
        return {};
    }
}