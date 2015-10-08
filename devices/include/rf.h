#ifndef _RF_H_
#define _RF_H_

#include <stdint.h>

uint32_t rf_elr_hyp_read();
void rf_elr_hyp_write(uint32_t elr_hyp);

uint32_t rf_spsr_hyp_read();
void rf_spsr_hyp_write(uint32_t spsr_hyp);

#endif
