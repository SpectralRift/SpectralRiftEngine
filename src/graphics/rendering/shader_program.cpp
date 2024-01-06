#include <graphics/rendering/shader_program.hpp>

#include <engine.hpp>
#include <utils/macros/debug.hpp>

namespace engine::graphics {
    ShaderProgram::ShaderProgram(const ShaderProgram& other) {
        handle = other.handle;
        is_linked = other.is_linked;

        shaders.reserve(other.shaders.size());
        for (const auto& shader : other.shaders) {
            shaders.push_back(shader);
        }
    }

    ShaderProgram::ShaderProgram(std::initializer_list<std::reference_wrapper<Shader>> sh) {
        shaders.reserve(sh.size());

        for(Shader& shd : sh) {
            add_shader(shd);
        }
    }

    void ShaderProgram::add_shader(Shader& shader) {
        ASSERT_MSG(!linked(), "can't add a new shader as this shader program was linked already.")
        shaders.emplace_back(shader);
    }

    ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other) {
        if (this != &other) {
            handle = other.handle;
            is_linked = other.is_linked;

            // Clear existing shaders and make copies of shaders from 'other'
            shaders.clear();
            shaders.reserve(other.shaders.size());
            for (Shader& shader : other.shaders) {
                shaders.emplace_back(shader); // Make a copy of the Shader pointed to by shaderPtr
            }
        }
        return *this;
    }

    bool ShaderProgram::link() {
        // reset linking state
        is_linked = false;

        // checks if there is a handle to our shader program
        // if not, we create one
        if(handle == PTR_NULL) {
            handle = engine::get_backend()->create_shader_program();
        }

        // now we add our shaders to the program
        for(Shader& shader: shaders) {
            // if the shader isn't compiled, try to compile it
            // if one of the shaders fail to compile, we fail the linking.
            if (!shader.compiled() && !shader.compile()) {
                return false;
            }

            engine::get_backend()->attach_shader_to_program(handle, shader.get_handle());
        }

        is_linked = engine::get_backend()->link_program(handle);

        if (!is_linked) {
            DEBUG_MSG("LINKING FAILED: %s\n", engine::get_backend()->get_program_log(handle).c_str());
        }

        return is_linked;
    }

    void ShaderProgram::destroy() {
        DEBUG_MSG("ShaderProgram::destroy\n")
        shaders.clear();
    }

    void ShaderProgram::use() {
        if(is_linked && handle != PTR_NULL) {
            engine::get_backend()->use_shader_program(handle);
        }
    }

    uintptr_t ShaderProgram::get_handle() const {
        return handle;
    }

    bool ShaderProgram::linked() const {
        return is_linked;
    }
}