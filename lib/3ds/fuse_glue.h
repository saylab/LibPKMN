#ifndef __FUSE_GLUE_H__
#define __FUSE_GLUE_H__

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

int DLL_EXPORT fuse_sav_init(u8 *buf, u32 size, u8 *xorpad, u32 xorpad_sz, int argc, char *argv[]);
int DLL_EXPORT sav_getattr(const char *path, struct stat *stbuf);
int DLL_EXPORT sav_open(const char *path, struct fuse_file_info *fi);
int DLL_EXPORT sav_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int DLL_EXPORT sav_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int DLL_EXPORT sav_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

#ifdef __cplusplus
}
#endif

#endif
