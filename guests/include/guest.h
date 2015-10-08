#ifndef _GUEST_H_
#define _GUEST_H_

#include <stdint.h>
#include <pagetable.h>
#include <vdev_rpb_sysctrl.h>
#include <vdev_rpb_sysreg.h>
#include <vdev_gic2_dist.h>
#include <vdev_timer.h>
#include <monitor.h>

typedef enum wfi_state_t
{
    WFI_In,
    WFI_Out,
    WFI_PendingIRQ
} wfi_state_t;

typedef struct sched_info
{
    wfi_state_t wfi_state;
} sched_info;

typedef enum guest_type
{
    Realview_PB,
    I_Know_Boss_Will_Need_New_Board,
    And_You_Are_The_Choosen_One,
    RIP
} guest_type;

typedef struct guest_rf
{
    uint32_t r_usr[13];

    uint32_t sp_usr;
    uint32_t sp_svc;
    uint32_t sp_abt;
    uint32_t sp_und;
    uint32_t sp_irq;
    uint32_t sp_fiq;

    uint32_t lr_usr;
    uint32_t lr_svc;
    uint32_t lr_abt;
    uint32_t lr_und;
    uint32_t lr_irq;
    uint32_t lr_fiq;

    uint32_t spsr_svc;
    uint32_t spsr_abt;
    uint32_t spsr_und;
    uint32_t spsr_irq;
    uint32_t spsr_fiq;

    uint32_t elr_hyp;
    uint32_t spsr_hyp;

} guest_rf;

typedef struct guest_cp15
{
    uint32_t csselr;
    uint32_t sctlr;
    uint32_t ttbr0;
    uint32_t ttbr1;
    uint32_t ttbcr;
    uint32_t dacr;
    uint32_t dfsr;
    uint32_t ifsr;
    uint32_t dfar;
    uint32_t ifar;
    uint32_t prrr;
    uint32_t nmrr;
    uint32_t fcseidr;
    uint32_t contextidr;
    uint32_t tpidr_uro;

    uint64_t vttbr_lpae;
} guest_cp15;

typedef struct guest_vcpu_if
{
    uint32_t ctlr;
} guest_vcpu_if;

typedef struct guest_backup
{
    bool dirty;
    guest_rf rf;
    guest_cp15 cp15;
    guest_vcpu_if vcpu_if;
    uint32_t list[64];
} guest_backup;

typedef struct guest
{
    //basic parameters
    bool valid;
    guest_type type;
    int id;
    uint32_t mem_base_pa;
    uint32_t mem_base;
    uint32_t highmem_base;
    uint32_t mem_size;
    uint32_t kernel_base;
    uint32_t initrd_base;
    uint32_t atag_base;
    uint32_t vcpu_if_base;
    pagetable_ld pt;

    //scheduler
    sched_info sched;

    //state backup
    guest_backup bk;

    //monitor
    monitor mon;

    //virtual drivers
    vdev_gic2_dist vdev_gic_dist;
    vdev_rpb_sysctrl vdev_sysctrl;
    vdev_rpb_sysreg vdev_sysreg;
    vdev_timer vdev_timer_2;
} guest;

uint32_t read_reg_from_stack(uint32_t sp, uint32_t rn);

void write_reg_to_stack(uint32_t sp, uint32_t rn, uint32_t value);

void guest_backup_rf(guest* vm, uint32_t sp);
void guest_restore_rf(guest* vm, uint32_t sp);

void guest_backup_cp15(guest* vm);
void guest_restore_cp15(guest* vm);

void guest_backup_list(guest* vm);
void guest_restore_list(guest* vm);

void guest_backup_vcpu_if(guest* vm);
void guest_restore_vcpu_if(guest* vm);

void guest_suspend(guest* vm, uint32_t sp);
void guest_restore(guest* vm, uint32_t sp);

#endif
