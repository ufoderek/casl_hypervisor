#ifndef _EXCEPTION_HANDLER_H_
#define _EXCEPTION_HANDLER_H_

extern void und_handler(void);
extern void svc_handler(void);
extern void pabort_handler(void);
extern void dabort_handler(void);
extern void hyp_trap_handler(uint32_t sp);
extern void irq_handler(uint32_t sp);
extern void fiq_handler(void);

#endif
