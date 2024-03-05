#include <algorithm>

#include <rendering/camera_3d.hpp>

#include <glm/gtx/transform.hpp>

namespace engine::rendering {
    void Camera3D::update() {
        // clamp pitch to prevent clipping
//        rotation.x = std::clamp(rotation.x, -90.0f, 90.0f);
        rotation.y = std::clamp(rotation.y, -90.0f, 90.0f);

        m_projection = glm::perspective(glm::radians(fov), (float) viewport.x / (float) viewport.y, near_clip, far_clip);

        glm::vec3 _front{
                cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)),
                sin(glm::radians(rotation.y)),
                sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y))
        };

        auto front = glm::normalize(_front);
        auto right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
        auto up = glm::normalize(glm::cross(right, front));

        m_view = glm::lookAt(position.to_glm_vec(), position.to_glm_vec() + front, up);
    }
}