#pragma once

#include <unordered_map>
#include <string>
#include <string_view>

#include <api.hpp>

#include <utils/enums/graphics.hpp>

namespace engine::graphics {
    /// <summary>
    /// Represents a shader in a graphical application.
    /// </summary>
    struct ENGINE_API Shader {
        /// <summary>
        /// Constructs a Shader object.
        /// </summary>
        Shader() = default;

        /// <summary>
        /// Constructs a Shader object of a specific type.
        /// </summary>
        /// <param name="shaderType">Type of shader to create.</param>
        explicit Shader(engine::utils::enums::graphics::ShaderType shaderType);

        /// <summary>
        /// Constructs a Shader object by copying another Shader.
        /// </summary>
        /// <param name="other">Another Shader to be copied.</param>
        Shader(const Shader& other);

        /// <summary>
        /// Destroys the Shader object and its associated resources.
        /// </summary>
        ~Shader() {
            destroy();
        }

        /// <summary>
        /// Sets the source code for the shader in a specified language.
        /// </summary>
        /// <param name="language">Shader language.</param>
        /// <param name="source">Source code to set.</param>
        void set_source(engine::utils::enums::graphics::ShaderLanguage language, std::string_view source);

        /// <summary>
        /// Retrieves the source code of the shader in a specified language.
        /// </summary>
        /// <param name="language">Shader language.</param>
        /// <returns>Source code of the shader in the specified language.</returns>
        std::string get_source(engine::utils::enums::graphics::ShaderLanguage language);

        /// <summary>
        /// Checks if the shader has source code in the specified language.
        /// </summary>
        /// <param name="language">Shader language.</param>
        /// <returns>True if the shader has source code in the specified language; otherwise, false.</returns>
        bool has_source(engine::utils::enums::graphics::ShaderLanguage language);

        /// <summary>
        /// Compiles the shader.
        /// </summary>
        /// <returns>True if compilation was successful; otherwise, false.</returns>
        bool compile();

        /// <summary>
        /// Destroys the shader.
        /// </summary>
        void destroy();

        /// <summary>
        /// Retrieves a handle to the shader used by the backend.
        /// </summary>
        /// <returns>Handle to the shader.</returns>
        uintptr_t get_handle() const;

        /// <summary>
        /// Checks if the shader has been successfully compiled.
        /// </summary>
        /// <returns>True if the shader is compiled; otherwise, false.</returns>
        bool compiled() const;

    private:
        /// <summary>
        /// Type of shader.
        /// </summary>
        engine::utils::enums::graphics::ShaderType type;

        /// <summary>
        /// Handle to the shader used by the graphical backend.
        /// </summary>
        uintptr_t handle{PTR_NULL};

        /// <summary>
        /// Indicates whether the shader has been successfully compiled.
        /// </summary>
        bool is_compiled{false};

        /// <summary>
        /// Map of source code for different shader languages.
        /// </summary>
        std::unordered_map<engine::utils::enums::graphics::ShaderLanguage, std::string> sources{};
    };
}
