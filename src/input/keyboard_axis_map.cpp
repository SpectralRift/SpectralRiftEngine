#include <input/keyboard_axis_map.hpp>
#include <input/input_manager.hpp>

namespace engine::input {
    bool KeyboardAxisMap::is_map_active() {
        return engine::input::InputManager::IsKeyDown(e_key);
    }

    double KeyboardAxisMap::get_axis_value() {
        return engine::input::InputManager::IsKeyDown(e_key) ? d_value : 0.f;
    }
}