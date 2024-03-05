#pragma once

#include <cstdint>
#include <memory>

#include <api.hpp>
#include <utils/color.hpp>
#include <utils/vector2.hpp>

namespace engine::graphics {
    struct ENGINE_API Texture {
        Texture() = default;
        ~Texture();

        void set_data(const utils::ColorArray& data, const utils::IVector2& size);

        void bind(uint16_t slot = 0);

        const utils::IVector2& get_size() {
            return tex_size;
        }
    private:
        bool is_uploaded{false};
        uintptr_t tex_handle{PTR_NULL};
        utils::IVector2 tex_size{};
        utils::ColorArray tex_buffer{};
    };

    using TexturePtr = std::shared_ptr<Texture>;

    TexturePtr ENGINE_API get_white_pixel();
}