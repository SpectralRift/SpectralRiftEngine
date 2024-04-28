#pragma once

#include <input/input_axis_map.hpp>
#include <utils/enums/input.hpp>

namespace engine::input {
    struct KeyboardAxisMap : public AbstractInputAxisMap {
        KeyboardAxisMap(utils::enums::input::KeyType key, double value) : e_key(key), d_value(value) {}

        bool is_map_active();
        double get_axis_value();
    protected:
        utils::enums::input::KeyType e_key;
        double d_value;
    };
}