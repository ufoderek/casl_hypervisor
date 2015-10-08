#include <bit_opt_v2l.h>

inline const uint32_t mask(uint32_t msb, uint32_t lsb)
{
    const uint32_t t = 0xFFFFFFFF;
    return (t << lsb) & (t>>(31 - msb));
}

inline const uint64_t mask64(uint32_t msb, uint32_t lsb)
{
    const uint64_t t = 0xFFFFFFFFFFFFFFFFULL;
    return (t << lsb) & (t>>(63 - msb));
}

inline uint32_t get_field(uint32_t src, uint32_t msb, uint32_t lsb)
{
    return (src & mask(msb, lsb)) >> lsb;
}

inline uint64_t get_field64(uint64_t src, uint32_t msb, uint32_t lsb)
{
    return (src & mask64(msb, lsb)) >> lsb;
}

inline uint32_t get_bit(uint32_t src, uint32_t n)
{
    return get_field(src, n, n);
}

inline uint64_t get_bit64(uint64_t src, uint32_t n)
{
    return get_field64(src, n, n);
}

inline const uint32_t imask(uint32_t msb, uint32_t lsb)
{
    return ~mask(msb, lsb);
}

inline const uint64_t imask64(uint32_t msb, uint32_t lsb)
{
    return ~mask64(msb, lsb);
}

inline void set_field(uint32_t* src, uint32_t msb, uint32_t lsb, uint32_t field)
{
    *src = (*src & imask(msb, lsb)) | ((field << lsb)&mask(msb, lsb));
}

inline void set_field64(uint64_t* src, uint32_t msb, uint32_t lsb, uint64_t field)
{
    *src = (*src & imask64(msb, lsb)) | ((field << lsb)&mask64(msb, lsb));
}

inline void set_bit(uint32_t* src, uint32_t n, uint32_t b)
{
    if(b)
    {
        *src |= mask(n, n);
    }
    else
    {
        *src &= imask(n, n);
    }
}

inline void set_bit64(uint64_t* src, uint32_t n, uint32_t b)
{
    if(b)
    {
        *src |= mask64(n, n);
    }
    else
    {
        *src &= imask64(n, n);
    }
}

inline void copy_field(uint32_t* dst, uint32_t src, uint32_t msb, uint32_t lsb)
{
    set_field(dst, msb, lsb, get_field(src, msb, lsb));
}

inline void copy_bit(uint32_t* dst, uint32_t src, uint32_t n)
{
    set_bit(dst, n, get_bit(src, n));
}

