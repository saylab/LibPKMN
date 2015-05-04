#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u8 hash[16];
	u32 block_idx;
	u32 count;
} hash_entry;

int DLL_EXPORT find_key(u8 *buf, size_t len, u8 *out);

#ifdef __cplusplus
}
#endif

#endif
