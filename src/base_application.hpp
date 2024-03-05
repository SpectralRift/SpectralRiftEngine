#pragma once

#include <windows/base_app_window.hpp>

#include <vector>
#include <string>
#include <string_view>
#include <memory>

namespace engine {
    struct Application {
        explicit Application(std::string_view name);

        virtual bool init() { return false; };

        virtual void destroy() {};

        virtual void render() {};

        // returns the current application instance
        static std::shared_ptr<Application> current();

    protected:
        std::string s_name;
        std::vector<void *> v_all_windows;
        engine::windows::BaseAppWindowSPtr window;
    };
}