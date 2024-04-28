#include "switch_touchscreen_input.hpp"

#include <utils/macros/debug.hpp>
#include <vector>
#include <input/input_manager.hpp>

extern "C" {
#include <switch/services/hid.h>
}

namespace engine::input {
    bool SwitchTouchScreenInput::init() {
        hidInitializeTouchScreen();

        return true;
    }

    void SwitchTouchScreenInput::destroy() {

    }

    void SwitchTouchScreenInput::poll() {
        HidTouchScreenState state{};

        if (hidGetTouchScreenStates(&state, 1)) {
            for (int i = 0; i < state.count; i++) {
                // logic taken from https://github.com/devkitPro/SDL/blob/switch-sdl-2.28/src/video/switch/SDL_switchtouch.c#L50
                // NX's touchscreen coordinates are provided absolutely to the screen size, which is 1280x720 by default.
                // we need to provide relative coordinates from 0 to 1 to the engine in order to handle input accordingly
                utils::FVector2 pos{(float) state.touches[i].x / 1280.0f, (float) state.touches[i].y / 720.0f};

                if (!finger_positions.contains(state.touches[i].finger_id)) {
                    finger_positions.insert({state.touches[i].finger_id, pos});

                    // position is new, so we consider this a touch-down event
                    InputManager::_set_touch_state(state.touches[i].finger_id, pos, true);
                } else {
                    if (finger_positions.at(state.touches[i].finger_id) != pos) {
                        finger_positions.at(state.touches[i].finger_id).x = pos.x;
                        finger_positions.at(state.touches[i].finger_id).y = pos.y;

                        // the position changed, so we signal it (touch-move event)
                        InputManager::_set_touch_state(state.touches[i].finger_id, pos, true);
                    }
                }
            }

            // use this vector to flag the inputs
            std::vector<unsigned int> flagged_fingers;

            for (const auto &finger: finger_positions) {
                bool finger_found = false;

                for (int i = 0; i < state.count; i++) {
                    if (finger.first == state.touches[i].finger_id) {
                        finger_found = true;
                        break;
                    }
                }

                if (!finger_found) {
                    flagged_fingers.push_back(finger.first); // flag the finger id for deletion
                    InputManager::_set_touch_state(finger.first, finger.second, false); // signal finger is up (touch-up event)
                }
            }

            // finally delete all the flagged fingers
            for (auto finger_id: flagged_fingers) {
                finger_positions.erase(finger_id);
            }
        }
    }
}