#include "base_application.hpp"

namespace engine {
    static std::shared_ptr<Application> g_app_itx{nullptr};

    Application::Application(std::string_view name) : s_name(name) {
        // make this application instance available to everyone
        if(g_app_itx == nullptr) {
            g_app_itx = std::shared_ptr<Application>(this);
        }
    }

    std::shared_ptr<Application> Application::current() {
        return g_app_itx;
    }
}