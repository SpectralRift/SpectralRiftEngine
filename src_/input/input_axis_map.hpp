#pragma once

namespace engine::input {
    struct AbstractInputAxisMap {
        virtual bool is_map_active() = 0;
        virtual double get_axis_value() = 0;
    };
}