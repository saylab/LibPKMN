#ifndef _UTILS_H
#define _UTILS_H

#include "config.h"

#include <openssl/sha.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void DLL_EXPORT sha256(void* buffer, size_t size, uint8_t* result);

#ifdef __cplusplus
}
#endif

#endif /* _UTILS_H */
