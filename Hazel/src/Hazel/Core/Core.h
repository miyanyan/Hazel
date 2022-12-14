#pragma once

#if HZ_DYNAMIC_LINK
#ifdef HZ_BUILD_DLL
#define HAZEL_API __declspec(dllexport)
#else
#define HAZEL_API __declspec(dllimport)
#endif
#else
#define HAZEL_API
#endif

#define BIT(x) (1 << x)
