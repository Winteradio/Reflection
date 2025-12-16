#ifndef __REFLECTION_MACRO_H__
#define __REFLECTION_MACRO_H__

#if defined(__GNUC__)
	#define __CLASSNAME__ __PRETTY_FUNCTION__
#elif defined(_WIN32)
	#define __CLASSNAME__ __FUNCSIG__
#else
	#define __CLASSNAME__ __func__
#endif

#if defined(__GNUC__) || defined(__clang__)
#define __STATIC_USED__ __attribute__((used))
#else
#define __STATIC_USED__
#endif

#endif // __REFLECTION_MACRO_H__