#include <input/base_controller_input.hpp>

#include <utils/macros/debug.hpp>

namespace engine::input {
    int BaseControllerInput::axis_count() {
        return (int) v_axes.size();
    }

    double BaseControllerInput::get_axis_value(int axis_id) {
        if (axis_id >= 0 && axis_id < v_axes.size()) {
            return v_axes[axis_id];
        } else {
            DEBUG_MSG("Value of axis %i requested, but there are only a maximum of %i axes available!", axis_id, axis_count())
            return 0.f;
        }
    }

    void BaseControllerInput::reserve_axis(int num_axis) {
        v_axes.clear();

        for(int i = 0; i < num_axis; i++) {
            v_axes.push_back(0.f);
        }
    }

    void BaseControllerInput::set_axis_value(int axis_id, double value) {
        if (axis_id >= 0 && axis_id <= v_axes.size()) {
            v_axes[axis_id] = value;
        } else {
            DEBUG_MSG("Failed to set the value of axis %i; there are only a maximum of %i axes available!", axis_id,
                      axis_count())
        }
    }
}