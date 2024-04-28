#pragma once

#include <utils/vector2.hpp>
#include <utils/vector3.hpp>
#include <utils/color.hpp>

namespace engine::graphics {
#pragma pack(push)
    struct VertexDefinition {
        const engine::utils::enums::graphics::DataType type;
        const int count;

        VertexDefinition(engine::utils::enums::graphics::DataType _type, int _count) : type(_type), count(_count) {}
    };

    template<typename T>
    struct Vertex {
        using VectorType = T;

        T position;
        utils::Color color;
        utils::FVector2 uv;

        Vertex(T _pos, const utils::Color &_col, const utils::FVector2 &_uv) {
            position = _pos;
            color = _col;
            uv = _uv;
        }

        Vertex(T _pos, const utils::Color &_col) : Vertex(_pos, _col, {0, 0}) {}

        static std::vector<VertexDefinition> get_definitions();
    };

#pragma pack(pop)

    using IVertex2D = Vertex<utils::IVector2>;
    using FVertex2D = Vertex<utils::FVector2>;

    using IVertex3D = Vertex<utils::IVector3>;
    using FVertex3D = Vertex<utils::FVector3>;

    // templated definitions
    template<> inline std::vector<VertexDefinition> IVertex2D::get_definitions() {
        return {
                {engine::utils::enums::graphics::DataType::INT,         2}, // attrib 0: position (IVector2 - 2 ints)
                {engine::utils::enums::graphics::DataType::UNSIGNED_BYTE, 4}, // attrib 1: color (Color - 4 ints)
                {engine::utils::enums::graphics::DataType::FLOAT,         2}, // attrib 2: texture UV (FVector2 - 2 floats)
        };
    }

    template<> inline std::vector<VertexDefinition> FVertex2D::get_definitions() {
        return {
                {engine::utils::enums::graphics::DataType::FLOAT,         2}, // attrib 0: position (FVector2 - 2 floats)
                {engine::utils::enums::graphics::DataType::UNSIGNED_BYTE, 4}, // attrib 1: color (Color - 4 ints)
                {engine::utils::enums::graphics::DataType::FLOAT,         2}, // attrib 2: texture UV (FVector2 - 2 floats)
        };
    }

    template<> inline std::vector<VertexDefinition> IVertex3D::get_definitions() {
        return {
                {engine::utils::enums::graphics::DataType::INT,         3}, // attrib 0: position (IVector3 - 3 ints)
                {engine::utils::enums::graphics::DataType::UNSIGNED_BYTE, 4}, // attrib 1: color (Color - 4 ints)
                {engine::utils::enums::graphics::DataType::FLOAT,         2}, // attrib 2: texture UV (FVector2 - 2 floats)
        };
    }

    template<> inline std::vector<VertexDefinition> FVertex3D::get_definitions() {
        return {
                {engine::utils::enums::graphics::DataType::FLOAT,         3}, // attrib 0: position (FVector3 - 3 floats)
                {engine::utils::enums::graphics::DataType::UNSIGNED_BYTE, 4}, // attrib 1: color (Color - 4 ints)
                {engine::utils::enums::graphics::DataType::FLOAT,         2}, // attrib 2: texture UV (FVector2 - 2 floats)
        };
    }
}