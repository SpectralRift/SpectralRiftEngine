#pragma once

#include <graphics/rendering/vertex_buffer.hpp>

namespace engine::graphics {
    template<typename T>
    struct ENGINE_API IndexedVertexBuffer : public VertexBuffer<T> {
        /// <summary>
        /// Constructs an Indexed Vertex Buffer object.
        /// </summary>
        IndexedVertexBuffer() : VertexBuffer<T>() {
            ebo_handle = engine::get_backend()->create_buffer();
        }

        IndexedVertexBuffer(std::initializer_list<T> _vertices, std::initializer_list<int> _indices) : IndexedVertexBuffer(), VertexBuffer<T>(
                _vertices), indices(_indices) {
            if(!indices.empty()) {
                upload_indices_data();
            }
        }

        ~IndexedVertexBuffer() {
            if (ebo_handle != PTR_NULL) {
                engine::get_backend()->delete_buffer(ebo_handle);
            }
        }

        void use() override {
            engine::get_backend()->bind_buffer(ebo_handle, engine::utils::enums::graphics::BufferTarget::ELEMENT_ARRAY_BUFFER);
        }

        void draw(utils::enums::graphics::PrimitiveType primitiveType) {
            use();
            engine::get_backend()->draw_indexed_buffer(indices.size(), engine::utils::enums::graphics::DataType::UNSIGNED_INT, primitiveType);
        }

        void define_indices(const std::vector<int>& _indices) {
            indices = _indices;
            upload_indices_data();
        }

    protected:
        void upload_indices_data() {
            // use vao
            VertexBuffer<T>::use();

            // use this ebo
            use();

            // set indices
            engine::get_backend()->set_buffer_data(engine::utils::enums::graphics::BufferTarget::ELEMENT_ARRAY_BUFFER,
                                                   indices.size(),
                                                   sizeof(T), &indices[0]);
        }

        uintptr_t ebo_handle{PTR_NULL};
        std::vector<int> indices;
    };

    using FIndexedVertexBuffer2D = IndexedVertexBuffer<graphics::FVertex2D>;
    using FIndexedVertexBuffer3D = IndexedVertexBuffer<graphics::FVertex3D>;
}