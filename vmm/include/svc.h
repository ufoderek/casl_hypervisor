#ifndef _SVC_H_
#define _SVC_H_

#define asm_svc(imm16)\
    {\
        __asm__ __volatile__ (\
                              "swi %0"\
                              :\
                              : "I"(imm16)\
                             );\
    }


#endif
