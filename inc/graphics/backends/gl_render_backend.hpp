#pragma once
#include <graphics/i_render_backend.hpp>

#include <api.hpp>

namespace engine::graphics::backends {
	struct ENGINE_API GLRenderBackend : public IRenderBackend {
		bool init();

		void set_viewport_size(const utils::IVector2& size);

		void set_clear_color(const utils::Color& color);
		void clear();

		uintptr_t create_buffer();
		void bind_buffer(uintptr_t id, BufferTarget target);
		void set_buffer_data(BufferTarget target, size_t buffer_size, size_t type_size, void* buf);
		void draw_buffer(int vertices_count, PrimitiveType type);

		uintptr_t create_vertex_buffer();
		void bind_vertex_buffer(uintptr_t id);

		uintptr_t create_shader(ShaderType type);
		void use_shader_source(uintptr_t id, std::string_view source);
		bool compile_shader(uintptr_t id);
		void delete_shader(uintptr_t id);
		std::string get_shader_log(uintptr_t id);

		uintptr_t create_shader_program();
		void use_shader_program(uintptr_t id);
		void attach_shader_to_program(uintptr_t program_id, uintptr_t shader_id);
		bool link_program(uintptr_t id);
		std::string get_program_log(uintptr_t id);

		void define_vertex_attrib(uintptr_t index, DataType type, int count, int stride);
	private:
		static uintptr_t buffer_target_mapper(BufferTarget target);
		static uintptr_t shader_type_mapper(ShaderType type);
		static uintptr_t data_type_mapper(DataType type);
		static uintptr_t primitive_type_mapper(PrimitiveType type);
	};
}