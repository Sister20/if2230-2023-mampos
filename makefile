# Compiler & linker
ASM           = nasm
LIN           = ld
CC            = gcc

# Directory
SOURCE_FOLDER = src
OUTPUT_FOLDER = bin
ISO_NAME      = OS2023

# Flags
WARNING_CFLAG = -Wall -Wextra -Werror
DEBUG_CFLAG   = -ffreestanding -fshort-wchar -g
STRIP_CFLAG   = -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
CFLAGS        = $(DEBUG_CFLAG) $(WARNING_CFLAG) $(STRIP_CFLAG) -m32 -c -I$(SOURCE_FOLDER)
AFLAGS        = -f elf32 -g -F dwarf
LFLAGS        = -T $(SOURCE_FOLDER)/linker.ld -melf_i386

DISK_NAME      = storage

run: all
	@qemu-system-i386 -s -drive file=bin/storage.bin,format=raw,if=ide,index=0,media=disk -cdrom $(OUTPUT_FOLDER)/$(ISO_NAME).iso
all: build
build: iso
clean:
	rm -rf *.o *.iso $(OUTPUT_FOLDER)/kernel

disk:
	@qemu-img create -f raw $(OUTPUT_FOLDER)/$(DISK_NAME).bin 4M

kernel: disk
	$(ASM) $(AFLAGS) $(SOURCE_FOLDER)/kernel_loader.s -o $(OUTPUT_FOLDER)/kernel_loader.o
	$(ASM) $(AFLAGS) $(SOURCE_FOLDER)/interrupt/intsetup.s -o $(OUTPUT_FOLDER)/intsetup.o
# TODO: Compile C file with CFLAGS
	@$(CC) $(CFLAGS) src/kernel.c -o bin/kernel.o
	@$(CC) $(CFLAGS) src/portio.c -o bin/portio.o
	@$(CC) $(CFLAGS) src/lib-header/gdt.c -o bin/gdt.o
	@$(CC) $(CFLAGS) src/lib-header/framebuffer.c -o bin/framebuffer.o  
	@$(CC) $(CFLAGS) src/stdmem.c -o bin/stdmem.o  
	@$(CC) $(CFLAGS) src/interrupt/idt.c -o bin/idt.o 
	@$(CC) $(CFLAGS) src/interrupt/interrupt.c -o bin/interrupt.o 
	@$(CC) $(CFLAGS) src/keyboard/keyboard.c -o bin/keyboard.o
	@$(CC) $(CFLAGS) src/filesystem/disk.c -o bin/disk.o
	@$(CC) $(CFLAGS) src/filesystem/fat32.c -o bin/fat32.o
	@$(LIN) $(LFLAGS) bin/*.o -o $(OUTPUT_FOLDER)/kernel
	@echo Linking object files and generate elf32...
	@rm -f *.o

iso: kernel
	@mkdir -p $(OUTPUT_FOLDER)/iso/boot/grub
	@cp $(OUTPUT_FOLDER)/kernel     $(OUTPUT_FOLDER)/iso/boot/
	@cp other/grub1                 $(OUTPUT_FOLDER)/iso/boot/grub/
	@cp $(SOURCE_FOLDER)/menu.lst   $(OUTPUT_FOLDER)/iso/boot/grub/
# TODO: Create ISO image
	genisoimage -R                 \
		-b boot/grub/grub1         \
		-no-emul-boot              \
		-boot-load-size 4          \
		-A os                      \
		-input-charset utf8        \
		-quiet                     \
		-boot-info-table           \
		-o bin/OS2023.iso          \
		bin/iso
	@rm -r $(OUTPUT_FOLDER)/iso/