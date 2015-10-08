#include <rf.h>

uint32_t rf_elr_hyp_read()
{
    uint32_t elr_hyp;

    asm volatile("mrs %0, ELR_hyp"
                 : "=r"(elr_hyp)
                );

    return elr_hyp;
}

void rf_elr_hyp_write(uint32_t elr_hyp)
{
    asm volatile("msr ELR_hyp, %0"
                 :
                 : "r"(elr_hyp)
                );
}

uint32_t rf_spsr_hyp_read()
{
    uint32_t spsr_hyp;

    asm volatile("mrs %0, SPSR_hyp"
                 : "=r"(spsr_hyp)
                );

    return spsr_hyp;
}

void rf_spsr_hyp_write(uint32_t spsr_hyp)
{
    asm volatile("msr SPSR_hyp, %0"
                 :
                 : "r"(spsr_hyp)
                );
}

