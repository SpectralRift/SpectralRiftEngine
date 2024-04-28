#pragma once

#include <api.hpp>
#include <engine.hpp>

#include <utils/enums/graphics.hpp>
#include <utils/vector2.hpp>
#include <utils/vector3.hpp>
#include <graphics/rendering/vertex.hpp>

#include <vector>

namespace engine::graphics {
    template<typename T>
    struct ENGINE_API VertexBuffer {
        /// <summary>
        /// Constructs a Vertex Buffer object.
        /// </summary>
        VertexBuffer() {
            vbo_handle = engine::get_backend()->create_buffer();
            vao_handle = engine::get_backend()->create_vertex_buffer();
        }

        VertexBuffer(std::initializer_list<T> _vertices) : vbo_handle(engine::get_backend()->create_buffer()),
                                                           vao_handle(engine::get_backend()->create_vertex_buffer()),
                                                           vertices(_vertices) {
            if (!vertices.empty()) {
                upload_buffer_data();
            }
        }

        ~VertexBuffer() {
            if (vbo_handle != PTR_NULL) {
                engine::get_backend()->delete_buffer(vbo_handle);
            }

            if (vao_handle != PTR_NULL) {
                engine::get_backend()->delete_buffer(vao_handle);
            }
        }

        void define_attributes(const std::vector<VertexDefinition> &definitions, int stride) {
            int id = 0;
            int offset = 0;

            is_attribs_set = true;

            use();

            for (const auto &def: definitions) {
                engine::get_backend()->define_vertex_attrib(id++, def.type, def.count,
                                                            stride, offset);

                offset += getDataTypeSize(def.type) * def.count;
            }
        }

        void clear() {
            vertices.clear();
            upload_buffer_data();
        }

        void add_vertex(T vertex) {
            vertices.push_back(vertex);
            upload_buffer_data();
        }

        void add_vertices(std::initializer_list<T> _vertices) {
            for (auto &v: _vertices) {
                vertices.push_back(v);
            }

            upload_buffer_data();
        }

        const std::vector<T> get_vertices() const {
            return vertices;
        }

        virtual void use() {
            engine::get_backend()->bind_vertex_buffer(vao_handle);
        }

        virtual void draw(utils::enums::graphics::PrimitiveType primitiveType) {
            use();
            engine::get_backend()->draw_buffer(vertices.size(), primitiveType);
        }

    protected:
        void upload_buffer_data() {
            use();
            engine::get_backend()->bind_buffer(vbo_handle, engine::utils::enums::graphics::BufferTarget::ARRAY_BUFFER);
            engine::get_backend()->set_buffer_data(engine::utils::enums::graphics::BufferTarget::ARRAY_BUFFER,
                                                   vertices.size(),
                                                   sizeof(T), &vertices[0]);

            // set buffer attribs after uploading some data
            if (!is_attribs_set) {
                define_attributes(T::get_definitions(), sizeof(T));
            }
        }

        uintptr_t vbo_handle{PTR_NULL};
        uintptr_t vao_handle{PTR_NULL};
        std::vector<T> vertices;
        bool is_attribs_set{false};
    };

    using FVertexBuffer2D = VertexBuffer<graphics::FVertex2D>;
    using FVertexBuffer3D = VertexBuffer<graphics::FVertex3D>;

    using FVertexBuffer2DPtr = std::shared_ptr<VertexBuffer<graphics::FVertex2D>>;
    using FVertexBuffer3DPtr = std::shared_ptr<VertexBuffer<graphics::FVertex3D>>;
}