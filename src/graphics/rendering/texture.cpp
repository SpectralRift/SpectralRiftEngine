#include <graphics/rendering/texture.hpp>

#include <engine.hpp>

namespace engine::graphics {
    static TexturePtr tex_white_pixel{nullptr};

    Texture::~Texture() {
        if (tex_handle == PTR_NULL) {
            engine::get_backend()->delete_texture(tex_handle);
            tex_handle = PTR_NULL;
        }
    }

    void Texture::set_data(const utils::ColorArray &data, const utils::IVector2& size) {
        tex_size = size;
        tex_buffer = data;

        is_uploaded = false;
    }

    void Texture::bind(uint16_t slot) {
        if(tex_handle == PTR_NULL) {
            tex_handle = engine::get_backend()->create_texture();
        }

        engine::get_backend()->bind_texture(tex_handle, slot);

        if(!is_uploaded) {
            engine::get_backend()->upload_texture_data(tex_handle, tex_buffer, tex_size);
            is_uploaded = true;
        }
    }

    TexturePtr get_white_pixel() {
        if(tex_white_pixel == nullptr) {
            tex_white_pixel = std::make_shared<Texture>();
            tex_white_pixel->set_data({utils::colors::white}, {1, 1});
        }

        return tex_white_pixel;
    }
}