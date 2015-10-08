# Customizable parts of Makefile
CC				:=	arm-none-eabi-gcc
AS				:=	arm-none-eabi-as
LD				:=	arm-none-eabi-gcc
STRIP			:=	arm-none-eabi-strip
OBJCOPY			:=	arm-none-eabi-objcopy
OBJDUMP			:=	arm-none-eabi-objdump

AS_FLAG			:=	-march=armv7-a -mcpu=cortex-a15

OPT_FLAG		:=	-O2
COMMON_FLAG		:=	-O2 -march=armv7-a -mcpu=cortex-a15 -fomit-frame-pointer -nostdlib
FUCKGCC_FLAG	:=	-Wall -Wno-strict-aliasing -Wno-format
RELEASE_FLAG	:=	$(COMMON_FLAG) $(OPT_FLAG) $(FUCKGCC_FLAG) -DRELEASE
DEBUG_FLAG		:=	$(COMMON_FLAG) $(FUCKGCC_FLAG)

LD_FLAG			:=	-static

STRIP_FLAG		:=	-s #--strip-debug --strip-unneeded

TOOLCHAIN_BASE  :=  /home/user/toolchain
NEWLIB			:=	-L$(TOOLCHAIN_BASE)/newlib-1.19.0/arm-none-eabi/lib/ -lc
GCCLIB			:=	-lgcc

INC				:=	-I$(TOOLCHAIN_BASE)/newlib-1.19.0/arm-none-eabi/include
LIB				:=	$(NEWLIB) $(GCCLIB)

platform		:=
target			:=
sub_dirs		:=	
CC_FLAG			:=

ifeq "$(MAKECMDGOALS)" "mt"
platform		:=	mt
target			:=	meteor
sub_dirs		+=	boot devices guests sys v_devices vmm
CC_FLAG			+=	$(RELEASE_FLAG)
LD_FLAG			+=	-T link_mt.lds
endif

ifeq "$(MAKECMDGOALS)" "mtd"
platform		:=	mtd
target			:=	meteor_debug
sub_dirs		+=	boot devices guests sys v_devices vmm
CC_FLAG			+=	$(DEBUG_FLAG)
LD_FLAG			+=	-T link_mtd.lds
endif
