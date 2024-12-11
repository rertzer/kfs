PROJECT = jros
PROJECT_DIR = .

RM = /bin/rm
KFS_BUILD_DIR = isodir
KFS_CONFIG_DIR = config

DOCKER_DIR = /app
DOCKER = /usr/bin/docker
DOCKER_CONT_NAME = debian-kfs1
DOCKER_EXEC = $(DOCKER) run --rm -v $(PROJECT_DIR):$(DOCKER_DIR) $(DOCKER_CONT_NAME)

SRCS_DIR = $(PROJECT_DIR)/srcs
OBJS_DIR = $(PROJECT_DIR)/objs
INCS_DIR = $(PROJECT_DIR)/incs
DEPS_DIR = $(PROJECT_DIR)/deps

PRINTK_SOURCE = printk/printk.c \
				printk/hexa.c \
				printk/integer.c \
				printk/string.c \
				printk/pointer.c \
				printk/unsigned.c \
				printk/utils.c \
				printk/options.c

SOURCES =	kernel.c \
			keycode.c \
			keypress.c \
			terminal.c \
			readline.c \
			vga.c \
			scancode.c \
		  	gdt.c \
			pic.c \
			interrupts.c \
			kernel_utils.c \
			utils.c \
			echo.c \
			qwerty.c \
			reboot.c \
			registers.c \
			boot_infos.c \
			setgdt.s \
			isrstub.s \
			boot.s \
			pit.s \
			hexdump.c 

SOURCES += $(PRINTK_SOURCE)

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
	@echo "Use 'make build_docker' to create it."
endif

$(PROJECT).iso: $(PROJECT).bin $(KFS_CONFIG_DIR)/grub.cfg
	@echo "Creating ISO..."
	@mkdir -p $(KFS_BUILD_DIR)/boot/grub
	@cp $(PROJECT).bin $(KFS_BUILD_DIR)/boot/$(PROJECT).bin
	@cp $(KFS_CONFIG_DIR)/grub.cfg $(KFS_BUILD_DIR)/boot/grub/grub.cfg	
	@$(DOCKER_EXEC) grub-mkrescue -o $(PROJECT).iso $(KFS_BUILD_DIR) -quiet
	@echo "Done."

$(PROJECT).bin: $(OBJS) $(KFS_CONFIG_DIR)/linker.ld
	@echo "Linking files..."
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) -T $(KFS_CONFIG_DIR)/linker.ld -o $(PROJECT).bin -ffreestanding -Og -nostdlib /app/$(OBJS) -lgcc


$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.s $(INCS_FILES) 
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@$(DOCKER_EXEC) nasm -g -f elf32 $(INCS) $< -o $@

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c $(INCS_FILES)
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@mkdir -p $(patsubst $(OBJS_DIR)/%,$(DEPS_DIR)/%,$(dir ./$(@:.o=.d)))
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) \
						-MMD -MF /app/$(patsubst $(SRCS_DIR)/%,$(DEPS_DIR)/%,./$(<:.c=.d)) \
						-std=gnu99 -ffreestanding -Og -Wall -Wextra \
						-c /app/$< \
						-o /app/$@


clean:
	@echo "Cleaning files..."
	@$(RM) -rf $(KFS_BUILD_DIR)
	@$(RM) -rf $(OBJS_DIR)
	@$(RM) -rf $(DEPS_DIR)

fclean: clean
	@echo "Cleaning build..."
	@$(RM) -f $(PROJECT).iso
	@$(RM) -f $(PROJECT).bin

re: fclean all

build_docker:
	@$(DOCKER) build ./config -t $(DOCKER_CONT_NAME)

start_iso: $(PROJECT).iso
	@echo "Starting ISO..."
	@qemu-system-i386 -boot d -cdrom $(PROJECT).iso

start_bin: $(PROJECT).bin
	@echo "Starting BIN..."
	@qemu-system-i386 -kernel $(PROJECT).bin

-include $(DEPS)

.PHONY: all clean fclean re build_docker start_iso start_bin
