#pragma once

#include <vector>

namespace engine::input {
    struct BaseControllerInput {
        int axis_count();
        double get_axis_value(int axis_id);
    protected:
        void reserve_axis(int num_axis);
        void set_axis_value(int axis_id, double value);

        std::vector<double> v_axes;
    };
}