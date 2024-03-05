#include <engine.hpp>

#include <utils/macros/debug.hpp>

#include <windows/base_app_window.hpp>
#include <factories/app_window_factory.hpp>

#include <platform/platform.hpp>

#include <graphics/backends/gl_render_backend.hpp>
#include <assets/generic_asset_manager.hpp>

#if defined(ENGINE_TARGET_WIN32)
#include <graphics/contexts/win32_graphics_context.hpp>
#include <Windows.h>

// make nVidia and AMD drivers consider this a game by exporting these values
// in order to make it use the discrete graphics!
// (only on Win32)
extern "C"
{
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

// http://developer.amd.com/community/blog/2015/10/02/amd-enduro-system-for-developers/
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#else
#include <graphics/contexts/egl_graphics_context.hpp>
#endif


namespace engine {
	static graphics::IRenderBackendSPtr g_backend{};
	static graphics::IGraphicsContextSPtr g_gfx_ctx{};
	static assets::IAssetManagerSPtr g_asset_manager{};

	bool init() {
        engine::platform::init();

		DEBUG_MSG("engine begin!\n");

        // tell our engine that we want to use the OpenGL backend globally
        // OpenGL is the only supported backend for now
        engine::use_backend(std::make_shared<engine::graphics::backends::GLRenderBackend>());

        engine::use_asset_manager(std::make_shared<engine::assets::GenericAssetManager>()); // use a generic asset manager

        // this should be platform dependent
//#if defined(ENGINE_TARGET_WIN32)
//        engine::use_graphics_context(); // use a global graphics context
//#else
//        engine::use_graphics_context(std::make_shared<engine::graphics::EGLGraphicsContext>()); // use a global graphics context
//#endif

        return true;
	}

	void terminate() {
		DEBUG_MSG("engine termination!\n");

        engine::platform::terminate();
	}

	void use_backend(graphics::IRenderBackendSPtr backend) {
		ASSERT_MSG(!g_backend, "the backend can be set only once!");
		g_backend = backend;
	}

	graphics::IRenderBackendSPtr get_backend() {
		ASSERT_MSG(g_backend, "a backend was not set previously using engine::use_backend");
		return g_backend;
	}

	void use_asset_manager(assets::IAssetManagerSPtr manager) {
		ASSERT_MSG(!g_asset_manager, "the asset manager can be set only once!");
		g_asset_manager = manager;
	}

	assets::IAssetManagerSPtr get_asset_manager() {
		ASSERT_MSG(g_asset_manager, "an asset manager was not set previously using engine::use_asset_manager");
		return g_asset_manager;
	}
}