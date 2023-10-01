#pragma once

// win32 specific macros
#ifdef WIN32

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