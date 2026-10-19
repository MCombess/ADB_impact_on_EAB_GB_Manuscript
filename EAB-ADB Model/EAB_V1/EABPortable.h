#ifndef __EABPortable_H__
#define __EABPortable_H__

// Standard headers that nag.h used to pull in indirectly. Provides replacements for the MSVC-only path and string
//functions so the model can also build with GCC or Clang as well as visual studio

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <stdexcept>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>

#ifndef _MAX_PATH
#define _MAX_PATH 4096
#endif

inline char* _getcwd(char* buf, int size) { return getcwd(buf, static_cast<size_t>(size)); }
inline int _mkdir(const char* path) { return mkdir(path, 0755); }

inline int strcpy_s(char* dest, size_t size, const char* src)
{
	size_t len = std::strlen(src);
	if (len >= size)
		throw std::logic_error("strcpy_s: file path too long for buffer");
	std::memcpy(dest, src, len + 1);
	return 0;
}
template <size_t N> inline int strcpy_s(char (&dest)[N], const char* src) { return strcpy_s(dest, N, src); }

inline int strcat_s(char* dest, size_t size, const char* src)
{
	size_t used = std::strlen(dest);
	size_t len = std::strlen(src);
	if (used + len >= size)
		throw std::logic_error("strcat_s: file path too long for buffer");
	std::memcpy(dest + used, src, len + 1);
	return 0;
}
template <size_t N> inline int strcat_s(char (&dest)[N], const char* src) { return strcat_s(dest, N, src); }

inline int sprintf_s(char* buf, size_t size, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int n = std::vsnprintf(buf, size, fmt, args);
	va_end(args);
	if (n < 0 || static_cast<size_t>(n) >= size)
		throw std::logic_error("sprintf_s: file name too long for buffer");
	return n;
}
#endif

#endif
