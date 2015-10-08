#include <cp15.h>
#include <bit_opt_v2l.h>
#include <debug_utils.h>

// hard to explain

#define CP15_PARSE64(action, name, reg_h, reg_l)\
    {\
        switch(name)\
        {\
            case TTBR0_LPAE:\
                asm_cp15_##action(reg_h, reg_l, 2, 0); break;\
            case TTBR1_LPAE:\
                asm_cp15_##action(reg_h, reg_l, 2, 1); break;\
            case HTTBR_LPAE:\
                asm_cp15_##action(reg_h, reg_l, 2, 4); break;\
            case VTTBR_LPAE:\
                asm_cp15_##action(reg_h, reg_l, 2, 6); break;\
            default: printe("cp15_" #action "64 error, %X", (unsigned int)name); break;\
        }\
    }


#define CP15_PARSE(action, name, reg)\
    {\
        switch(name)\
        {\
            case MIDR:\
                asm_cp15_##action(reg, 0, 0, 0, 0); break;\
            case CTR:\
                asm_cp15_##action(reg, 0, 0, 0, 1); break;\
            case MPIDR:\
                asm_cp15_##action(reg, 0, 0, 0, 5); break;\
            case MMFR0:\
                asm_cp15_##action(reg, 0, 0, 1, 4); break;\
            case MMFR1:\
                asm_cp15_##action(reg, 0, 0, 1, 5); break;\
            case MMFR2:\
                asm_cp15_##action(reg, 0, 0, 1, 6); break;\
            case MMFR3:\
                asm_cp15_##action(reg, 0, 0, 1, 7); break;\
            case CCSIDR0:\
                asm_cp15_##action(reg, 0, 1, 0, 0); break;\
            case CCSIDR1:\
                asm_cp15_##action(reg, 0, 1, 0, 6); break;\
            case CCSIDR2:\
                asm_cp15_##action(reg, 0, 1, 0, 7); break;\
            case CLIDR:\
                asm_cp15_##action(reg, 0, 1, 0, 1); break;\
            case CSSELR:\
                asm_cp15_##action(reg, 0, 2, 0, 0); break;\
            case VPIDR:\
                asm_cp15_##action(reg, 0, 4, 0, 0); break;\
            case SCTLR:\
                asm_cp15_##action(reg, 1, 0, 0, 0); break;\
            case SCR:\
                asm_cp15_##action(reg, 1, 0, 1, 0); break;\
            case HSCTLR:\
                asm_cp15_##action(reg, 1, 4, 0, 0); break;\
            case HCR:\
                asm_cp15_##action(reg, 1, 4, 1, 0); break;\
            case HDCR:\
                asm_cp15_##action(reg, 1, 4, 1, 1); break;\
            case HCPTR:\
                asm_cp15_##action(reg, 1, 4, 1, 2); break;\
            case HSTR:\
                asm_cp15_##action(reg, 1, 4, 1, 3); break;\
            case TTBCR:\
                asm_cp15_##action(reg, 2, 0, 0, 0); break;\
            case TTBR0:\
                asm_cp15_##action(reg, 2, 0, 0, 1); break;\
            case TTBR1:\
                asm_cp15_##action(reg, 2, 0, 0, 2); break;\
            case HTCR:\
                asm_cp15_##action(reg, 2, 4, 0, 2); break;\
            case VTCR:\
                asm_cp15_##action(reg, 2, 4, 1, 2); break;\
            case DACR:\
                asm_cp15_##action(reg, 3, 0, 0, 0); break;\
            case DFSR:\
                asm_cp15_##action(reg, 5, 0, 0, 0); break;\
            case IFSR:\
                asm_cp15_##action(reg, 5, 0, 0, 1); break;\
            case ADFSR:\
                asm_cp15_##action(reg, 5,0,1,0); break;\
            case AIFSR:\
                asm_cp15_##action(reg, 5,0,1,1); break;\
            case HADFSR:\
                asm_cp15_##action(reg,5,4,1,0); break;\
            case HAIFSR:\
                asm_cp15_##action(reg,5,4,1,1); break;\
            case HSR:\
                asm_cp15_##action(reg, 5, 4, 2, 0); break;\
            case DFAR:\
                asm_cp15_##action(reg, 6, 0, 0, 0); break;\
            case IFAR:\
                asm_cp15_##action(reg, 6, 0, 0, 2); break;\
            case HDFAR:\
                asm_cp15_##action(reg, 6, 4, 0, 0); break;\
            case HIFAR:\
                asm_cp15_##action(reg, 6, 4, 0, 2); break;\
            case HPFAR:\
                asm_cp15_##action(reg, 6, 4, 0, 4); break;\
            case PRRR:\
                asm_cp15_##action(reg, 10, 0, 2, 0); break;\
            case NMRR:\
                asm_cp15_##action(reg, 10, 0, 2, 1); break;\
            case VBAR:\
                asm_cp15_##action(reg, 12, 0, 0, 0); break;\
            case MVBAR:\
                asm_cp15_##action(reg, 12, 0, 0, 1); break;\
            case ISR:\
                asm_cp15_##action(reg, 12, 0, 1, 0); break;\
            case HVBAR:\
                asm_cp15_##action(reg, 12, 4, 0, 0); break;\
            case FCSEIDR:\
                asm_cp15_##action(reg, 13, 0, 0, 0); break;\
            case CONTEXTIDR:\
                asm_cp15_##action(reg, 13, 0, 0, 1); break;\
            case TPIDR_URO:\
                asm_cp15_##action(reg, 13, 0, 0, 3); break;\
                \
            default: printe("cp15_" #action " error, %X.", (unsigned int)name); break;\
        }\
    }

uint32_t cp15_read(cp15_reg_names name)
{

    uint32_t full_name = name;

    set_field(&full_name, 31, 24, 31);
    set_field(&full_name, 23, 16, 0);

    uint64_t reg = 0;

    CP15_PARSE(read, full_name, reg);

    return get_field(reg, get_hsb(name), get_lsb(name));
}

uint64_t cp15_read64(cp15_reg_names64 name)
{

    uint32_t full_name = name;

    set_field(&full_name, 31, 24, 63);
    set_field(&full_name, 23, 16, 0);

    uint32_t reg_h = 0;
    uint32_t reg_l = 0;

    CP15_PARSE64(read64, full_name, reg_h, reg_l);

    return get_field64(((uint64_t)reg_h << 32) | reg_l, get_hsb(name), get_lsb(name));
}

void cp15_write(cp15_reg_names name, uint32_t value)
{
    uint32_t full_name = name;

    set_field(&full_name, 31, 24, 31);
    set_field(&full_name, 23, 16, 0);

    uint32_t reg;
    if(full_name != name)
    {
        reg = cp15_read(full_name);
        set_field(&reg, get_hsb(name), get_lsb(name), value);
    }
    else
    {
        reg = value;
    }

    CP15_PARSE(write, full_name, reg);
}

void cp15_write64(cp15_reg_names64 name, uint64_t value)
{
    uint32_t full_name = name;

    set_field(&full_name, 31, 24, 63);
    set_field(&full_name, 23, 16, 0);

    uint64_t reg;

    if(full_name != name)
    {
        reg = cp15_read64(full_name);
        set_field64(&reg, get_hsb(name), get_lsb(name), value);
    }
    else
    {
        reg = value;
    }

    uint32_t reg_h = reg >> 32;
    uint32_t reg_l = (uint32_t)reg;

    CP15_PARSE64(write64, full_name, reg_h, reg_l);
}
