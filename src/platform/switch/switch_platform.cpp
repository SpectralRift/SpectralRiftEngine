#if defined(ENGINE_TARGET_SWITCH)

#include <platform/switch/switch_platform.hpp>
#include <windows/switch_app_window.hpp>

#include <switch.h>

namespace engine::platform {
    void init_switch() {
        // mount romfs
        romfsInit();

        socketInitializeDefault();
        nxlinkStdio(); // stdout/stderr > NETWORK

        // register app window factories
        factories::app_window::register_type("SwitchWindow", []()
                                             {
                                                 return std::make_shared<windows::SwitchAppWindow>();
                                             }
        );
    }

    void terminate_switch() {
        // unmount romfs
        romfsExit();

        socketExit();
    }

    uintptr_t load_library_switch(std::string_view lib_name) {
        return 0;
    }

    uintptr_t get_library_func_switch(uintptr_t lib_handle, std::string_view func) {
        return 0;
    }
}
#endif