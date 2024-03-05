#pragma once

#include <memory>

#include <api.hpp>
#include <io/i_stream.hpp>
#include <graphics/rendering/texture.hpp>

namespace engine::loaders {
    std::shared_ptr<graphics::Texture> ENGINE_API stb_texture_load(const io::IStreamSPtr& stream);
}