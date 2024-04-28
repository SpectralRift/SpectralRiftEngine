#pragma once

#include <vector>
#include <memory>

#include <api.hpp>

#include <utils/enums/graphics.hpp>
#include <utils/color.hpp>
#include <graphics/rendering/shader.hpp>
#include <glm/glm.hpp>

namespace engine::graphics {
    /// <summary>
    /// Manages shaders for compilation, linking, and usage in a graphical application.
    /// </summary>
    struct ENGINE_API ShaderProgram {
        /// <summary>
        /// Constructs a ShaderProgram object.
        /// </summary>
        ShaderProgram() = default;

        /// <summary>
        /// Constructs a ShaderProgram object with a list of shaders.
        /// </summary>
        /// <param name="sh">List of shaders to be included in the program.</param>
        ShaderProgram(std::initializer_list<std::reference_wrapper<Shader>> sh);

        /// <summary>
        /// Constructs a ShaderProgram object by copying another ShaderProgram.
        /// </summary>
        /// <param name="other">Another ShaderProgram to be copied.</param>
        ShaderProgram(const ShaderProgram &other);

        /// <summary>
        /// Copy assignment operator for ShaderProgram.
        /// </summary>
        /// <param name="other">Another ShaderProgram to copy.</param>
        /// <returns>Reference to the assigned ShaderProgram.</returns>
        ShaderProgram& operator=(const ShaderProgram& other);

        /// <summary>
        /// Destroys the ShaderProgram object and its associated resources.
        /// </summary>
        ~ShaderProgram() {
            destroy();
        }

        /// <summary>
        /// Adds a shader to the program for linking.
        /// </summary>
        /// <param name="shader">Shader to be added.</param>
        void add_shader(Shader& shader);

        /// <summary>
        /// Performs linking of shaders resulting in a program.
        /// </summary>
        /// <returns>true if linking was successful; otherwise, false.</returns>
        bool link();

        /// <summary>
        /// Destroys the shader program.
        /// </summary>
        void destroy();

        /// <summary>
        /// Sets this shader program as the currently used one.
        /// </summary>
        void use();

        void set_uniform(std::string_view name, const utils::Color& color);
        void set_uniform(std::string_view name, const glm::mat4& mat);
        void set_uniform(std::string_view name, int val);

        /// <summary>
        /// Retrieves a handle to the shader program used by the backend.
        /// </summary>
        /// <returns>Handle to the shader program.</returns>
        uintptr_t get_handle() const;

        /// <summary>
        /// Checks if the shader program has been successfully linked.
        /// </summary>
        /// <returns>true if the program is linked; otherwise, false.</returns>
        bool linked() const;
    private:
        /// <summary>
        /// Handle to the shader program used by the graphical backend.
        /// </summary>
        uintptr_t handle{PTR_NULL};

        /// <summary>
        /// Indicates whether the shader program has been successfully linked.
        /// </summary>
        bool is_linked{false};

        /// <summary>
        /// Collection of shaders added to the program.
        /// </summary>
        std::vector<std::reference_wrapper<Shader>> shaders;
    };

    using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
}
