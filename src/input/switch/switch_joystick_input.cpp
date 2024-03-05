#include <input/switch/switch_joystick_input.hpp>

#include <utils/macros/debug.hpp>
#include <vector>
#include <input/input_manager.hpp>

namespace engine::input {
    bool SwitchJoystickInput::init() {
        if(!is_init) {
            set_max_players(1);
        }

        for (const auto &c: players) {
            c->init();
        }

        is_init = true;

        return true;
    }

    void SwitchJoystickInput::set_max_players(uint8_t num_players) {
        if (num_players < 0) {
            DEBUG_MSG("num_players < 0\n")
            ASSERT_MSG(num_players > 0, "num_players < 0")
        }

        if (num_players > NX_JOYSTICK_COUNT_MAX) {
            DEBUG_MSG("num_players > NX_JOYSTICK_COUNT_MAX!\n")
            ASSERT_MSG(num_players < NX_JOYSTICK_COUNT_MAX, "num_players > NX_JOYSTICK_COUNT_MAX")
        }

        num_max_players = num_players;

        // configure the controller pad scheme
        padConfigureInput(num_players, HidNpadStyleSet_NpadStandard);

        if(players.size() < num_max_players) {
            for(int i = (int) players.size(); i < num_max_players; i++) {
                auto c = std::make_shared<SwitchPlayerController>((HidNpadIdType) i);
                if(is_init) {
                    c->init();
                }

                players.push_back(c);
            }
        } else {
            DEBUG_MSG("subtract\n")
        }
    }

    void SwitchJoystickInput::destroy() {
        // destroy players
        for (const auto &c: players) {
            c->destroy();
        }

        // clear list
        players.clear();
    }

    void SwitchJoystickInput::poll() {
        for (const auto &c: players) {
            c->poll();

            // something happened, show config panel
            if(!c->is_connected() && !c->is_handheld()) {
                HidLaControllerSupportResultInfo info;
                HidLaControllerSupportArg args;
                hidLaCreateControllerSupportArg(&args);

                args.hdr.player_count_max = (s8) num_max_players;

                // in single player mode, we allow the use of handheld mode!
                if(num_max_players == 1) {
                    args.hdr.enable_single_mode = 1;
                }

                hidLaShowControllerSupportForSystem(&info, &args, false);

                for (const auto &ctlr: players) {
                    ctlr->reconfigure();
                }

                break;
            }
        }
    }
}