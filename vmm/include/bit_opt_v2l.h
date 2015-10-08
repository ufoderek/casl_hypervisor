#ifndef _BIT_OPT_V2L_H_
#define _BIT_OPT_V2L_H_

#include <stdint.h>

inline const uint32_t mask(uint32_t msb, uint32_t lsb);

inline const uint64_t mask64(uint32_t msb, uint32_t lsb);

inline uint32_t get_field(uint32_t src, uint32_t msb, uint32_t lsb);

inline uint64_t get_field64(uint64_t src, uint32_t msb, uint32_t lsb);

inline uint32_t get_bit(uint32_t src, uint32_t n);

inline uint64_t get_bit64(uint64_t src, uint32_t n);

inline const uint32_t imask(uint32_t msb, uint32_t lsb);

inline const uint64_t imask64(uint32_t msb, uint32_t lsb);

inline void set_field(uint32_t* src, uint32_t msb, uint32_t lsb, uint32_t field);

inline void set_field64(uint64_t* src, uint32_t msb, uint32_t lsb, uint64_t field);

inline void set_bit(uint32_t* src, uint32_t n, uint32_t b);

inline void set_bit64(uint64_t* src, uint32_t n, uint32_t b);

inline void copy_field(uint32_t* dst, uint32_t src, uint32_t msb, uint32_t lsb);

inline void copy_bit(uint32_t* dst, uint32_t src, uint32_t n);

#endif
