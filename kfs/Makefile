CFLAGS += -nostdinc -fno-builtin -m32
CFLAGS += -fno-stack-protector
LDFLAGS += -nostdlib -Wl,--build-id=none -m32
CC = gcc $(CFLAGS)
QEMU = qemu-system-i386

kernel.elf: src/main.o src/crt0.o
	$(CC) -o kernel.elf src/main.o src/crt0.o $(LDFLAGS)

all:	kernel.elf

clean:
	rm -f src/*.o kernel.elf

boot: kernel.elf
	$(QEMU) -kernel kernel.elf -serial stdio -m 8000

debug: src/main.o src/crt0.o
	$(CC) -o kernel.elf -g src/main.o src/crt0.o $(LDFLAGS)
	$(QEMU) -kernel kernel.elf -m 800 -serial stdio -S
