#ifndef _HYPERVISOR_CALL_H_
#define _HYPERVISOR_CALL_H_

#include <stdint.h>
#include <guest.h>

void hypervisor_call(guest* vm, uint32_t imm16);

#endif
