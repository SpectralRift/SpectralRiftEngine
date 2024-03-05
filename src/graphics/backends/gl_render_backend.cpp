#include <stdexcept>

#include <engine.hpp>

#include <graphics/backends/gl_render_backend.hpp>
#include <utils/macros/debug.hpp>
#include <graphics/gfx_debug.hpp>
#include <glm/gtc/type_ptr.hpp>

#if defined(ENGINE_BACKEND_OPENGL_ES)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#else
#include <glad/gl.h>
#endif

namespace engine::graphics::backends {
//    void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
//                                     const GLchar *message, const void *userParam) {
////        DEBUG_MSG("OpenGL Message: %s\n", message)
//    }

    uintptr_t GLRenderBackend::buffer_target_mapper(utils::enums::graphics::BufferTarget target) {
        if (target == utils::enums::graphics::ARRAY_BUFFER) {
            return GL_ARRAY_BUFFER;
        } else if (target == utils::enums::graphics::BufferTarget::ELEMENT_ARRAY_BUFFER) {
            return GL_ELEMENT_ARRAY_BUFFER;
        }

        return GL_NONE;
    }

    uintptr_t GLRenderBackend::shader_type_mapper(utils::enums::graphics::ShaderType type) {
        if (type == utils::enums::graphics::VERTEX) {
            return GL_VERTEX_SHADER;
        } else if (type == utils::enums::graphics::FRAGMENT) {
            return GL_FRAGMENT_SHADER;
        }

        return GL_NONE;
    }

    uintptr_t GLRenderBackend::data_type_mapper(utils::enums::graphics::DataType type) {
        switch (type) {
            case utils::enums::graphics::DataType::UNSIGNED_BYTE:
                return GL_UNSIGNED_BYTE;
            case utils::enums::graphics::DataType::BYTE:
                return GL_BYTE;
            case utils::enums::graphics::DataType::INT:
                return GL_INT;
            case utils::enums::graphics::DataType::UNSIGNED_INT:
                return GL_UNSIGNED_INT;
            case utils::enums::graphics::DataType::FLOAT:
                return GL_FLOAT;
            default:
                return GL_NONE;
        }
    }

    uintptr_t GLRenderBackend::primitive_type_mapper(utils::enums::graphics::PrimitiveType type) {
        if (type == utils::enums::graphics::PrimitiveType::TRIANGLES) {
            return GL_TRIANGLES;
        } else if (type == utils::enums::graphics::PrimitiveType::POINTS) {
            return GL_POINTS;
        } else if (type == utils::enums::graphics::PrimitiveType::LINES) {
            return GL_LINES;
        }

        return GL_NONE;
    }

    std::vector<engine::utils::enums::graphics::ShaderLanguage> GLRenderBackend::get_supported_shader_languages() {
#if defined(ENGINE_BACKEND_OPENGL_ES)
        return {engine::utils::enums::graphics::ShaderLanguage::GLSL_ES};
#else
        return {engine::utils::enums::graphics::ShaderLanguage::GLSL,
                engine::utils::enums::graphics::ShaderLanguage::GLSL_ES};
#endif
    }

    utils::Version GLRenderBackend::get_version() {
        const char *versionString = reinterpret_cast<const char *>(glGetString(GL_VERSION));
        GLint major, minor;

        sscanf(versionString, "%d.%d", &major, &minor);
        return {major, minor};
    }

    bool GLRenderBackend::is_mobile() {
#if defined(ENGINE_BACKEND_OPENGL_ES)
        return true;
#else
        return false;
#endif
    }

    bool GLRenderBackend::init() {
#if false
#if defined(ENGINE_BACKEND_OPENGL_ES)
        int ret = gladLoaderLoadGLES2();
#else
        int ret = gladLoadGL((GLADloadfunc) gl_loader);
#endif

        if (!ret) {
            DEBUG_MSG("Failed to initialize OpenGL through glad!\n");
            return false;
        }
#endif

        auto version = get_version();

        DEBUG_MSG("OpenGL %d.%d (%s) loaded!\n", version.major, version.minor, glGetString(GL_VENDOR));
        DEBUG_MSG("Renderer: %s\n", glGetString(GL_RENDERER))
        DEBUG_MSG("Extensions: %s\n", glGetString(GL_EXTENSIONS))

//#if defined(GL_DEBUG_OUTPUT_KHR)
//        glEnable(GL_DEBUG_OUTPUT_KHR);
////        glDebugMessageCallbackKHR(message_callback, nullptr);
//#else
//        glEnable(GL_DEBUG_OUTPUT);
//        glDebugMessageCallback(message_callback, nullptr);
//#endif

        return true;
    }

    void GLRenderBackend::set_viewport_size(const utils::IVector2 &size) {
        glViewport(0, 0, size.x, size.y);
    }

    void GLRenderBackend::set_clear_color(const utils::Color &color) {
        glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
    }

    void GLRenderBackend::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    uintptr_t GLRenderBackend::create_buffer() {
        GLuint buf_id = -1;
        glGenBuffers(1, &buf_id);

        if (buf_id != -1) {
            debug::get_graphics_debug()->vbo_count++;
        }

        return buf_id;
    }

    void GLRenderBackend::delete_buffer(uintptr_t id) {
        auto buf = (GLuint) id;
        glDeleteBuffers(1, &buf);

        debug::get_graphics_debug()->vbo_count--;
    }

    void GLRenderBackend::bind_buffer(uintptr_t id, utils::enums::graphics::BufferTarget target) {
        GLenum buffer_target = buffer_target_mapper(target);

        if (buffer_target == GL_NONE) {
            throw std::runtime_error("invalid buffer target!");
        }

        glBindBuffer(buffer_target, id);
    }

    void
    GLRenderBackend::set_buffer_data(utils::enums::graphics::BufferTarget target, size_t buffer_size, size_t type_size,
                                     void *buf) {
        GLenum buffer_target = buffer_target_mapper(target);

        if (buffer_target == GL_NONE) {
            throw std::runtime_error("invalid buffer target!");
        }

        glBufferData(buffer_target, buffer_size * type_size, buf, GL_STATIC_DRAW);
    }

    void GLRenderBackend::enable_wireframe(bool state) {
#if !defined(ENGINE_BACKEND_OPENGL_ES)
        glPolygonMode(GL_FRONT_AND_BACK, state ? GL_LINE : GL_FILL);
#endif
    }

    void GLRenderBackend::draw_buffer(int vertices_count, utils::enums::graphics::PrimitiveType type) {
        GLenum primitive_type = primitive_type_mapper(type);

        if (primitive_type == GL_NONE) {
            throw std::runtime_error("invalid primitive type!");
        }

        glDrawArrays(primitive_type, 0, vertices_count);

        engine::graphics::debug::get_graphics_debug()->draw_count++;
    }

    uintptr_t GLRenderBackend::create_vertex_buffer() {
        GLuint buf_id = -1;
        glGenVertexArrays(1, &buf_id);

        if (buf_id != -1) {
            debug::get_graphics_debug()->vao_count++;
        }

        return buf_id;
    }

    void GLRenderBackend::delete_vertex_buffer(uintptr_t id) {
        auto array = (GLuint) id;
        glDeleteVertexArrays(1, &array);

        debug::get_graphics_debug()->vao_count--;
    }

    void GLRenderBackend::bind_vertex_buffer(uintptr_t id) {
        GLint current_vao = -1;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);

        if (current_vao != id) {
            glBindVertexArray(id);
        }
    }

    uintptr_t GLRenderBackend::create_shader(utils::enums::graphics::ShaderType type) {
        GLenum shader_type = shader_type_mapper(type);

        if (shader_type == GL_NONE) {
            throw std::runtime_error("invalid shader type!");
        }

        uintptr_t shader_id = glCreateShader(shader_type);
        if (shader_id == 0) shader_id = -1;

        return shader_id;
    }

    void GLRenderBackend::use_shader_source(uintptr_t id, std::string_view source) {
        const char *srcs[1] = {source.data()};
        const GLint sizes[1] = {(GLint) source.size()};

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
        if (id == 0) id = PTR_NULL;

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

    uintptr_t GLRenderBackend::get_program_uniform(uintptr_t program_handle, std::string_view name) {
        return (uintptr_t) glGetUniformLocation(program_handle, name.data());
    }

    void GLRenderBackend::set_uniform_color(uintptr_t uf_handle, const utils::Color &color) {
        glUniform4f((GLint) uf_handle, (float) color.r / 255.f, (float) color.g / 255.f, (float) color.b / 255.f,
                    (float) color.a / 255.f);
    }

    void GLRenderBackend::set_uniform_mat4(uintptr_t uf_handle, const glm::mat4 &mat) {
        glUniformMatrix4fv((GLint) uf_handle, 1, GL_FALSE, &mat[0][0]);
    }

    void GLRenderBackend::set_uniform_int(uintptr_t uf_handle, int val) {
        glUniform1i((GLint) uf_handle, val);
    }

    void GLRenderBackend::define_vertex_attrib(uintptr_t index, utils::enums::graphics::DataType type, int count,
                                               int stride, uintptr_t offset) {
        GLenum data_type = data_type_mapper(type);

        if (data_type == GL_NONE) {
            throw std::runtime_error("invalid data type!");
        }

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, count, data_type, GL_FALSE, stride, (void *) offset);
    }

    uintptr_t GLRenderBackend::create_texture() {
        GLuint tex_id = -1;
        glGenTextures(1, &tex_id);

        if (tex_id != -1) {
            debug::get_graphics_debug()->tex_count++;
        }

        return tex_id;
    }

    void GLRenderBackend::delete_texture(uintptr_t id) {
        auto array = (GLuint) id;
        glDeleteTextures(1, &array);

        debug::get_graphics_debug()->tex_count--;
    }

    void GLRenderBackend::bind_texture(uintptr_t id, uint16_t slot) {
//        ASSERT_MSG(glIsTexture(id), "Invalid texture handle!")
        ASSERT_MSG(slot < 32, "only a max of 32 textures can be bound!")
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, (GLuint) id);
    }

    void
    GLRenderBackend::upload_texture_data(uintptr_t id, const utils::ColorArray &data, const utils::IVector2 &size) {
//        ASSERT_MSG(glIsTexture(id), "Invalid texture handle!")

        GLint current_tex = -1;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_tex);

        ASSERT_MSG(current_tex == id, "Texture not bound!")

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void GLRenderBackend::draw_indexed_buffer(int indices_count, utils::enums::graphics::DataType index_data_type,
                                              utils::enums::graphics::PrimitiveType type) {
        GLenum data_type = data_type_mapper(index_data_type);
        GLenum primitive_type = primitive_type_mapper(type);

        if (data_type == GL_NONE) {
            throw std::runtime_error("invalid data type!");
        }

        if (primitive_type == GL_NONE) {
            throw std::runtime_error("invalid primitive type!");
        }

        glDrawElements(primitive_type, indices_count, data_type, nullptr);

        engine::graphics::debug::get_graphics_debug()->draw_count++;
    }

    void GLRenderBackend::enable_features(uint32_t features) {
        if ((features & utils::enums::graphics::GraphicalBackendFeature::DEPTH_TESTING) > 0) {
            glEnable(GL_DEPTH_TEST);
        }

        if ((features & utils::enums::graphics::GraphicalBackendFeature::TRANSPARENCY_BLENDING) > 0) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

#if defined(GL_DEBUG_OUTPUT_KHR)
        glEnable(GL_DEBUG_OUTPUT_KHR);
#else
        glEnable(GL_DEBUG_OUTPUT);
#endif
    }
}