#include <rendering/renderer_3d.hpp>
#include <graphics/rendering/shader.hpp>
#include <graphics/rendering/shader_program.hpp>

#include <utils/macros/debug.hpp>

namespace engine::rendering {
    static graphics::ShaderProgramPtr renderer_3d_program{nullptr};

    bool renderer_3d_init() {
        if (renderer_3d_program != nullptr) {
            DEBUG_MSG("3D Renderer seems to be already initialised!")
            return false;
        }

        graphics::Shader vertex_shader_3d{engine::utils::enums::graphics::ShaderType::VERTEX};
        graphics::Shader frag_shader_3d{engine::utils::enums::graphics::ShaderType::FRAGMENT};

        renderer_3d_program = std::make_shared<graphics::ShaderProgram>();

        renderer_3d_program->add_shader(vertex_shader_3d);
        renderer_3d_program->add_shader(frag_shader_3d);

        // set shader source
#pragma region Shader Source Codes for 3D Rendering
        vertex_shader_3d.set_source(engine::utils::enums::graphics::ShaderLanguage::GLSL,
                                    "#version 330 core\n"
                                    "layout (location = 0) in vec3 sPosition;\n"
                                    "layout (location = 1) in vec4 sColor;\n"
                                    "layout (location = 2) in vec2 sTexUV;\n"
                                    "uniform mat4 sModelMat;\n"
                                    "uniform mat4 sViewMat;\n"
                                    "uniform mat4 sProjMat;\n"
                                    "out vec4 OutColor;\n"
                                    "out vec2 TexCoord;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   gl_Position = sProjMat * sViewMat * sModelMat * vec4(sPosition, 1.0);\n"
                                    "   OutColor = sColor / vec4(255, 255, 255, 255);\n"
                                    "   TexCoord = vec2(sTexUV.x, 1.0 - sTexUV.y);\n"
                                    "}"
        );

        frag_shader_3d.set_source(engine::utils::enums::graphics::ShaderLanguage::GLSL,
                                  "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "in vec4 OutColor;\n"
                                  "in vec2 TexCoord;\n"
                                  "uniform sampler2D sTexture;\n"
                                  "void main()\n"
                                  "{\n"
                                  //                                                      "	FragColor = OutColor;\n"
                                  "	FragColor = texture(sTexture, TexCoord) * OutColor;\n"
                                  "}"
        );

        vertex_shader_3d.set_source(engine::utils::enums::graphics::ShaderLanguage::GLSL_ES,
                                    "#version 300 es\n"
                                    "precision highp float;\n"
                                    "layout (location = 0) in vec3 sPosition;\n"
                                    "layout (location = 1) in vec4 sColor;\n"
                                    "layout (location = 2) in vec2 sTexUV;\n"
                                    "uniform mat4 sModelMat;\n"
                                    "uniform mat4 sViewMat;\n"
                                    "uniform mat4 sProjMat;\n"
                                    "out vec4 OutColor;\n"
                                    "out vec2 TexCoord;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   gl_Position = sProjMat * sViewMat * sModelMat * vec4(sPosition, 1.0);\n"
                                    "   OutColor = sColor / vec4(255, 255, 255, 255);\n"
                                    "   TexCoord = vec2(sTexUV.x, 1.0 - sTexUV.y);\n"
                                    "}"
        );

        frag_shader_3d.set_source(engine::utils::enums::graphics::ShaderLanguage::GLSL_ES,
                                  "#version 300 es\n"
                                  "precision highp float;\n"
                                  "out vec4 FragColor;\n"
                                  "in vec4 OutColor;\n"
                                  "in vec2 TexCoord;\n"
                                  "uniform sampler2D sTexture;\n"
                                  "void main()\n"
                                  "{\n"
                                  //                                                      "	FragColor = OutColor;\n"
                                  "	FragColor = texture(sTexture, TexCoord) * OutColor;\n"
                                  "}"
        );
#pragma endregion

        if (!renderer_3d_program->link()) {
            DEBUG_MSG("Failed to link the 3D Renderer Shader Program!")

            vertex_shader_3d.destroy();
            frag_shader_3d.destroy();

            return false;
        }

        vertex_shader_3d.destroy();
        frag_shader_3d.destroy();

        return true;
    }

    void renderer_3d_setup_camera(const rendering::Camera& camera) {
        if(renderer_3d_program == nullptr) {
            DEBUG_MSG("CANNOT SET CAMERA MTX! renderer_3d_program IS NULL!")
            return;
        }

        renderer_3d_program->use();
        renderer_3d_program->set_uniform("sProjMat", camera.get_projection());
        renderer_3d_program->set_uniform("sViewMat", camera.get_view());
    }

    void renderer_3d_setup_model_mtx(const glm::mat4& model_mtx) {
        renderer_3d_program->use();
        renderer_3d_program->set_uniform("sModelMat", model_mtx);
    }

    void renderer_3d_shutdown() {
        if (renderer_3d_program != nullptr) {
            renderer_3d_program->destroy();
            renderer_3d_program = nullptr;
        }
    }
}