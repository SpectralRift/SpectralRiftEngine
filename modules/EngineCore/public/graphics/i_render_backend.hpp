#pragma once

#include <string>
#include <memory>
#include <string_view>
#include <vector>

#include "../../src_/utils/color.hpp"
#include "../../src_/utils/vector2.hpp"
#include "../../src_/utils/version.hpp"

#include "../../src_/utils/enums/graphics.hpp"

#include "glm/glm.hpp"

namespace engine::graphics {
    struct IRenderBackend {
        virtual bool init() = 0;

        virtual std::vector<engine::utils::enums::graphics::ShaderLanguage> get_supported_shader_languages() = 0;

        virtual utils::Version get_version() = 0;

        virtual bool is_mobile() = 0;

        virtual void set_viewport_size(const utils::IVector2 &size) = 0;

        virtual void set_clear_color(const utils::Color &color) = 0;

        virtual void clear() = 0;

        virtual uintptr_t create_buffer() = 0;

        virtual void delete_buffer(uintptr_t id) = 0;

        virtual void bind_buffer(uintptr_t id, utils::enums::graphics::BufferTarget target) = 0;

        virtual void set_buffer_data(utils::enums::graphics::BufferTarget target, size_t buffer_size, size_t type_size,
                                     void *buf) = 0;

        virtual void draw_buffer(int vertices_count, utils::enums::graphics::PrimitiveType type) = 0;

        virtual void draw_indexed_buffer(int indices_count, utils::enums::graphics::DataType index_data_type,
                                         utils::enums::graphics::PrimitiveType type) = 0;

        virtual uintptr_t create_vertex_buffer() = 0;

        virtual void delete_vertex_buffer(uintptr_t id) = 0;

        virtual void bind_vertex_buffer(uintptr_t id) = 0;

        virtual uintptr_t create_shader(utils::enums::graphics::ShaderType type) = 0;

        virtual void use_shader_source(uintptr_t id, std::string_view source) = 0;

        virtual bool compile_shader(uintptr_t id) = 0;

        virtual void delete_shader(uintptr_t id) = 0;

        virtual std::string get_shader_log(uintptr_t id) = 0;

        virtual uintptr_t create_shader_program() = 0;

        virtual void use_shader_program(uintptr_t id) = 0;

        virtual void attach_shader_to_program(uintptr_t program_id, uintptr_t shader_id) = 0;

        virtual bool link_program(uintptr_t id) = 0;

        virtual std::string get_program_log(uintptr_t id) = 0;

        virtual uintptr_t create_texture() = 0;

        virtual void delete_texture(uintptr_t id) = 0;

        virtual void bind_texture(uintptr_t id, uint16_t slot) = 0;

        // upload as RGBA
        virtual void upload_texture_data(uintptr_t id, const utils::ColorArray &data, const utils::IVector2 &size) = 0;

        virtual uintptr_t get_program_uniform(uintptr_t handle, std::string_view name) = 0;

        virtual void set_uniform_color(uintptr_t uf_handle, const utils::Color &color) = 0;

        virtual void set_uniform_mat4(uintptr_t uf_handle, const glm::mat4 &mat) = 0;

        virtual void set_uniform_int(uintptr_t uf_handle, int val) = 0;

        virtual void define_vertex_attrib(uintptr_t index, utils::enums::graphics::DataType type, int count, int stride,
                                          uintptr_t offset) = 0;

        virtual void enable_wireframe(bool state) = 0;

        virtual void enable_features(uint32_t features) = 0;
    };

    using IRenderBackendSPtr = std::shared_ptr<IRenderBackend>;
}