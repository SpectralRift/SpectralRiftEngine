#pragma once

namespace engine::utils::enums::graphics {
    enum ShaderType {
        VERTEX,
        FRAGMENT
    };

    enum ShaderLanguage {
        GLSL,
        GLSL_ES
    };

    enum BufferTarget {
        ARRAY_BUFFER
    };

    enum DataType {
        INT,
        FLOAT
    };

    enum PrimitiveType {
        TRIANGLES,
        POINTS,
        LINES
    };
}