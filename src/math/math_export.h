
#ifndef __MATH_EXPORT_H__
#define __MATH_EXPORT_H__

#if defined(MATH_STATIC_BUILD)
#define MATH_EXPORT
#else

#if defined(WIN32)

#if defined(MATH_IMPLEMENTATION)
#define MATH_EXPORT __declspec(dllexport)
#else
#define MATH_EXPORT __declspec(dllimport)
#endif  // defined(MATH_IMPLEMENTATION)

#else  // defined(WIN32)
#define MATH_EXPORT __attribute__((visibility("default")))
#endif

#endif // define(MATH_STATIC_BUILD)


#endif