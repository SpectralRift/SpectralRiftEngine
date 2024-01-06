#include <graphics/rendering/shader.hpp>

#include <engine.hpp>
#include <utils/macros/debug.hpp>

namespace engine::graphics {
    Shader::Shader(engine::utils::enums::graphics::ShaderType shaderType) {
        type = shaderType;
    }

    Shader::Shader(const Shader &other) {
        type = other.type;
        handle = other.handle;
        is_compiled = other.is_compiled;
        sources = other.sources;
    }

    void Shader::destroy() {
        if(handle != PTR_NULL) {
            engine::get_backend()->delete_shader(handle);
            handle = PTR_NULL;
        }
    }

    void Shader::set_source(engine::utils::enums::graphics::ShaderLanguage language, std::string_view source) {
        ASSERT_MSG(!compiled(), "can't set source as the shader was compiled already.")
        sources.insert_or_assign(language, source);
    }

    bool Shader::has_source(engine::utils::enums::graphics::ShaderLanguage language) {
        return sources.contains(language);
    }

    std::string Shader::get_source(engine::utils::enums::graphics::ShaderLanguage language) {
        return sources.at(language);
    }

    uintptr_t Shader::get_handle() const {
        return handle;
    }

    bool Shader::compiled() const {
        return is_compiled;
    }

    bool Shader::compile() {
        // mark it as not compiled.
        is_compiled = false;

        // check if there is a handle, and if not, create it
        if(handle == PTR_NULL) {
            handle = engine::get_backend()->create_shader(type);
        }

        // compile shaders for the supported languages.
        for(auto l : engine::get_backend()->get_supported_shader_languages()) {
            if(!has_source(l)) {
                continue;
            }

            DEBUG_MSG("Compiling shader for lang id %i...\n", l);

            // set the source code of the shader
            engine::get_backend()->use_shader_source(handle, get_source(l));

            // perform the compilation, and if it succeeds, we return out
            // otherwise we show the error in the logs, and we try to compile the rest of the variants;
            if (engine::get_backend()->compile_shader(handle)) {
                DEBUG_MSG("COMPILED SUCCESSFULLY!\n");
                is_compiled = true;

                return true;
            } else {
                DEBUG_MSG("COMPILATION FAILED: %s\n", engine::get_backend()->get_shader_log(handle).c_str());
            }
        }

        return false;
    }
}
