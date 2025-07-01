#ifndef _SUPREGEX_BASE_BASEDEFS_H_
#define _SUPREGEX_BASE_BASEDEFS_H_

#if defined(_MSC_VER)
	#define SUPREGEX_DLLEXPORT __declspec(dllexport)
	#define SUPREGEX_DLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
	#define SUPREGEX_DLLEXPORT __attribute__((__visibility__("default")))
	#define SUPREGEX_DLLIMPORT __attribute__((__visibility__("default")))
#endif

#if defined(_MSC_VER)
	#define SUPREGEX_FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
	#ifndef NDEBUG
		#define SUPREGEX_FORCEINLINE __attribute__((__always_inline__)) inline
	#else
		#define SUPREGEX_FORCEINLINE inline
	#endif
#endif

#if defined(_MSC_VER)
	#define SUPREGEX_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
		apiModifier extern template class name<__VA_ARGS__>;
	#define SUPREGEX_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
		apiModifier template class name<__VA_ARGS__>;
#elif defined(__GNUC__) || defined(__clang__)
	#define SUPREGEX_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
		extern template class apiModifier name<__VA_ARGS__>;
	#define SUPREGEX_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...) \
		template class name<__VA_ARGS__>;
#else
	#define SUPREGEX_DECL_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...)
	#define SUPREGEX_DEF_EXPLICIT_INSTANTIATED_CLASS(apiModifier, name, ...)
#endif

#define SUPREGEX_CONTAINER_OF(t, m, p) ((t *)(((char *)p) - offsetof(t, m)))

#if SUPREGEX_DYNAMIC_LINK
	#if IS_SUPREGEX_BUILDING
		#define SUPREGEX_API SUPREGEX_DLLEXPORT
	#else
		#define SUPREGEX_API SUPREGEX_DLLIMPORT
	#endif
#else
	#define SUPREGEX_API
#endif

#ifdef _MSC_VER

	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#include <cstdlib>

	// Memory leak detection
	#define malloc(n) _malloc_dbg(n, _NORMAL_BLOCK, __FILE__, __LINE__)
	#define free(n) _free_dbg(n, _NORMAL_BLOCK)
	#define __DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new __DBG_NEW

#endif

namespace supregex {
}

#endif
