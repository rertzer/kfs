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

SOURCES =	kernel.c \
		  	keyboard.c \
			keycode.c \
			printk.c \
		  	gdt.c \
			setgdt.s \
			boot.s

SRCS = $(addprefix $(SRCS_DIR)/, $(SOURCES))

OBJS = $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o,$(patsubst $(SRCS_DIR)/%.s,$(OBJS_DIR)/%.o,$(SRCS)))
INCS = -I $(INCS_DIR)

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

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.s
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@$(DOCKER_EXEC) nasm -f elf32 $< -o $@

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) -c /app/$< -o /app/$@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

$(PROJECT).bin: $(OBJS) $(KFS_CONFIG_DIR)/linker.ld
	@echo "Linking files..."
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) -T $(KFS_CONFIG_DIR)/linker.ld -o $(PROJECT).bin -ffreestanding -O2 -nostdlib /app/$(OBJS) -lgcc

clean:
	@echo "Cleaning files..."
	@$(RM) -rf $(KFS_BUILD_DIR)
	@$(RM) -rf $(OBJS_DIR)

fclean: clean
	@echo "Cleaning build..."
	@$(RM) -f $(PROJECT).iso
	@$(RM) -f $(PROJECT).bin

re: fclean all

build_docker:
	@$(DOCKER) build ./config -t $(DOCKER_CONT_NAME)

start_iso: $(PROJECT).iso
	@echo "Starting ISO..."
	@qemu-system-i386 -cdrom $(PROJECT).iso

start_bin: $(PROJECT).bin
	@echo "Starting BIN..."
	@qemu-system-i386 -kernel $(PROJECT).bin
