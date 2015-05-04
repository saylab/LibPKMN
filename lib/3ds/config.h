#ifndef __3DSFUSE_CONFIG_H__
#define __3DSFUSE_CONFIG_H__

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#ifdef _MSC_VER
    #define DLL_EXPORT __declspec(dllexport)
#elif defined(__GNUG__) && __GNUG__ >= 4
    #define DLL_EXPORT __attribute__((visibility("default")))
#else
    #define DLL_EXPORT
#endif

#endif