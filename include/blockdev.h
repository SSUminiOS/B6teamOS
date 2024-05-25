// blockdev.h
#ifndef BLOCKDEV_H
#define BLOCKDEV_H

#include <stdint.h>
#include <stddef.h>

int blockdev_read(uint32_t block, void* buf, size_t count);
int blockdev_write(uint32_t block, const void* buf, size_t count);

#endif
