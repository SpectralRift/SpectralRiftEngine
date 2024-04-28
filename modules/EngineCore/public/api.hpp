#pragma once

// win32 specific macros
#if defined(WIN32)
#ifndef ENGINE_API_EXPORT
#define ENGINE_API_EXPORT __declspec(dllexport)
#endif

#ifndef ENGINE_API_IMPORT
#define ENGINE_API_IMPORT __declspec(dllimport)
#endif

#ifndef ENGINE_TARGET_WIN32
#define ENGINE_TARGET_WIN32
#endif
#endif

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

#ifndef PTR_NULL
#define PTR_NULL static_cast<uintptr_t>(-1)
#endif

// the real API macro
#ifdef ENGINE_BUILD_LIBRARY
#define ENGINE_API ENGINE_API_EXPORT
#else
#define ENGINE_API ENGINE_API_IMPORT
#endif