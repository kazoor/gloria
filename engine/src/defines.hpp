#pragma once
#include <spdlog/spdlog.h>

#ifdef GL_API
#ifdef GL_EXPORT
#define GL_API __declspec(dllexport)
#else
#define GL_API __declspec(dllimport)
#endif
#endif
