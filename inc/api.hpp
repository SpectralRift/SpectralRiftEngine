#pragma once

// platform specific macros
#include <platform/win32/macros.hpp>

// API fallback macros
#ifndef ENGINE_API_EXPORT
#define ENGINE_API_EXPORT
#endif

#ifndef ENGINE_API_IMPORT
#define ENGINE_API_IMPORT
#endif

#ifndef NULL
#define NULL 0
#endif

// the real API macro
#ifdef ENGINE_BUILD_LIBRARY
#define ENGINE_API ENGINE_API_EXPORT
#else
#define ENGINE_API ENGINE_API_IMPORT
#endif