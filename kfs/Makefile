CFLAGS += -nostdinc -fno-builtin -m32
CFLAGS += -fno-stack-protector
LDFLAGS += -nostdlib -melf_i386 --build-id=none
CC = gcc $(CFLAGS)
QEMU = qemu-system-i386

kernel.elf: src/main.o src/crt0.o
	$(LD) -o kernel.elf src/main.o src/crt0.o $(LDFLAGS) -T kfs.ld

all:	kernel.elf

clean:
	rm -f src/*.o kernel.elf

boot: kernel.elf
	$(QEMU) -kernel kernel.elf -serial stdio

debug: src/main.o src/crt0.o
	$(LD) -o kernel.elf -g src/main.o src/crt0.o $(LDFLAGS) -T kfs.ld
	$(QEMU) -kernel kernel.elf -serial stdio -s -S
