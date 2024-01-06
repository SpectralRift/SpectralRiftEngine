#include <engine.hpp>

#include <iostream>

#include <utils/macros/debug.hpp>

#include <windows/base_app_window.hpp>
#include <factories/app_window_factory.hpp>

#include <platform/platform.hpp>
#include <utils/macros/debug.hpp>
#include <graphics/debug.hpp>

namespace engine {
	static graphics::IRenderBackendSPtr g_backend{};
	static graphics::IGraphicsContextSPtr g_gfx_ctx{};
	static assets::IAssetManagerSPtr g_asset_manager{};

	bool init() {
		DEBUG_MSG("engine begin!\n");
		engine::platform::init();

        engine::graphics::debug::init();

        return true;
	}

	void terminate() {
		DEBUG_MSG("engine termination!\n");
	}

	void use_backend(graphics::IRenderBackendSPtr backend) {
		ASSERT_MSG(!g_backend, "the backend can be set only once!");
		g_backend = backend;
	}

	graphics::IRenderBackendSPtr get_backend() {
		ASSERT_MSG(g_backend, "a backend was not set previously using engine::use_backend");
		return g_backend;
	}

	void use_graphics_context(graphics::IGraphicsContextSPtr ctx) {
		ASSERT_MSG(!g_gfx_ctx, "the graphical context can be set only once!");
		g_gfx_ctx = ctx;
	}

	graphics::IGraphicsContextSPtr get_graphics_context() {
		ASSERT_MSG(g_gfx_ctx, "a graphical context was not set previously using engine::use_graphics_context");
		return g_gfx_ctx;
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