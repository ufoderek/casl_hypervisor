#include <dev_semihost.h>
#include <io.h>
#include <string.h>

void dev_semihost_init(dev_semihost* dev_semi, uint32_t base_address)
{
    dev_semi->base_address = base_address;
}

void dev_semihost_writew(dev_semihost* dev_semi, semihost_offset offset, uint32_t data)
{
    io_writew(dev_semi->base_address + offset, data);
}

void dev_semihost_writearr(dev_semihost* dev_semi, semihost_offset offset, char* buf, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        io_writeb(dev_semi->base_address + offset + i, buf[i]);
    }
}

void dev_semihost_readarr(dev_semihost* dev_semi, semihost_offset offset, char* buf, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        buf[i] = io_readb(dev_semi->base_address + offset + i);
    }
}

uint32_t dev_semihost_readw(dev_semihost* dev_semi, semihost_offset offset)
{
    return io_readw(dev_semi->base_address + offset);
}

int dev_semihost_do_open(dev_semihost* dev_semi, const char* path, int flags, int mode)
{
    dev_semihost_writew(dev_semi, OFFSET_TYPE, TYPE_OPEN);
    dev_semihost_writearr(dev_semi, OFFSET_PATH, (char*)path, strlen(path) + 1);
    dev_semihost_writew(dev_semi, OFFSET_FLAGS, flags);
    dev_semihost_writew(dev_semi, OFFSET_MODE, mode);

    dev_semihost_writew(dev_semi, OFFSET_SUBMIT, 0);
    return dev_semihost_readw(dev_semi, OFFSET_RET_VAL);
}

int dev_semihost_do_read(dev_semihost* dev_semi, int fd, char* buf, int count)
{
    dev_semihost_writew(dev_semi, OFFSET_TYPE, TYPE_READ);
    dev_semihost_writew(dev_semi, OFFSET_FD, fd);
    dev_semihost_writew(dev_semi, OFFSET_COUNT, count);
    dev_semihost_writew(dev_semi, OFFSET_SUBMIT, 0);
    dev_semihost_readarr(dev_semi, OFFSET_BUF, buf, count);

    return dev_semihost_readw(dev_semi, OFFSET_RET_VAL);
}

int dev_semihost_do_write(dev_semihost* dev_semi, int fd, char* buf, int count)
{
    dev_semihost_writew(dev_semi, OFFSET_TYPE, TYPE_WRITE);
    dev_semihost_writew(dev_semi, OFFSET_FD, fd);
    dev_semihost_writew(dev_semi, OFFSET_COUNT, count);
    dev_semihost_writearr(dev_semi, OFFSET_BUF, buf, count);

    dev_semihost_writew(dev_semi, OFFSET_SUBMIT, 0);
    return dev_semihost_readw(dev_semi, OFFSET_RET_VAL);
}

int dev_semihost_do_lseek(dev_semihost* dev_semi, int fd, int offset, int whence)
{
    dev_semihost_writew(dev_semi, OFFSET_TYPE, TYPE_LSEEK);
    dev_semihost_writew(dev_semi, OFFSET_FD, fd);
    dev_semihost_writew(dev_semi, OFFSET_OFFSET, offset);
    dev_semihost_writew(dev_semi, OFFSET_WHENCE, whence);

    dev_semihost_writew(dev_semi, OFFSET_SUBMIT, 0);
    return dev_semihost_readw(dev_semi, OFFSET_RET_VAL);
}

int dev_semihost_do_close(dev_semihost* dev_semi, int fd)
{
    dev_semihost_writew(dev_semi, OFFSET_TYPE, TYPE_CLOSE);
    dev_semihost_writew(dev_semi, OFFSET_FD, fd);

    dev_semihost_writew(dev_semi, OFFSET_SUBMIT, 0);
    return dev_semihost_readw(dev_semi, OFFSET_RET_VAL);
}

int dev_semihost_do_fstat(dev_semihost* dev_semi, int fd, struct stat* st)
{
    dev_semihost_writew(dev_semi, OFFSET_TYPE, TYPE_FSTAT);
    dev_semihost_writew(dev_semi, OFFSET_FD, fd);
    dev_semihost_writew(dev_semi, OFFSET_SUBMIT, 0);

    //this is wrong, the field size is not 4 bytes, but it works
    st->st_dev = dev_semihost_readw(dev_semi, OFFSET_BUF);
    st->st_ino = dev_semihost_readw(dev_semi, OFFSET_BUF + 4);
    st->st_mode = dev_semihost_readw(dev_semi, OFFSET_BUF + 8);
    st->st_nlink = dev_semihost_readw(dev_semi, OFFSET_BUF + 12);
    st->st_uid = dev_semihost_readw(dev_semi, OFFSET_BUF + 16);
    st->st_gid = dev_semihost_readw(dev_semi, OFFSET_BUF + 20);
    st->st_rdev = dev_semihost_readw(dev_semi, OFFSET_BUF + 24);
    st->st_size = dev_semihost_readw(dev_semi, OFFSET_BUF + 28);
    st->st_blksize = dev_semihost_readw(dev_semi, OFFSET_BUF + 32);
    st->st_blocks = dev_semihost_readw(dev_semi, OFFSET_BUF + 36);
    st->st_atime = dev_semihost_readw(dev_semi, OFFSET_BUF + 40);
    st->st_mtime = dev_semihost_readw(dev_semi, OFFSET_BUF + 44);
    st->st_ctime = dev_semihost_readw(dev_semi, OFFSET_BUF + 48);

    return dev_semihost_readw(dev_semi, OFFSET_RET_VAL);
}

