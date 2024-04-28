#include <components/mesh_component.hpp>
#include <rendering/renderer_3d.hpp>

#include <glm/gtx/transform.hpp>
#include <utils/macros/debug.hpp>

namespace engine::components {
    void MeshComponent::render(graphics::FrameRenderContextRef ctx) {
        // if the buffer is not set, we don't render it!
        if(vertex_buffer == nullptr) {
            DEBUG_MSG("MeshComponent does not have a buffer set!")
            return;
        }

        auto model = glm::translate(glm::mat4(1.0f), position.to_glm_vec());
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0, 0.0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0, 1.0f, 0.0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0f));

        vertex_buffer->use();

        if(texture == nullptr) {
            DEBUG_MSG("MeshComponent does not have a texture set; falling back to white pixel")
            texture = graphics::get_white_pixel();
        }

        texture->bind();

        engine::rendering::renderer_3d_setup_model_mtx(model);
        vertex_buffer->draw(primitive_type);
    }
}