#pragma once

#include <graphics/interfaces/i_render_backend.hpp>
#include <api.hpp>

namespace engine::graphics::backends {
	struct ENGINE_API GLRenderBackend : public IRenderBackend {
		bool init();

        std::vector<engine::utils::enums::graphics::ShaderLanguage> get_supported_shader_languages();
        utils::Version get_version();
        bool is_mobile();

        void set_viewport_size(const utils::IVector2& size);

		void set_clear_color(const utils::Color& color);
		void clear();

		uintptr_t create_buffer();
        void delete_buffer(uintptr_t id);
		void bind_buffer(uintptr_t id, utils::enums::graphics::BufferTarget target);
		void set_buffer_data(utils::enums::graphics::BufferTarget target, size_t buffer_size, size_t type_size, void* buf);
		void draw_buffer(int vertices_count, utils::enums::graphics::PrimitiveType type);
        void draw_indexed_buffer(int indices_count, utils::enums::graphics::DataType index_data_type, utils::enums::graphics::PrimitiveType type);

		uintptr_t create_vertex_buffer();
        void delete_vertex_buffer(uintptr_t id);
		void bind_vertex_buffer(uintptr_t id);

		uintptr_t create_shader(utils::enums::graphics::ShaderType type);
		void use_shader_source(uintptr_t id, std::string_view source);
		bool compile_shader(uintptr_t id);
		void delete_shader(uintptr_t id);
		std::string get_shader_log(uintptr_t id);

		uintptr_t create_shader_program();
		void use_shader_program(uintptr_t id);
		void attach_shader_to_program(uintptr_t program_id, uintptr_t shader_id);
		bool link_program(uintptr_t id);

		std::string get_program_log(uintptr_t id);
        uintptr_t get_program_uniform(uintptr_t handle, std::string_view name);
        void set_uniform_color(uintptr_t uf_handle, const utils::Color& color);
        void set_uniform_mat4(uintptr_t uf_handle, const glm::mat4& mat);
        void set_uniform_int(uintptr_t uf_handle, int val);

        uintptr_t create_texture();
        void delete_texture(uintptr_t id);
        void bind_texture(uintptr_t id, uint16_t slot);
        void upload_texture_data(uintptr_t id, const utils::ColorArray& data, const utils::IVector2& size);

		void define_vertex_attrib(uintptr_t index, utils::enums::graphics::DataType type, int count, int stride, uintptr_t offset);

        void enable_wireframe(bool state);
        void enable_features(uint32_t features);
	private:
		static uintptr_t buffer_target_mapper(utils::enums::graphics::BufferTarget target);
		static uintptr_t shader_type_mapper(utils::enums::graphics::ShaderType type);
		static uintptr_t data_type_mapper(utils::enums::graphics::DataType type);
		static uintptr_t primitive_type_mapper(utils::enums::graphics::PrimitiveType type);
		static uintptr_t gl_loader(std::string_view name);
	};
}