#pragma once

#if defined(ENGINE_TARGET_WIN32)
#include <platform/win32/win32_macros.hpp>
#endif

#if defined(ANDROID)
#ifndef ENGINE_FEATURE_EGL
#define ENGINE_FEATURE_EGL
#endif

#ifndef ENGINE_BACKEND_OPENGL_ES
#define ENGINE_BACKEND_OPENGL_ES
#endif
#endif