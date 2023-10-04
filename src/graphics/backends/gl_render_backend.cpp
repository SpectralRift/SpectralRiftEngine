#include <stdexcept>

#include <graphics/backends/gl_render_backend.hpp>
#include <utils/macros/debug.hpp>
#include <platform/platform.hpp>

#include <glad/glad.h>

namespace engine::graphics::backends {
	uintptr_t GLRenderBackend::buffer_target_mapper(BufferTarget target) {
		if (target == ARRAY_BUFFER) {
			return GL_ARRAY_BUFFER;
		}

		return GL_NONE;
	}

	uintptr_t GLRenderBackend::shader_type_mapper(ShaderType type) {
		if (type == VERTEX) {
			return GL_VERTEX_SHADER;
		}
		else if (type == FRAGMENT) {
			return GL_FRAGMENT_SHADER;
		}

		return GL_NONE;
	}

	uintptr_t GLRenderBackend::data_type_mapper(DataType type) {
		if (type == INT) {
			return GL_INT;
		}
		else if (type == FLOAT) {
			return GL_FLOAT;
		}

		return GL_NONE;
	}

	uintptr_t GLRenderBackend::primitive_type_mapper(PrimitiveType type) {
		if (type == TRIANGLES) {
			return GL_TRIANGLES;
		}
		else if (type == POINTS) {
			return GL_POINTS;
		}
		else if (type == LINES) {
			return GL_LINES;
		}

		return GL_NONE;
	}

	bool GLRenderBackend::init() {
		if (!gladLoadGLLoader((GLADloadproc)engine::platform::graphics::get_gl_func)) {
			DEBUG_MSG("Failed to load OpenGL through glad!\n");
			return false;
		}

		if (!gladLoadGL()) {
			DEBUG_MSG("Failed to initialize OpenGL through glad!\n");
			return false;
		}

		DEBUG_MSG("OpenGL %d.%d (%s) loaded!\n", GLVersion.major, GLVersion.minor, glGetString(GL_VENDOR));
		DEBUG_MSG("Renderer: %s\n", glGetString(GL_RENDERER))
			DEBUG_MSG("Extensions: %s\n", glGetString(GL_EXTENSIONS));

		return true;
	}

	void GLRenderBackend::set_viewport_size(const utils::IVector2& size) {
		glViewport(0, 0, size.x, size.y);
	}

	void GLRenderBackend::set_clear_color(const utils::Color& color) {
		glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	}

	void GLRenderBackend::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	uintptr_t GLRenderBackend::create_buffer() {
		uintptr_t buf_id = -1;
		glGenBuffers(1, &buf_id);

		return buf_id;
	}

	void GLRenderBackend::bind_buffer(uintptr_t id, BufferTarget target) {
		GLenum buffer_target = buffer_target_mapper(target);

		if (buffer_target == GL_NONE) {
			throw std::runtime_error("invalid buffer target!");
		}

		glBindBuffer(buffer_target, id);
	}

	void GLRenderBackend::set_buffer_data(BufferTarget target, size_t buffer_size, size_t type_size, void* buf) {
		GLenum buffer_target = buffer_target_mapper(target);

		if (buffer_target == GL_NONE) {
			throw std::runtime_error("invalid buffer target!");
		}

		glBufferData(buffer_target, buffer_size * type_size, buf, GL_STATIC_DRAW);
	}

	void GLRenderBackend::draw_buffer(int vertices_count, PrimitiveType type) {
		GLenum primitive_type = primitive_type_mapper(type);

		if (primitive_type == GL_NONE) {
			throw std::runtime_error("invalid primitive type!");
		}

		glDrawArrays(primitive_type, 0, vertices_count);
	}

	uintptr_t GLRenderBackend::create_vertex_buffer() {
		uintptr_t buf_id = -1;
		glGenVertexArrays(1, &buf_id);

		return buf_id;
	}

	void GLRenderBackend::bind_vertex_buffer(uintptr_t id) {
		glBindVertexArray(id);
	}

	uintptr_t GLRenderBackend::create_shader(ShaderType type) {
		GLenum shader_type = shader_type_mapper(type);

		if (shader_type == GL_NONE) {
			throw std::runtime_error("invalid shader type!");
		}

		uintptr_t shader_id = glCreateShader(shader_type);
		if (shader_id == 0) shader_id = -1;

		return shader_id;
	}

	void GLRenderBackend::use_shader_source(uintptr_t id, std::string_view source) {
		const char* srcs[1] = { source.data() };
		const GLint sizes[1] = { (GLint) source.size() };

		glShaderSource(id, 1, srcs, sizes);
	}

	bool GLRenderBackend::compile_shader(uintptr_t shader) {
		int ret;

		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);

		return ret == GL_TRUE;
	}

	void GLRenderBackend::delete_shader(uintptr_t id) {
		glDeleteShader(id);
	}

	std::string GLRenderBackend::get_shader_log(uintptr_t id) {
		int log_len = 0;
		std::string shader_log = "none";

		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);

		if (log_len) {
			shader_log.resize(log_len);
			glGetShaderInfoLog(id, log_len, NULL, &shader_log[0]);
		}

		return shader_log;
	}

	uintptr_t GLRenderBackend::create_shader_program() {
		uintptr_t id = glCreateProgram();
		if (id == 0) id = -1;

		return id;
	}

	void GLRenderBackend::use_shader_program(uintptr_t id) {
		glUseProgram(id);
	}

	void GLRenderBackend::attach_shader_to_program(uintptr_t program_id, uintptr_t shader_id) {
		glAttachShader(program_id, shader_id);
	}

	bool GLRenderBackend::link_program(uintptr_t id) {
		int ret;

		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &ret);

		return ret == GL_TRUE;
	}

	std::string GLRenderBackend::get_program_log(uintptr_t id) {
		int log_len = 0;
		std::string program_log = "none";

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);

		if (log_len) {
			program_log.resize(log_len);
			glGetProgramInfoLog(id, log_len, NULL, &program_log[0]);
		}

		return program_log;
	}

	void GLRenderBackend::define_vertex_attrib(uintptr_t index, DataType type, int count, int stride) {
		GLenum data_type = data_type_mapper(type);

		if (data_type == GL_NONE) {
			throw std::runtime_error("invalid data type!");
		}

		glVertexAttribPointer(index, count, data_type, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(index);
	}
}