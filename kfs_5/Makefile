PROJECT = jros
PROJECT_DIR = .

LIBKFS_DIR = ./libkfs
LIBKFS = $(LIBKFS_DIR)/libkfs.a
LIBKFS_INCS = -I $(LIBKFS_DIR)/incs

RM = /bin/rm
KFS_BUILD_DIR = isodir
KFS_CONFIG_DIR = config

OPT_FLAG = -g3 -Og

DOCKER_DIR = /app
DOCKER = /usr/bin/docker
DOCKER_CONT_NAME = rertzer/crosscompiler_i386
DOCKER_EXEC = $(DOCKER) run --rm -v $(PROJECT_DIR):$(DOCKER_DIR) $(DOCKER_CONT_NAME)

FLAG_DEBUG		= -g -ggdb3
FLAG_WARNING	= -Wall -Wextra -Winline -Wformat
FLAG			= $(FLAG_DEBUG) $(FLAG_WARNING) $(OPT_FLAG) -DJROS

SRCS_DIR = $(PROJECT_DIR)/srcs
OBJS_DIR = $(PROJECT_DIR)/objs
INCS_DIR = $(PROJECT_DIR)/incs
DEPS_DIR = $(PROJECT_DIR)/deps


SOURCE_MALLOC = $(addprefix malloc/,	\
				malloc_bins.c			\
				malloc_block.c			\
				malloc_page.c			\
				malloc_show.c			\
				malloc_test.c			\
				malloc.c)

SOURCE_STDLIB = $(addprefix stdlib/,	\
				$(SOURCE_MALLOC)		\
				itoa.c					\
				atoi.c)


SOURCE_STDIO = $(addprefix stdio/,		\
				printf.c				\
				printk.c)

SOURCE_UNISTD = $(addprefix unistd/,	\
				write.c)

SOURCES_LIBS = $(addprefix libs/, 		\
				$(SOURCE_STDLIB)		\
				$(SOURCE_STDIO)			\
				$(SOURCE_UNISTD)		\
				arg_split.c				\
				substr.c	\
				math.c)

SOURCES_MEMORY	= $(addprefix memory/, 		\
					mbook.c					\
					paging.c				\
					page_fault.c			\
					memory.c				\
					mmap.c					\
					mmap_info.c				\
					mmap_init.c				\
					mmap_book.c				\
					mmap_chunk.c			\
					mmap_free.c				\
					v_memory.c				\
					utils.c)					


SOURCES =	boot/boot.s \
			boot/boot_infos.c \
			builtins/reboot.c \
			builtins/echo.c \
			builtins/hexdump.c \
			builtins/interrupt.c \
			builtins/memory_infos.c \
			builtins/qwerty.c \
			builtins/readline.c \
			builtins/registers.c \
			builtins/test_stack_dump.c \
			builtins/uptime.c \
		  	gdt/gdt.c \
			gdt/setgdt.s \
			interrupts/interrupts.c \
			interrupts/pic.c \
			interrupts/pit.s \
			interrupts/interrupts_test.c \
			interrupts/isrstub.s \
			kernel/kernel.c \
			kernel/kernel_asm_utils.c \
			kernel/kernel_utils.c \
			keyboard/keycode.c \
			keyboard/keypress.c \
			keyboard/scancode.c \
			$(SOURCES_MEMORY) \
			panic/panic.c \
			panic/dump_stack.c \
			terminal/terminal.c \
			terminal/putchar.c \
			terminal/vga.c \
			tss/processus.c \
			tss/tss.c \
			tss/tss_asm.s \
			tss/fork.c \
			$(SOURCES_LIBS)

HEADER_FILES = builtin.h \
			   interrupts.h \
			   kernel.h \
			   keycode.h \
			   pic.h \
			   printk.h \
			   terminal.h

SRCS = $(addprefix $(SRCS_DIR)/, $(SOURCES))

OBJS = $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o,$(patsubst $(SRCS_DIR)/%.s,$(OBJS_DIR)/%.o,$(SRCS)))
DEPS = $(patsubst $(SRCS_DIR)/%.c,$(DEPS_DIR)/%.d,$(patsubst $(SRCS_DIR)/%.s,$(DEPS_DIR)/%.d,$(SRCS)))
INCS = -I $(INCS_DIR)
INCS_FILES = $(addprefix $(INCS_DIR)/, $(HEADER_FILES))

ifneq ($(shell $(DOCKER) image list --format '{{.Repository}}' | grep -w $(DOCKER_CONT_NAME)),)
all: $(PROJECT).iso
else
all:
	@echo "Image '$(DOCKER_CONT_NAME)' does not exist."
	@echo "Pulling '$(DOCKER_CONT_NAME)' from Docker Hub."
	@docker pull $(DOCKER_CONT_NAME)
endif

$(PROJECT).iso: $(PROJECT).bin $(KFS_CONFIG_DIR)/grub.cfg
	@echo "Creating ISO..."
	@mkdir -p $(KFS_BUILD_DIR)/boot/grub
	@cp $(PROJECT).bin $(KFS_BUILD_DIR)/boot/$(PROJECT).bin
	@cp $(KFS_CONFIG_DIR)/grub.cfg $(KFS_BUILD_DIR)/boot/grub/grub.cfg	
	@$(DOCKER_EXEC) grub-mkrescue -o $(PROJECT).iso --compress=gz $(KFS_BUILD_DIR) -quiet
	@echo "Done."

$(PROJECT).bin: $(OBJS) $(KFS_CONFIG_DIR)/linker.ld
	@echo "Linking files..."
	@$(DOCKER_EXEC) i386-elf-gcc \
		$(INCS) \
		-T $(KFS_CONFIG_DIR)/linker.ld \
		-o $(PROJECT).bin \
		-ffreestanding $(FLAG_DEBUG) $(OPT_FLAG) -nostdlib /app/$(OBJS) /app/$(LIBKFS) -lgcc

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.s $(INCS_FILES) $(LIBKFS)
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@$(DOCKER_EXEC) nasm -g -f elf32 $(INCS) $(LIBKFS_INCS) $< -o $@ 

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c $(INCS_FILES) $(LIBKFS)
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@mkdir -p $(patsubst $(OBJS_DIR)/%,$(DEPS_DIR)/%,$(dir ./$(@:.o=.d)))
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) $(LIBKFS_INCS) \
						-MMD -MF /app/$(patsubst $(SRCS_DIR)/%,$(DEPS_DIR)/%,./$(<:.c=.d)) \
						$(FLAG) \
						-std=gnu99 -ffreestanding -Og \
						-c /app/$< \
						-o /app/$@

$(LIBKFS):
	make -C $(LIBKFS_DIR)


clean:
	@echo "Cleaning files..."
	@$(RM) -rf $(KFS_BUILD_DIR)
	@$(RM) -rf $(OBJS_DIR)
	@$(RM) -rf $(DEPS_DIR)

fclean: clean
	@echo "Cleaning build..."
	@$(RM) -f $(PROJECT).iso
	@$(RM) -f $(PROJECT).bin
	make -C $(LIBKFS_DIR) fclean

re: fclean all

build_docker:
	@$(DOCKER) build ./config -t $(DOCKER_CONT_NAME)

start_iso: $(PROJECT).iso
	@echo "Starting ISO..."
	@qemu-system-i386 -boot d -cdrom $(PROJECT).iso

start_bin: $(PROJECT).bin
	@echo "Starting BIN..."
	@qemu-system-i386 -kernel $(PROJECT).bin

debug: $(PROJECT).bin
	@echo "starting in debug mode\n(gdb) target remote localhost:1234\n"
	ls -al ./config
	@gnome-terminal -- $(shell pwd)/config/debug.sh 
	@qemu-system-i386 -s -S -kernel $(PROJECT).bin 

-include $(DEPS)

.PHONY: all build_docker clean debug fclean re start_iso start_bin
