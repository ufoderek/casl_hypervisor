#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

inline uint8_t io_readb(const volatile uint32_t addr);
inline uint16_t io_readh(const volatile uint32_t addr);
inline uint32_t io_readw(const volatile uint32_t addr);
inline uint64_t io_readd(const volatile uint32_t addr);

inline void io_writeb(const volatile uint32_t addr, const uint8_t data);
inline void io_writeh(const volatile uint32_t addr, const uint16_t data);
inline void io_writew(const volatile uint32_t addr, const uint32_t data);
inline void io_writed(const volatile uint32_t addr, const uint64_t data);

#endif
