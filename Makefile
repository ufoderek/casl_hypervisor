# include user defined parameters
include Defs.mk

bin_dir				:=	bin
target_bin			:=	$(bin_dir)/$(target)
target_elf_bin		:=	$(bin_dir)/$(target).elf
build_dir			:=	builds/$(target)/

src_files			:=	$(shell find $(sub_dirs) -name '*.c')
obj_files			:=	$(notdir $(src_files:%.c=%.o))
full_obj_files		:=	$(addprefix $(build_dir), $(obj_files))

as_src_files		:=	$(shell find $(sub_dirs) -name '*.s')
as_obj_files		:=	$(notdir $(as_src_files:%.s=%.o))
as_full_obj_files	:=	$(addprefix $(build_dir), $(as_obj_files))

dep_files			:=	$(subst .o,.d,$(full_obj_files))

INC					+=	$(addprefix -I./, $(addsuffix /include, $(sub_dirs)))

create-build-dir	:=	$(shell test -d $(build_dir) || mkdir -p $(build_dir))
create-binary-dir	:=	$(shell test -d $(bin_dir) || mkdir -p $(bin_dir))

vpath %.c $(sub_dirs)
vpath %.s $(sub_dirs)

.PHONY: $(platform)
$(platform): $(target_bin)

$(target_bin): $(target_elf_bin)
	@echo	'	OBJCOPY $<'
	@$(OBJCOPY) -O binary $(target_elf_bin) $(target_bin)
	@echo	'	OBJDUMP $<.txt'
	@$(OBJDUMP) -xdS $(target_elf_bin) > $(target_elf_bin).txt
	@mv $(target_elf_bin).txt ./

$(target_elf_bin): $(as_full_obj_files) $(full_obj_files)
	@echo	'	LD $@'
	@$(LD) $(CC_FLAG) $(LD_FLAG) $(INC) $^ -o $@ $(LIB)
ifeq "$(platform)" "mt"
	@echo	'	STRIP $@'
	@$(STRIP) $(STRIP_FLAG) $@
endif

$(build_dir)%.o: %.s
	@echo '	AS $<'
	@$(AS) $(AS_FLAG) $(INC) $< -o $@

$(build_dir)%.o: %.c
	@echo '	CC $<'
	@$(CC) $(CC_FLAG) $(INC) -M -MF $(@:.o=.d) -MP -MT $@ $<
	@$(CC) $(CC_FLAG) $(INC) $< -c -o $@

.PHONY: clean
clean:
	rm -rf *.log
	rm -rf bin
	rm -rf builds

ifneq "$(MAKECMDGOALS)" "clean"
-include $(dep_files)
endif
