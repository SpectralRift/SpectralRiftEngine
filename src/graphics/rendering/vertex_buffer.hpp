#pragma once

#include <api.hpp>
#include <utils/enums/graphics.hpp>

#include <vector>

namespace engine::graphics {
    template<typename T>
    struct ENGINE_API VertexBuffer {
        /// <summary>
        /// Constructs a Vertex Buffer object.
        /// </summary>
        VertexBuffer() = default;

        void clear();
        void add_vertex(T vertex);

        void use();

        void draw(utils::enums::graphics::PrimitiveType primitiveType);

    private:
        uintptr_t vbo_handle{PTR_NULL};
        uintptr_t vao_handle{PTR_NULL};
        std::vector<T> vertices;

        static uintptr_t last_vao_handle = PTR_NULL;
    };


}