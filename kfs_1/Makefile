PROJECT = jros
PROJECT_DIR = $(shell pwd)
RM = /bin/rm
KFS_BUILD_DIR = isodir
DOCKER_EXEC = docker run --rm -v $(PROJECT_DIR):/app debian-kfs1

all: $(PROJECT).iso

$(PROJECT).iso: $(PROJECT).bin grub.cfg
	@echo "Creating ISO..."
	@mkdir -p $(KFS_BUILD_DIR)/boot/grub
	@cp $(PROJECT).bin $(KFS_BUILD_DIR)/boot/$(PROJECT).bin
	@cp grub.cfg $(KFS_BUILD_DIR)/boot/grub/grub.cfg	
	@$(DOCKER_EXEC) grub-mkrescue -o $(PROJECT).iso $(KFS_BUILD_DIR) -quiet
	@echo "Done."

boot.o: boot.s
	@$(DOCKER_EXEC) nasm -f elf32 boot.s -o boot.o

kernel.o: kernel_main.c
	@$(DOCKER_EXEC) i386-elf-gcc -c kernel_main.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

$(PROJECT).bin: boot.o kernel.o
	@$(DOCKER_EXEC) i386-elf-gcc -T linker.ld -o $(PROJECT).bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

clean:
	@echo "Cleaning files..."
	@$(RM) -rf $(KFS_BUILD_DIR)
	@$(RM) -f *.o

fclean: clean
	@echo "Cleaning build..."
	@$(RM) -f $(PROJECT).iso
	@$(RM) -f $(PROJECT).bin

re: clean all

start_iso: $(PROJECT).iso
	@echo "Starting ISO..."
	@qemu-system-i386 -cdrom $(PROJECT).iso

start_bin: $(PROJECT).bin
	@echo "Starting BIN..."
	@qemu-system-i386 -kernel $(PROJECT).bin
