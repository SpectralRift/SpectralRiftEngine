#pragma once

#include <string_view>
#include <memory>

#include <utils/vector2.hpp>
#include <utils/event.hpp>
#include <utils/enums/input.hpp>

#include <graphics/i_graphics_context.hpp>
#include <input/input_manager.hpp>

namespace engine::windows {
    struct BaseAppWindow {
#pragma region Events
        /// <summary>
        /// Event delegate for the KeyDown(KeyType key) event.
        /// </summary>
        using KeyDownEventDelegate = utils::Event<utils::enums::input::KeyType>;

        /// <summary>
        /// Event delegate for the KeyUp(KeyType key) event.
        /// </summary>
        using KeyUpEventDelegate = utils::Event<utils::enums::input::KeyType>;

        /// <summary>
        /// Event delegate for the WindowQuitRequested() event.
        /// </summary>
        using WindowQuitRequestedEventDelegate = utils::Event<>;

        /// <summary>
        /// Event delegate for the WindowResize(const IVector2& size) event.
        /// </summary>
        using WindowResizeEventDelegate = utils::Event<const utils::IVector2 &>;

        /// <summary>
        /// Event delegate for the MouseMove(const IVector2& position, bool isTouch) event.
        /// </summary>
        using MouseMoveEventDelegate = utils::Event<const utils::IVector2 &, bool>;

        /// <summary>
        /// Event delegate for the MouseKeyDown(const IVector2& position, MouseButton button, bool isTouch) event.
        /// </summary>
        using MouseKeyDownEventDelegate = utils::Event<const utils::IVector2 &, utils::enums::input::MouseButton, bool>;

        /// <summary>
        /// Event delegate for the MouseKeyUp(const IVector2& position, MouseButton button, bool isTouch) event.
        /// </summary>
        using MouseKeyUpEventDelegate = utils::Event<const utils::IVector2 &, utils::enums::input::MouseButton, bool>;

        /// <summary>
        /// Event delegate for the PadKeyStateUpdate(PadButton buttons, PadButton last_buttons) event.
        /// </summary>
        using PadKeyStateUpdateEventDelegate = utils::Event<utils::enums::input::PadButton, utils::enums::input::PadButton>;
#pragma endregion

        // platform specific window implementations
        virtual bool init() = 0;

        virtual void destroy() {
            if (gfx_ctx != nullptr) {
                gfx_ctx->destroy();
            }
        };

        virtual void set_title(std::string_view title) = 0;

        virtual void set_position(utils::IVector2 position) = 0;

        virtual void set_size(utils::IVector2 size) = 0;

        virtual utils::IVector2 get_position() = 0;

        virtual utils::IVector2 get_size() = 0;

        virtual void process_events() = 0;

        virtual void update() = 0;

        virtual void show() = 0;

        virtual void hide() = 0;

        virtual bool is_focused() = 0;

        template<typename T>
        void user_data_put(std::string_view name, const std::shared_ptr<T> &value) {
            user_data_map[name] = std::static_pointer_cast<void>(value);
        }

        template<typename T>
        std::shared_ptr<T> user_data_get(std::string_view name) {
            auto it = user_data_map.find(name);

            if (it != user_data_map.end()) {
                return std::static_pointer_cast<T>(it->second);
            } else {
                return nullptr;
            }
        }

        graphics::IGraphicsContextSPtr get_graphics_context() {
            return gfx_ctx;
        }

        // base window functionality
        bool is_quit_requested() {
            return quit_requested;
        };

        void quit() {
            quit_requested = true;
        };
#pragma region Events

        /// <summary>
        /// Add a listener to the KeyDown event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_key_down_listener(const KeyDownEventDelegate::Listener &listener) {
            ev_key_down.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the KeyDown event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_key_down_listener(const KeyDownEventDelegate::ListenerID &listener) {
            ev_key_down.remove_listener(listener);
        }

        /// <summary>
        /// Add a listener to the KeyUp event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_key_up_listener(const KeyUpEventDelegate::Listener &listener) {
            ev_key_up.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the KeyUp event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_key_up_listener(const KeyUpEventDelegate::ListenerID &listener) {
            ev_key_up.remove_listener(listener);
        }

        /// <summary>
        /// Add a listener to the WindowQuitRequested event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_window_quit_requested_listener(const WindowQuitRequestedEventDelegate::Listener &listener) {
            ev_window_quit_requested.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the WindowQuitRequested event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_window_quit_requested_listener(const WindowQuitRequestedEventDelegate::ListenerID &listener) {
            ev_window_quit_requested.remove_listener(listener);
        }

        /// <summary>
        /// Add a listener to the WindowResize event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_window_resize_listener(const WindowResizeEventDelegate::Listener &listener) {
            ev_window_resize.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the WindowResize event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_window_resize_listener(const WindowResizeEventDelegate::ListenerID &listener) {
            ev_window_resize.remove_listener(listener);
        }

        /// <summary>
        /// Add a listener to the MouseMove event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_mouse_move_listener(const MouseMoveEventDelegate::Listener &listener) {
            ev_mouse_move.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the MouseMove event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_mouse_move_listener(const MouseMoveEventDelegate::ListenerID &listener) {
            ev_mouse_move.remove_listener(listener);
        }

        /// <summary>
        /// Add a listener to the MouseKeyDown event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_mouse_key_down_listener(const MouseKeyDownEventDelegate::Listener &listener) {
            ev_mouse_key_down.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the MouseKeyDown event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_mouse_key_down_listener(const MouseKeyDownEventDelegate::ListenerID &listener) {
            ev_mouse_key_down.remove_listener(listener);
        }

        /// <summary>
        /// Add a listener to the MouseKeyUp event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_mouse_key_up_listener(const MouseKeyUpEventDelegate::Listener &listener) {
            ev_mouse_key_up.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the MouseKeyUp event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_mouse_key_up_listener(const MouseKeyUpEventDelegate::ListenerID &listener) {
            ev_mouse_key_up.remove_listener(listener);
        }

        /// <summary>
        /// Add a listener to the MouseKeyUp event.
        /// </summary>
        /// <param name="listener">The listener function to be added.</param>
        void add_pad_key_state_update_listener(const PadKeyStateUpdateEventDelegate::Listener &listener) {
            ev_pad_key_state_update.add_listener(listener);
        }

        /// <summary>
        /// Remove a listener from the MouseKeyUp event.
        /// </summary>
        /// <param name="listener">The listener function to be removed.</param>
        void remove_pad_key_state_update_listener(const PadKeyStateUpdateEventDelegate::ListenerID &listener) {
            ev_pad_key_state_update.remove_listener(listener);
        }

#pragma endregion
    protected:
        bool quit_requested{false};
        std::unordered_map<std::string_view, std::shared_ptr<void>> user_data_map;
        graphics::IGraphicsContextSPtr gfx_ctx{nullptr};

        KeyDownEventDelegate ev_key_down;
        KeyUpEventDelegate ev_key_up;
        WindowQuitRequestedEventDelegate ev_window_quit_requested;
        WindowResizeEventDelegate ev_window_resize;
        MouseMoveEventDelegate ev_mouse_move;
        MouseKeyDownEventDelegate ev_mouse_key_down;
        MouseKeyUpEventDelegate ev_mouse_key_up;
        PadKeyStateUpdateEventDelegate ev_pad_key_state_update;
    };

    using BaseAppWindowSPtr = std::shared_ptr<BaseAppWindow>;
}