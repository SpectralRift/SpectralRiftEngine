#include <loaders/model/yoyo_gm_model_loader.hpp>
#include <io/string_stream.hpp>
#include <utils/macros/debug.hpp>

#include <sstream>

namespace engine::loaders {
    enum YoyoD3DCommand {
        // d3d_model_primitive_begin(m, kind)
        D3D_COMMAND_PRIMITIVE_BEGIN,

        // d3d_model_primitive_end(m, kind)
        D3D_COMMAND_PRIMITIVE_END,

        // d3d_model_vertex(m, x, y, z)
        D3D_COMMAND_VERTEX,

        // d3d_model_vertex_color(m, x, y, z, color, alpha)
        D3D_COMMAND_VERTEX_COLOR,

        // d3d_model_vertex_texture(m, x, y, z, xtex, ytex)
        D3D_COMMAND_VERTEX_TEXTURE,

        // d3d_model_vertex_texture_color(m, x, y, z, xtex, ytex, color, alpha)
        D3D_COMMAND_VERTEX_TEXTURE_COLOR,

        // d3d_model_vertex_normal(m, x, y, z, nx, ny, nz)
        D3D_COMMAND_VERTEX_NORMAL,

        // d3d_model_vertex_normal_color(m, x, y, z, nx, ny, nz, color, alpha)
        D3D_COMMAND_VERTEX_NORMAL_COLOR,

        // d3d_model_vertex_normal_texture(m, x, y, z, nx, ny, nz, xtex, ytex)
        D3D_COMMAND_VERTEX_NORMAL_TEXTURE,

        // d3d_model_vertex_normal_texture_color(m, x, y, z, nx, ny, nz, xtex, ytex, color, alpha)
        D3D_COMMAND_VERTEX_NORMAL_TEXTURE_COLOR
    };

    utils::Color yoyo_bgr_to_color(float bgr, float alpha) {
        uint32_t col = static_cast<uint32_t>(bgr);

        // unpack BGR into colors
        uint8_t b = (col & 0xFF);
        uint8_t g = ((col >> 8) & 0xFF);
        uint8_t r = ((col >> 16) & 0xFF);

        return {r, g, b, (uint8_t) (alpha * 255)};
    }

    std::shared_ptr<engine::components::MeshComponent> yoyo_gm_model_load(const io::IStreamSPtr &stream) {
        io::StringStream data{stream};
        std::stringstream ss(data.read_all());
        std::string line;

        auto buffer = std::make_shared<graphics::FVertexBuffer3D>();
        auto mesh = std::make_shared<engine::components::MeshComponent>();

        int version = -1, entries = -1, entries_cnt = 0;

        while (std::getline(ss, line)) {
            std::stringstream line_ss(line);

            // if we didn't read the version, we try to read it first thing
            // as the first line contains the version
            if (version == -1) {
                line_ss >> version;

                DEBUG_MSG("YoYo GameMaker D3D version %i\n", version)
                if (version != 100) {
                    DEBUG_MSG("Invalid YoYo GameMaker D3D file version! %i\n", version)
                    return nullptr;
                }
            } else if (entries == -1) {
                // if we got the version, get the metadata
                std::string magic;
                line_ss >> entries >> magic;
                entries_cnt = entries;

                if (magic != "GMMC") {
                    DEBUG_MSG("Invalid YoYo GameMaker D3D Magic!\n")
                    return nullptr;
                }
            } else {
                if (entries-- == 0) {
                    break;
                }

                int command = -1;
                float x, y, z, nx, ny, nz, uv_x, uv_y, color, alpha;

                // one param is command
                // 10 more params mean many things
                line_ss >> command;
                line_ss >> x >> y >> z >> nx >> ny >> nz >> uv_x >> uv_y >> color >> alpha;

                switch (command) {
                    case D3D_COMMAND_PRIMITIVE_BEGIN:
                    case D3D_COMMAND_PRIMITIVE_END:
//                    DEBUG_MSG("D3D_COMMAND_PRIMITIVE %i\n", (int) x)
                        break;
                    case D3D_COMMAND_VERTEX:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX\n")
                        break;
                    case D3D_COMMAND_VERTEX_COLOR:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX_COLOR\n")
                        break;
                    case D3D_COMMAND_VERTEX_TEXTURE:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX_TEXTURE\n")
                        break;
                    case D3D_COMMAND_VERTEX_TEXTURE_COLOR:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX_TEXTURE_COLOR\n")
                        break;
                    case D3D_COMMAND_VERTEX_NORMAL:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX_NORMAL\n")
                        break;
                    case D3D_COMMAND_VERTEX_NORMAL_COLOR:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX_NORMAL_COLOR\n")
                        break;
                    case D3D_COMMAND_VERTEX_NORMAL_TEXTURE:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX_NORMAL_TEXTURE\n")
                        break;
                    case D3D_COMMAND_VERTEX_NORMAL_TEXTURE_COLOR:
//                    DEBUG_MSG("D3D_COMMAND_VERTEX_NORMAL_TEXTURE_COLOR\n")
                        buffer->add_vertex({
                                                   {x, y, z},
                                                   yoyo_bgr_to_color(color, alpha),
                                                   {uv_x, uv_y}

                                           });
                        break;
                    default:
                        break;
                }

//                DEBUG_MSG("Vector '%s'\n", line.data())
            }
        }

        if (version == -1 || entries == -1) {
            DEBUG_MSG("Missing GameMaker Model Creator Metadata!\n")
            return nullptr;
        }

        mesh->vertex_buffer = buffer;

        return mesh;
    }
}