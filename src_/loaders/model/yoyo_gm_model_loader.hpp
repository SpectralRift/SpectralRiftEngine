#pragma once

#include <memory>

#include <io/i_stream.hpp>
#include <api.hpp>
#include <graphics/rendering/vertex_buffer.hpp>
#include <components/mesh_component.hpp>

namespace engine::loaders {
    std::shared_ptr<components::MeshComponent> ENGINE_API yoyo_gm_model_load(const io::IStreamSPtr& stream);
}