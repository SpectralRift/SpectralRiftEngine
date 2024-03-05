#pragma once

#include <functional>
#include <vector>
#include <memory>

#include <api.hpp>

namespace engine::utils {
    template<typename... Args>
    class ENGINE_API Event {
    public:
        using Listener = std::function<void(Args...)>;
        using ListenerID = int;

        ListenerID add_listener(const Listener &listener) {
            listeners.push_back(std::make_pair(next_id++, std::make_shared<Listener>(listener)));
            return next_id - 1;
        }

        void remove_listener(ListenerID id) {
            auto it = std::find_if(listeners.begin(), listeners.end(), [id](const auto &pair) {
                return pair.first == id;
            });

            if (it != listeners.end()) {
                listeners.erase(it);
            }
        }

        void invoke(Args... args) {
            for (auto &listenerPair : listeners) {
                auto &listener = listenerPair.second;
                (*listener)(args...);
            }
        }

    private:
        std::vector<std::pair<ListenerID, std::shared_ptr<Listener>>> listeners;
        ListenerID next_id = 0;
    };
}