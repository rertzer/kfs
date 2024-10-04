PROJECT = jros
PROJECT_DIR = .
RM = /bin/rm
DOCKER_DIR = /app
KFS_BUILD_DIR = isodir
DOCKER_EXEC = docker run --rm -v $(PROJECT_DIR):$(DOCKER_DIR) debian-kfs1

SRCS_DIR = $(PROJECT_DIR)/srcs
OBJS_DIR = $(PROJECT_DIR)/objs
INCS_DIR = $(PROJECT_DIR)/incs

SRCS = $(SRCS_DIR)/kernel.c
OBJS = $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(SRCS:.c=.o))
INCS = -I $(INCS_DIR)

all: $(PROJECT).iso

$(PROJECT).iso: $(PROJECT).bin grub.cfg
	@echo "Creating ISO..."
	@mkdir -p $(KFS_BUILD_DIR)/boot/grub
	@cp $(PROJECT).bin $(KFS_BUILD_DIR)/boot/$(PROJECT).bin
	@cp grub.cfg $(KFS_BUILD_DIR)/boot/grub/grub.cfg	
	@$(DOCKER_EXEC) grub-mkrescue -o $(PROJECT).iso $(KFS_BUILD_DIR) -quiet
	@echo "Done."

boot.o: boot.s
	@echo "Compiling boot.s..."
	@$(DOCKER_EXEC) nasm -f elf32 boot.s -o boot.o

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) -c /app/$< -o /app/$@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

$(PROJECT).bin: boot.o $(OBJS)
	@echo "Linking files..."
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) -T linker.ld -o $(PROJECT).bin -ffreestanding -O2 -nostdlib boot.o /app/$(OBJS) -lgcc

clean:
	@echo "Cleaning files..."
	@$(RM) -rf $(KFS_BUILD_DIR)
	@$(RM) -rf $(OBJS_DIR)

fclean: clean
	@echo "Cleaning build..."
	@$(RM) -f $(PROJECT).iso
	@$(RM) -f $(PROJECT).bin

re: fclean all

start_iso: $(PROJECT).iso
	@echo "Starting ISO..."
	@qemu-system-i386 -cdrom $(PROJECT).iso

start_bin: $(PROJECT).bin
	@echo "Starting BIN..."
	@qemu-system-i386 -kernel $(PROJECT).bin
