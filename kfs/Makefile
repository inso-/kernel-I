CFLAGS += -nostdinc -fno-builtin -m32
CFLAGS += -fno-stack-protector
LDFLAGS += -nostdlib -m32 -Wl,--build-id=none
QEMU = qemu-system-x86_64
CC = gcc $(CFLAGS)

kernel.elf: src/main.o src/crt0.o
	$(CC) -o kernel.elf src/main.o src/crt0.o $(LDFLAGS)

all:	kernel.elf

clean:
	rm -f src/*.o kernel.elf

boot: kernel.elf
	$(QEMU) -fda kernel.elf

debug:
