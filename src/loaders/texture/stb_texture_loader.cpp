#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_NO_GIF
#define STBI_NO_LINEAR
#define STBI_NO_HDR

#include <loaders/texture/stb_texture_loader.hpp>
#include <loaders/texture/stb_image.h>
#include <utils/macros/debug.hpp>

namespace engine::loaders {
    std::shared_ptr<graphics::Texture> stb_texture_load(const io::IStreamSPtr &stream) {
        // read the entire file into memory
        auto buf = stream->read(stream->size());
        utils::IVector2 size{};
        int channels = 0;

        auto tex = std::make_shared<graphics::Texture>();
        utils::ColorArray pixels;

        stbi_set_flip_vertically_on_load(true);
        auto image_data = stbi_load_from_memory(buf.data(), buf.size(), &size.x, &size.y, &channels, 0);

        if (!image_data) {
            return nullptr;
        }

        DEBUG_MSG("stbi loaded %ix%i texture with %i channels of color!\n", size.x, size.y, channels)

        for (int i = 0; i < ((size.x * size.y) * channels); i += channels) {

            if (channels == 3) {
                auto p = *(utils::RGBColor *) (image_data + i);
                pixels.push_back({p.r, p.g, p.b});
            } else if (channels == 4) {
                auto p = *(utils::Color *) (image_data + i);
                pixels.push_back(p);
            }
        }

        tex->set_data(pixels, size);

        stbi_image_free(image_data);

        return tex;
    }
}