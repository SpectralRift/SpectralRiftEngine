#pragma once

#include <vector>
#include <memory>

#include <utils/vector2.hpp>
#include "switch_controller.hpp"

#define NX_JOYSTICK_COUNT_MAX 8

namespace engine::input {
    struct SwitchJoystickInput {
        bool init();
        void destroy();
        void set_max_players(uint8_t num_players);

        void poll();

        const std::vector<std::shared_ptr<SwitchPlayerController>>& get_controllers() {
            return players;
        }
    protected:
        bool is_init{false};
        uint8_t num_max_players{1};
        std::vector<std::shared_ptr<SwitchPlayerController>> players{};
    };
}