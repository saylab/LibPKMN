#ifndef __HELPER_H__
#define __HELPER_H__

#include "config.h"

#define CRC16_DEFAULT_SEED 0xFFFF

u16 DLL_EXPORT getle16(const void* ptr);
u16 DLL_EXPORT getbe16(const void* ptr);
u32 DLL_EXPORT getle32(const void* ptr);
u32 DLL_EXPORT getbe32(const void* ptr);
u64 DLL_EXPORT getle64(const void* ptr);
u64 DLL_EXPORT getbe64(const void* ptr);
void DLL_EXPORT md5_print(u8 *buf);
void DLL_EXPORT md5_buf(u8 *buf, u8 *out, size_t len);
u16 DLL_EXPORT crc16(u8 *buf, u32 size, u16 seed);
u16 DLL_EXPORT calc_crc16(u8 *buf, u32 size, u16 initval, u16 outxor);
void DLL_EXPORT xor_3dsfuse(u8 *in, u32 len, u8 *out, u8 *key, u32 keylen);
void DLL_EXPORT hexdump(void *ptr, int buflen);

#endif
