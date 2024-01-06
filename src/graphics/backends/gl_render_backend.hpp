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

		void define_vertex_attrib(uintptr_t index, utils::enums::graphics::DataType type, int count, int stride);
	private:
		static uintptr_t buffer_target_mapper(utils::enums::graphics::BufferTarget target);
		static uintptr_t shader_type_mapper(utils::enums::graphics::ShaderType type);
		static uintptr_t data_type_mapper(utils::enums::graphics::DataType type);
		static uintptr_t primitive_type_mapper(utils::enums::graphics::PrimitiveType type);
		static uintptr_t gl_loader(std::string_view name);
	};
}