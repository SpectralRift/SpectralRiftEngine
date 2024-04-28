#pragma once

#include <cstdint>

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
        ARRAY_BUFFER,
        ELEMENT_ARRAY_BUFFER
    };

    enum DataType {
        BYTE,
        UNSIGNED_BYTE,
        INT,
        UNSIGNED_INT,
        FLOAT
    };

    enum PrimitiveType {
        TRIANGLES,
        POINTS,
        LINES
    };

    enum GraphicalBackendFeature : uint32_t {
        TRANSPARENCY_BLENDING = (1 << 0),
        DEPTH_TESTING = (1 << 1),
    };

    inline int getDataTypeSize(DataType _type) {
        switch(_type) {
            case BYTE:
                return sizeof(int8_t);
            case UNSIGNED_BYTE:
                return sizeof(uint8_t);
            case INT:
                return sizeof(int);
            case UNSIGNED_INT:
                return sizeof(unsigned int);
            case FLOAT:
                return sizeof(float);
            default:
                return 0;
        }
    }
}