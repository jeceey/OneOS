IMAGE_NAME = oneos

.PHONY: all build_docker compile clean run

all: build_docker compile

build_docker:
	docker build -t $(IMAGE_NAME) .

compile:
	docker run --rm -v "$(PWD)":/os $(IMAGE_NAME) bash -c "\
		nasm -f elf32 src/boot.asm -o boot.o && \
		gcc -m32 -c src/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra && \
		ld -m elf_i386 -T src/linker.ld -o oneos.bin boot.o kernel.o && \
		mkdir -p iso/boot/grub && \
		cp oneos.bin iso/boot/ && \
		echo 'set default=0' > iso/boot/grub/grub.cfg && \
		echo 'set timeout=0' >> iso/boot/grub/grub.cfg && \
		echo 'menuentry \"OneOS\" {' >> iso/boot/grub/grub.cfg && \
		echo '  multiboot /boot/oneos.bin' >> iso/boot/grub/grub.cfg && \
		echo '}' >> iso/boot/grub/grub.cfg && \
		grub-mkrescue -o oneos.iso iso"

run:
	qemu-system-i386 -cdrom oneos.iso

clean:
	rm -rf *.o *.bin *.iso iso