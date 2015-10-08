#ifndef _DEV_SEMIHOST_H_
#define _DEV_SEMIHOST_H_

#include <stdint.h>
#include <io.h>
#include <sys/stat.h>

typedef enum semihost_type
{
    TYPE_OPEN = 0,
    TYPE_READ = 1,
    TYPE_WRITE = 2,
    TYPE_LSEEK = 3,
    TYPE_FSTAT = 4,
    TYPE_CLOSE = 5,
} semihost_type;

typedef enum semihost_offset
{
    OFFSET_TYPE = 0x0,
    OFFSET_FD = 0x4,
    OFFSET_FLAGS = 0x8,
    OFFSET_MODE = 0xC,
    OFFSET_WHENCE = 0x10,
    OFFSET_OFFSET = 0x14,
    OFFSET_COUNT = 0x18,
    OFFSET_RET_VAL = 0x1C,
    OFFSET_SUBMIT = 0x100,
    OFFSET_PATH = 0x200,
    OFFSET_BUF = 0x2000
} semihost_offset;

typedef struct dev_semihost
{
    uint32_t base_address;
} dev_semihost;

void dev_semihost_init(dev_semihost* dev_semi, uint32_t base_address);

void dev_semihost_writew(dev_semihost* dev_semi, semihost_offset offset, uint32_t data);

void dev_semihost_writearr(dev_semihost* dev_semi, semihost_offset offset, char* buf, int length);

void dev_semihost_readarr(dev_semihost* dev_semi, semihost_offset offset, char* buf, int length);

uint32_t dev_semihost_readw(dev_semihost* dev_semi, semihost_offset offset);

int dev_semihost_do_open(dev_semihost* dev_semi, const char* path, int flags, int mode);

int dev_semihost_do_read(dev_semihost* dev_semi, int fd, char* buf, int count);

int dev_semihost_do_write(dev_semihost* dev_semi, int fd, char* buf, int count);

int dev_semihost_do_lseek(dev_semihost* dev_semi, int fd, int offset, int whence);

int dev_semihost_do_close(dev_semihost* dev_semi, int fd);

int dev_semihost_do_fstat(dev_semihost* dev_semi, int fd, struct stat* st);


#endif
