#pragma once

#include <utils/vector2.hpp>

#include <map>

namespace engine::input {
    struct SwitchTouchScreenInput {
        bool init();
        void destroy();

        void poll();
    protected:
        std::map<unsigned int, utils::FVector2> finger_positions;
    };
}