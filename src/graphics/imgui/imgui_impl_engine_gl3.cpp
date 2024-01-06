// dear imgui: Renderer Backend for modern OpenGL with shaders / programmatic pipeline
// - Desktop GL: 2.x 3.x 4.x
// - Embedded GL: ES 2.0 (WebGL 1.0), ES 3.0 (WebGL 2.0)
// This needs to be used along with a Platform Backend (e.g. GLFW, SDL, Win32, custom..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID!
//  [x] Renderer: Large meshes support (64k+ vertices) with 16-bit indices (Desktop OpenGL only).

// About WebGL/ES:
// - You need to '#define IMGUI_IMPL_OPENGL_ES2' or '#define IMGUI_IMPL_OPENGL_ES3' to use WebGL or OpenGL ES.
// - This is done automatically on iOS, Android and Emscripten targets.
// - For other targets, the define needs to be visible from the imgui_impl_opengl3.cpp compilation unit. If unsure, define globally or in imconfig.h.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2023-11-08: OpenGL: Update GL3W based imgui_impl_opengl3_loader.h to load "libGL.so" instead of "libGL.so.1", accomodating for NetBSD systems having only "libGL.so.3" available. (#6983)
//  2023-10-05: OpenGL: Rename symbols in our internal loader so that LTO compilation with another copy of gl3w is possible. (#6875, #6668, #4445)
//  2023-06-20: OpenGL: Fixed erroneous use glGetIntegerv(GL_CONTEXT_PROFILE_MASK) on contexts lower than 3.2. (#6539, #6333)
//  2023-05-09: OpenGL: Support for glBindSampler() backup/restore on ES3. (#6375)
//  2023-04-18: OpenGL: Restore front and back polygon mode separately when supported by context. (#6333)
//  2023-03-23: OpenGL: Properly restoring "no shader program bound" if it was the case prior to running the rendering function. (#6267, #6220, #6224)
//  2023-03-15: OpenGL: Fixed GL loader crash when GL_VERSION returns NULL. (#6154, #4445, #3530)
//  2023-03-06: OpenGL: Fixed restoration of a potentially deleted OpenGL program, by calling glIsProgram(). (#6220, #6224)
//  2022-11-09: OpenGL: Reverted use of glBufferSubData(), too many corruptions issues + old issues seemingly can't be reproed with Intel drivers nowadays (revert 2021-12-15 and 2022-05-23 changes).
//  2022-10-11: Using 'nullptr' instead of 'NULL' as per our switch to C++11.
//  2022-09-27: OpenGL: Added ability to '#define IMGUI_IMPL_OPENGL_DEBUG'.
//  2022-05-23: OpenGL: Reworking 2021-12-15 "Using buffer orphaning" so it only happens on Intel GPU, seems to cause problems otherwise. (#4468, #4825, #4832, #5127).
//  2022-05-13: OpenGL: Fixed state corruption on OpenGL ES 2.0 due to not preserving GL_ELEMENT_ARRAY_BUFFER_BINDING and vertex attribute states.
//  2021-12-15: OpenGL: Using buffer orphaning + glBufferSubData(), seems to fix leaks with multi-viewports with some Intel HD drivers.
//  2021-08-23: OpenGL: Fixed ES 3.0 shader ("#version 300 es") use normal precision floats to avoid wobbly rendering at HD resolutions.
//  2021-08-19: OpenGL: Embed and use our own minimal GL loader (imgui_impl_opengl3_loader.h), removing requirement and support for third-party loader.
//  2021-06-29: Reorganized backend to pull data from a single structure to facilitate usage with multiple-contexts (all g_XXXX access changed to bd->XXXX).
//  2021-06-25: OpenGL: Use OES_vertex_array extension on Emscripten + backup/restore current state.
//  2021-06-21: OpenGL: Destroy individual vertex/fragment shader objects right after they are linked into the main shader.
//  2021-05-24: OpenGL: Access GL_CLIP_ORIGIN when "GL_ARB_clip_control" extension is detected, inside of just OpenGL 4.5 version.
//  2021-05-19: OpenGL: Replaced direct access to ImDrawCmd::TextureId with a call to ImDrawCmd::GetTexID(). (will become a requirement)
//  2021-04-06: OpenGL: Don't try to read GL_CLIP_ORIGIN unless we're OpenGL 4.5 or greater.
//  2021-02-18: OpenGL: Change blending equation to preserve alpha in output buffer.
//  2021-01-03: OpenGL: Backup, setup and restore GL_STENCIL_TEST state.
//  2020-10-23: OpenGL: Backup, setup and restore GL_PRIMITIVE_RESTART state.
//  2020-10-15: OpenGL: Use glGetString(GL_VERSION) instead of glGetIntegerv(GL_MAJOR_VERSION, ...) when the later returns zero (e.g. Desktop GL 2.x)
//  2020-09-17: OpenGL: Fix to avoid compiling/calling glBindSampler() on ES or pre 3.3 context which have the defines set by a loader.
//  2020-07-10: OpenGL: Added support for glad2 OpenGL loader.
//  2020-05-08: OpenGL: Made default GLSL version 150 (instead of 130) on OSX.
//  2020-04-21: OpenGL: Fixed handling of glClipControl(GL_UPPER_LEFT) by inverting projection matrix.
//  2020-04-12: OpenGL: Fixed context version check mistakenly testing for 4.0+ instead of 3.2+ to enable ImGuiBackendFlags_RendererHasVtxOffset.
//  2020-03-24: OpenGL: Added support for glbinding 2.x OpenGL loader.
//  2020-01-07: OpenGL: Added support for glbinding 3.x OpenGL loader.
//  2019-10-25: OpenGL: Using a combination of GL define and runtime GL version to decide whether to use glDrawElementsBaseVertex(). Fix building with pre-3.2 GL loaders.
//  2019-09-22: OpenGL: Detect default GL loader using __has_include compiler facility.
//  2019-09-16: OpenGL: Tweak initialization code to allow application calling ImGui_ImplEngineGL3_CreateFontsTexture() before the first NewFrame() call.
//  2019-05-29: OpenGL: Desktop GL only: Added support for large mesh (64K+ vertices), enable ImGuiBackendFlags_RendererHasVtxOffset flag.
//  2019-04-30: OpenGL: Added support for special ImDrawCallback_ResetRenderState callback to reset render state.
//  2019-03-29: OpenGL: Not calling glBindBuffer more than necessary in the render loop.
//  2019-03-15: OpenGL: Added a GL call + comments in ImGui_ImplEngineGL3_Init() to detect uninitialized GL function loaders early.
//  2019-03-03: OpenGL: Fix support for ES 2.0 (WebGL 1.0).
//  2019-02-20: OpenGL: Fix for OSX not supporting OpenGL 4.5, we don't try to read GL_CLIP_ORIGIN even if defined by the headers/loader.
//  2019-02-11: OpenGL: Projecting clipping rectangles correctly using draw_data->FramebufferScale to allow multi-viewports for retina display.
//  2019-02-01: OpenGL: Using GLSL 410 shaders for any version over 410 (e.g. 430, 450).
//  2018-11-30: Misc: Setting up io.BackendRendererName so it can be displayed in the About Window.
//  2018-11-13: OpenGL: Support for GL 4.5's glClipControl(GL_UPPER_LEFT) / GL_CLIP_ORIGIN.
//  2018-08-29: OpenGL: Added support for more OpenGL loaders: glew and glad, with comments indicative that any loader can be used.
//  2018-08-09: OpenGL: Default to OpenGL ES 3 on iOS and Android. GLSL version default to "#version 300 ES".
//  2018-07-30: OpenGL: Support for GLSL 300 ES and 410 core. Fixes for Emscripten compilation.
//  2018-07-10: OpenGL: Support for more GLSL versions (based on the GLSL version string). Added error output when shaders fail to compile/link.
//  2018-06-08: Misc: Extracted imgui_impl_opengl3.cpp/.h away from the old combined GLFW/SDL+OpenGL3 examples.
//  2018-06-08: OpenGL: Use draw_data->DisplayPos and draw_data->DisplaySize to setup projection matrix and clipping rectangle.
//  2018-05-25: OpenGL: Removed unnecessary backup/restore of GL_ELEMENT_ARRAY_BUFFER_BINDING since this is part of the VAO state.
//  2018-05-14: OpenGL: Making the call to glBindSampler() optional so 3.2 context won't fail if the function is a nullptr pointer.
//  2018-03-06: OpenGL: Added const char* glsl_version parameter to ImGui_ImplEngineGL3_Init() so user can override the GLSL version e.g. "#version 150".
//  2018-02-23: OpenGL: Create the VAO in the render function so the setup can more easily be used with multiple shared GL context.
//  2018-02-16: Misc: Obsoleted the io.RenderDrawListsFn callback and exposed ImGui_ImplSdlGL3_RenderDrawData() in the .h file so you can call it yourself.
//  2018-01-07: OpenGL: Changed GLSL shader version from 330 to 150.
//  2017-09-01: OpenGL: Save and restore current bound sampler. Save and restore current polygon mode.
//  2017-05-01: OpenGL: Fixed save and restore of current blend func state.
//  2017-05-01: OpenGL: Fixed save and restore of current GL_ACTIVE_TEXTURE.
//  2016-09-05: OpenGL: Fixed save and restore of current scissor rectangle.
//  2016-07-29: OpenGL: Explicitly setting GL_UNPACK_ROW_LENGTH to reduce issues because SDL changes it. (#752)

//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120       "#version 120"
//  3.0       130       "#version 130"
//  3.1       140       "#version 140"
//  3.2       150       "#version 150"
//  3.3       330       "#version 330 core"
//  4.0       400       "#version 400 core"
//  4.1       410       "#version 410 core"
//  4.2       420       "#version 410 core"
//  4.3       430       "#version 430 core"
//  ES 2.0    100       "#version 100"      = WebGL 1.0
//  ES 3.0    300       "#version 300 es"   = WebGL 2.0
//----------------------------------------

#include <graphics/imgui/imgui_impl_engine_gl3.hpp>
#include <engine.hpp>
#include <utils/macros/debug.hpp>
#include <graphics/rendering/shader.hpp>
#include <graphics/rendering/shader_program.hpp>

#include <string>
#include <memory>

#if defined(ENGINE_BACKEND_OPENGL_ES)

#include <glad/gles2.h>

#else

#include "glad/gl.h"

#endif

namespace engine::graphics::imgui {
    struct ImGui_ImplEngineGL3_Data {
        GLuint GlVersion;               // Extracted at runtime using GL_MAJOR_VERSION, GL_MINOR_VERSION queries (e.g. 320 for GL 3.2)
        bool IsMobile; // represents whether we are using a mobile/embedded graphics library
        bool GlProfileIsES2;
        bool GlProfileIsES3;
        bool GlProfileIsCompat;
        GLint GlProfileMask;
        GLuint FontTexture;
        std::unique_ptr<ShaderProgram> shaderPtr;
        GLint AttribLocationTex;       // Uniforms location
        GLint AttribLocationProjMtx;
        GLuint AttribLocationVtxPos;    // Vertex attributes location
        GLuint AttribLocationVtxUV;
        GLuint AttribLocationVtxColor;
        unsigned int VboHandle, ElementsHandle;
        GLsizeiptr VertexBufferSize;
        GLsizeiptr IndexBufferSize;
        bool HasClipOrigin;

        ImGui_ImplEngineGL3_Data() { memset((void *) this, 0, sizeof(*this)); }
    };

    struct ImGui_ImplEngineGL3_VtxAttribState {
        GLint Enabled, Size, Type, Normalized, Stride;
        GLvoid *Ptr;

        void GetState(GLint index) {
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &Enabled);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &Size);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_TYPE, &Type);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &Normalized);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &Stride);
            glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER, &Ptr);
        }

        void SetState(GLint index) {
            glVertexAttribPointer(index, Size, Type, (GLboolean) Normalized, Stride, Ptr);
            if (Enabled) glEnableVertexAttribArray(index); else glDisableVertexAttribArray(index);
        }
    };

    static ImGui_ImplEngineGL3_Data *ImGui_ImplEngineGL3_GetBackendData() {
        return ImGui::GetCurrentContext()
               ? (ImGui_ImplEngineGL3_Data *) ImGui::GetIO().BackendRendererUserData
               : nullptr;
    }

    bool ImGui_ImplEngineGL3_Init() {
        ImGuiIO &io = ImGui::GetIO();
        IM_ASSERT(
                io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");

        // Setup backend capabilities flags
        auto bd = IM_NEW(ImGui_ImplEngineGL3_Data)();
        io.BackendRendererUserData = (void *) bd;
        io.BackendRendererName = "imgui_impl_engine_gl3";

        // Query for GL version (e.g. 320 for GL 3.2)
        bd->GlVersion = engine::get_backend()->get_version().pack_version_simple();
        bd->IsMobile = engine::get_backend()->is_mobile();

        DEBUG_MSG(
                "GlVersion = %d\nGlProfileIsCompat = %d\nGlProfileMask = 0x%X\nGlProfileIsES2 = %d, GlProfileIsES3 = %d\nGL_VENDOR = '%s'\nGL_RENDERER = '%s'\n",
                bd->GlVersion, bd->GlProfileIsCompat, bd->GlProfileMask, bd->GlProfileIsES2,
                bd->GlProfileIsES3,
                (const char *) glGetString(GL_VENDOR),
                (const char *) glGetString(GL_RENDERER)); // [DEBUG]

        if (bd->GlVersion >= 320) {
            io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
        }

        // Detect extensions we support
        bd->HasClipOrigin = (bd->GlVersion >= 450);
        GLint num_extensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
        for (GLint i = 0; i < num_extensions; i++) {
            const char *extension = (const char *) glGetStringi(GL_EXTENSIONS, i);
            if (extension != nullptr && strcmp(extension, "GL_ARB_clip_control") == 0)
                bd->HasClipOrigin = true;
        }

        return true;
    }

    bool ImGui_ImplEngineGL3_CreateDeviceObjects() {
        ImGuiIO &io = ImGui::GetIO();
        auto bd = ImGui_ImplEngineGL3_GetBackendData();

        // Backup GL state
        GLint last_texture, last_array_buffer;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
        GLint last_vertex_array;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

        int glsl_version = 130;

        if (bd->IsMobile && bd->GlVersion >= 300) {
            glsl_version = 300; // "#version 300 es\n"
        }

        std::string glsl_version_str{"#version " + std::to_string(glsl_version)};

        if (bd->IsMobile) {
            glsl_version_str += " es";
        }

        glsl_version_str += '\n';

        // Select shaders matching our GLSL versions
        std::string vertex_shader_source;
        std::string fragment_shader_source;

        if (glsl_version < 130) {
            vertex_shader_source = R"(
            uniform mat4 ProjMtx;
            attribute vec2 Position;
            attribute vec2 UV;
            attribute vec4 Color;
            varying vec2 Frag_UV;
            varying vec4 Frag_Color;
            void main()
            {
                Frag_UV = UV;
                Frag_Color = Color;
                gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
            }
        )";
            fragment_shader_source = R"(
            #ifdef GL_ES
                precision mediump float;
            #endif
            uniform sampler2D Texture;
            varying vec2 Frag_UV;
            varying vec4 Frag_Color;
            void main()
            {
                gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);
            }
        )";
        } else if (glsl_version >= 410) {
            vertex_shader_source = R"(
            layout (location = 0) in vec2 Position;
            layout (location = 1) in vec2 UV;
            layout (location = 2) in vec4 Color;
            uniform mat4 ProjMtx;
            out vec2 Frag_UV;
            out vec4 Frag_Color;
            void main()
            {
                Frag_UV = UV;
                Frag_Color = Color;
                gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
            }
        )";
            fragment_shader_source = R"(
            in vec2 Frag_UV;
            in vec4 Frag_Color;
            uniform sampler2D Texture;
            layout (location = 0) out vec4 Out_Color;
            void main()
            {
                Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
            }
        )";
        } else if (glsl_version == 300) {
            vertex_shader_source = R"(
                uniform mat4 ProjMtx;
                layout (location = 0) in vec2 Position;
                layout (location = 1) in vec2 UV;
                layout (location = 2) in vec4 Color;
                out vec2 Frag_UV;
                out vec4 Frag_Color;
                void main()
                {
                    Frag_UV = UV;
                    Frag_Color = Color;
                    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
                }
            )";
            fragment_shader_source = R"(
                precision mediump float;
                uniform sampler2D Texture;
                in vec2 Frag_UV;
                in vec4 Frag_Color;
                layout (location = 0) out vec4 Out_Color;
                void main()
                {
                    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
                }
            )";
        } else {
            vertex_shader_source = R"(
                uniform mat4 ProjMtx;
                in vec2 Position;
                in vec2 UV;
                in vec4 Color;
                out vec2 Frag_UV;
                out vec4 Frag_Color;
                void main()
                {
                    Frag_UV = UV;
                    Frag_Color = Color;
                    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
                }
            )";
            fragment_shader_source = R"(
                uniform sampler2D Texture;
                in vec2 Frag_UV;
                in vec4 Frag_Color;
                out vec4 Out_Color;
                void main()
                {
                    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
                }
            )";
        }

        // append versions
        vertex_shader_source = glsl_version_str + vertex_shader_source;
        fragment_shader_source = glsl_version_str + fragment_shader_source;

        // Create Shader objects
        Shader vertexShader{engine::utils::enums::graphics::ShaderType::VERTEX};
        Shader fragmentShader{engine::utils::enums::graphics::ShaderType::FRAGMENT};

        vertexShader.set_source(
                bd->IsMobile ? engine::utils::enums::graphics::ShaderLanguage::GLSL_ES
                             : engine::utils::enums::graphics::ShaderLanguage::GLSL,
                vertex_shader_source);
        fragmentShader.set_source(
                bd->IsMobile ? engine::utils::enums::graphics::ShaderLanguage::GLSL_ES
                             : engine::utils::enums::graphics::ShaderLanguage::GLSL,
                fragment_shader_source);

        if (!vertexShader.compile() || !fragmentShader.compile()) {
            DEBUG_MSG("VERTEX/FRAGMENT COMPILE ERROR!\n")
            return false;
        }

        // Create ShaderProgram
        bd->shaderPtr = std::make_unique<ShaderProgram>();

        bd->shaderPtr->add_shader(vertexShader);
        bd->shaderPtr->add_shader(fragmentShader);

        // Link ShaderProgram
        if (!bd->shaderPtr->link()) {
            DEBUG_MSG("SHADER LINKING ERROR!\n")
            return false;
        }

        bd->AttribLocationTex = glGetUniformLocation(bd->shaderPtr->get_handle(), "Texture");
        bd->AttribLocationProjMtx = glGetUniformLocation(bd->shaderPtr->get_handle(), "ProjMtx");
        bd->AttribLocationVtxPos = (GLuint) glGetAttribLocation(bd->shaderPtr->get_handle(),
                                                                "Position");
        bd->AttribLocationVtxUV = (GLuint) glGetAttribLocation(bd->shaderPtr->get_handle(), "UV");
        bd->AttribLocationVtxColor = (GLuint) glGetAttribLocation(bd->shaderPtr->get_handle(),
                                                                  "Color");

        // Create buffers
        bd->VboHandle = engine::get_backend()->create_buffer();
        bd->ElementsHandle = engine::get_backend()->create_buffer();

        // Build texture atlas
        unsigned char *pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width,
                                     &height);   // Load as RGBA 32-bit (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

        // Upload texture to graphics system
        // (Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling)
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        glGenTextures(1, &bd->FontTexture);
        glBindTexture(GL_TEXTURE_2D, bd->FontTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     pixels);

        // Store our identifier
        io.Fonts->SetTexID((ImTextureID) (intptr_t) bd->FontTexture);

        // Restore state
        glBindTexture(GL_TEXTURE_2D, last_texture);

        // Restore modified GL state
        glBindTexture(GL_TEXTURE_2D, last_texture);
        glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
        glBindVertexArray(last_vertex_array);

        return true;
    }

    void ImGui_ImplEngineGL3_DestroyDeviceObjects() {
        ImGuiIO &io = ImGui::GetIO();
        auto bd = ImGui_ImplEngineGL3_GetBackendData();

        if (bd->VboHandle) {
            engine::get_backend()->delete_buffer(bd->VboHandle);
            bd->VboHandle = 0;
        }

        if (bd->ElementsHandle) {
            engine::get_backend()->delete_buffer(bd->ElementsHandle);
            bd->ElementsHandle = 0;
        }

        if (bd->shaderPtr->get_handle()) {
            bd->shaderPtr->destroy();
            bd->shaderPtr = nullptr;
        }

        if (bd->FontTexture) {
            glDeleteTextures(1, &bd->FontTexture);
            io.Fonts->SetTexID(0);
            bd->FontTexture = 0;
        }
    }

    void ImGui_ImplEngineGL3_Shutdown() {
        auto bd = ImGui_ImplEngineGL3_GetBackendData();
        IM_ASSERT(bd != nullptr && "No renderer backend to shutdown, or already shutdown?");
        ImGuiIO &io = ImGui::GetIO();

        ImGui_ImplEngineGL3_DestroyDeviceObjects();
        io.BackendRendererName = nullptr;
        io.BackendRendererUserData = nullptr;
        io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
        IM_DELETE(bd);
    }

    void ImGui_ImplEngineGL3_NewFrame() {
        auto bd = ImGui_ImplEngineGL3_GetBackendData();
        IM_ASSERT(bd != nullptr && "Did you call ImGui_ImplEngineGL3_Init()?");

        if (bd->shaderPtr == nullptr)
            ImGui_ImplEngineGL3_CreateDeviceObjects();
    }

    static void
    ImGui_ImplEngineGL3_SetupRenderState(ImDrawData *draw_data, int fb_width, int fb_height,
                                         GLuint vertex_array_object) {
        auto bd = ImGui_ImplEngineGL3_GetBackendData();

        // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glEnable(GL_SCISSOR_TEST);
#if !defined(ENGINE_BACKEND_OPENGL_ES)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

        // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)
        bool clip_origin_lower_left = true;
        if (bd->HasClipOrigin) {
            GLenum current_clip_origin = 0;
            glGetIntegerv(GL_CLIP_ORIGIN, (GLint *) &current_clip_origin);
            if (current_clip_origin == GL_UPPER_LEFT)
                clip_origin_lower_left = false;
        }

        // Setup viewport, orthographic projection matrix
        // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
        glViewport(0, 0, (GLsizei) fb_width, (GLsizei) fb_height);
        float L = draw_data->DisplayPos.x;
        float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        float T = draw_data->DisplayPos.y;
        float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
        if (!clip_origin_lower_left) {
            float tmp = T;
            T = B;
            B = tmp;
        } // Swap top and bottom if origin is upper left

        const float ortho_projection[4][4] =
                {
                        {2.0f / (R - L),    0.0f,              0.0f,  0.0f},
                        {0.0f,              2.0f / (T - B),    0.0f,  0.0f},
                        {0.0f,              0.0f,              -1.0f, 0.0f},
                        {(R + L) / (L - R), (T + B) / (B - T), 0.0f,  1.0f},
                };

        glUseProgram(bd->shaderPtr->get_handle());
        glUniform1i(bd->AttribLocationTex, 0);
        glUniformMatrix4fv(bd->AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

        if (bd->GlVersion >= 330 || bd->GlProfileIsES3)
            glBindSampler(0,
                          0); // We use combined texture/sampler state. Applications using GL 3.3 and GL ES 3.0 may set that otherwise.
        glBindVertexArray(vertex_array_object);

        // Bind vertex/index buffers and setup attributes for ImDrawVert
        glBindBuffer(GL_ARRAY_BUFFER, bd->VboHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bd->ElementsHandle);
        glEnableVertexAttribArray(bd->AttribLocationVtxPos);
        glEnableVertexAttribArray(bd->AttribLocationVtxUV);
        glEnableVertexAttribArray(bd->AttribLocationVtxColor);
        glVertexAttribPointer(bd->AttribLocationVtxPos, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                              (GLvoid *) offsetof(ImDrawVert, pos));
        glVertexAttribPointer(bd->AttribLocationVtxUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                              (GLvoid *) offsetof(ImDrawVert, uv));
        glVertexAttribPointer(bd->AttribLocationVtxColor, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                              sizeof(ImDrawVert),
                              (GLvoid *) offsetof(ImDrawVert, col));
    }

    void ImGui_ImplEngineGL3_Render() {
        ImDrawData *draw_data = ImGui::GetDrawData();

        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int fb_width = (int) (draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
        int fb_height = (int) (draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0)
            return;

        ImGui_ImplEngineGL3_Data *bd = ImGui_ImplEngineGL3_GetBackendData();

        // Backup GL state
        GLenum last_active_texture;
        glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint *) &last_active_texture);
        glActiveTexture(GL_TEXTURE0);
        GLuint last_program;
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *) &last_program);
        GLuint last_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &last_texture);
        GLuint last_sampler;
        if (bd->GlVersion >= 330 || bd->GlProfileIsES3) {
            glGetIntegerv(GL_SAMPLER_BINDING, (GLint *) &last_sampler);
        } else { last_sampler = 0; }
        GLuint last_array_buffer;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *) &last_array_buffer);

        // This is part of VAO on OpenGL 3.0+ and OpenGL ES 3.0+.
        GLint last_element_array_buffer;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
        ImGui_ImplEngineGL3_VtxAttribState last_vtx_attrib_state_pos;
        last_vtx_attrib_state_pos.GetState(bd->AttribLocationVtxPos);
        ImGui_ImplEngineGL3_VtxAttribState last_vtx_attrib_state_uv;
        last_vtx_attrib_state_uv.GetState(bd->AttribLocationVtxUV);
        ImGui_ImplEngineGL3_VtxAttribState last_vtx_attrib_state_color;
        last_vtx_attrib_state_color.GetState(bd->AttribLocationVtxColor);
        GLuint last_vertex_array_object;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint *) &last_vertex_array_object);
#if !defined(ENGINE_BACKEND_OPENGL_ES)
        GLint last_polygon_mode[2];
        glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
#endif
        GLint last_viewport[4];
        glGetIntegerv(GL_VIEWPORT, last_viewport);
        GLint last_scissor_box[4];
        glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
        GLenum last_blend_src_rgb;
        glGetIntegerv(GL_BLEND_SRC_RGB, (GLint *) &last_blend_src_rgb);
        GLenum last_blend_dst_rgb;
        glGetIntegerv(GL_BLEND_DST_RGB, (GLint *) &last_blend_dst_rgb);
        GLenum last_blend_src_alpha;
        glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint *) &last_blend_src_alpha);
        GLenum last_blend_dst_alpha;
        glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint *) &last_blend_dst_alpha);
        GLenum last_blend_equation_rgb;
        glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint *) &last_blend_equation_rgb);
        GLenum last_blend_equation_alpha;
        glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint *) &last_blend_equation_alpha);
        GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
        GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
        GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
        GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
        GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

        // Setup desired GL state
        // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to. VAO are not shared among GL contexts)
        // The renderer would actually work without any VAO bound, but then our VertexAttrib calls would overwrite the default one currently bound.
        GLuint vertex_array_object = 0;
        glGenVertexArrays(1, &vertex_array_object);
        ImGui_ImplEngineGL3_SetupRenderState(draw_data, fb_width, fb_height, vertex_array_object);

        // Will project scissor/clipping rectangles into framebuffer space
        ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
        ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

        // Render command lists
        for (int n = 0; n < draw_data->CmdListsCount; n++) {
            const ImDrawList *cmd_list = draw_data->CmdLists[n];

            // Upload vertex/index buffers
            // - OpenGL drivers are in a very sorry state nowadays....
            //   During 2021 we attempted to switch from glBufferData() to orphaning+glBufferSubData() following reports
            //   of leaks on Intel GPU when using multi-viewports on Windows.
            // - After this we kept hearing of various display corruptions issues. We started disabling on non-Intel GPU, but issues still got reported on Intel.
            // - We are now back to using exclusively glBufferData(). So bd->UseBufferSubData IS ALWAYS FALSE in this code.
            //   We are keeping the old code path for a while in case people finding new issues may want to test the bd->UseBufferSubData path.
            // - See https://github.com/ocornut/imgui/issues/4468 and please report any corruption issues.
            const GLsizeiptr vtx_buffer_size =
                    (GLsizeiptr) cmd_list->VtxBuffer.Size * (int) sizeof(ImDrawVert);
            const GLsizeiptr idx_buffer_size =
                    (GLsizeiptr) cmd_list->IdxBuffer.Size * (int) sizeof(ImDrawIdx);
            glBufferData(GL_ARRAY_BUFFER, vtx_buffer_size,
                         (const GLvoid *) cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_size,
                         (const GLvoid *) cmd_list->IdxBuffer.Data,
                         GL_STREAM_DRAW);

            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
                const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback != nullptr) {
                    // User callback, registered via ImDrawList::AddCallback()
                    // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                        ImGui_ImplEngineGL3_SetupRenderState(draw_data, fb_width, fb_height,
                                                             vertex_array_object);
                    else
                        pcmd->UserCallback(cmd_list, pcmd);
                } else {
                    // Project scissor/clipping rectangles into framebuffer space
                    ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                                    (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                    ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                                    (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                    if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                        continue;

                    // Apply scissor/clipping rectangle (Y is inverted in OpenGL)
                    glScissor((int) clip_min.x, (int) ((float) fb_height - clip_max.y),
                              (int) (clip_max.x - clip_min.x), (int) (clip_max.y - clip_min.y));

                    // Bind texture, Draw
                    glBindTexture(GL_TEXTURE_2D, (GLuint) (intptr_t)
                            pcmd->GetTexID());
#if !defined(ENGINE_BACKEND_OPENGL_ES)
                    if (bd->GlVersion >= 320) {
                        glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei) pcmd->ElemCount,
                                                 sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT
                                                                        : GL_UNSIGNED_INT,
                                                 (void *) (intptr_t) (pcmd->IdxOffset *
                                                                      sizeof(ImDrawIdx)),
                                                 (GLint) pcmd->VtxOffset);
                    } else {
                        glDrawElements(GL_TRIANGLES, (GLsizei) pcmd->ElemCount,
                                       sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                                       (void *) (intptr_t) (pcmd->IdxOffset * sizeof(ImDrawIdx)));

                    }
#else
                    glDrawElements(GL_TRIANGLES, (GLsizei) pcmd->ElemCount,
                                   sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                                   (void *) (intptr_t) (pcmd->IdxOffset * sizeof(ImDrawIdx)));
#endif
                }
            }
        }

        // Destroy the temporary VAO
        glDeleteVertexArrays(1, &vertex_array_object);

        // Restore modified GL state
        // This "glIsProgram()" check is required because if the program is "pending deletion" at the time of binding backup, it will have been deleted by now and will cause an OpenGL error. See #6220.
        if (last_program == 0 || glIsProgram(last_program)) glUseProgram(last_program);
        glBindTexture(GL_TEXTURE_2D, last_texture);

        if (bd->GlVersion >= 330 || bd->GlProfileIsES3) {
            glBindSampler(0, last_sampler);
        }

        glActiveTexture(last_active_texture);
        glBindVertexArray(last_vertex_array_object);
        glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_USE_VERTEX_ARRAY
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
        last_vtx_attrib_state_pos.SetState(bd->AttribLocationVtxPos);
        last_vtx_attrib_state_uv.SetState(bd->AttribLocationVtxUV);
        last_vtx_attrib_state_color.SetState(bd->AttribLocationVtxColor);
#endif
        glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
        glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha,
                            last_blend_dst_alpha);
        if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
        if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
        if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
        if (last_enable_stencil_test) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);
        if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);

#if !defined(ENGINE_BACKEND_OPENGL_ES)
        // Desktop OpenGL 3.0 and OpenGL 3.1 had separate polygon draw modes for front-facing and back-facing faces of polygons
        if (bd->GlVersion <= 310 || bd->GlProfileIsCompat) {
            glPolygonMode(GL_FRONT, (GLenum) last_polygon_mode[0]);
            glPolygonMode(GL_BACK, (GLenum) last_polygon_mode[1]);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, (GLenum) last_polygon_mode[0]);
        }
#endif

        glViewport(last_viewport[0], last_viewport[1], (GLsizei) last_viewport[2],
                   (GLsizei) last_viewport[3]);
        glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei) last_scissor_box[2],
                  (GLsizei) last_scissor_box[3]);
        (void) bd; // Not all compilation paths use this
    }
}