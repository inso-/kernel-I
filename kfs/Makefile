CFLAGS += -nostdinc -fno-builtin #-m32
CFLAGS += -fno-stack-protector
LDFLAGS += -nostdlib -Wl,--build-id=none #-m32 
CC = gcc $(CFLAGS)

kernel.elf: src/main.o src/crt0.o
	$(CC) -o kernel.elf src/main.o src/crt0.o $(LDFLAGS)

all:	kernel.elf

clean:
	rm -f src/*.o kernel.elf

boot: kernel.elf
	$(QEMU) -fda kernel.elf

debug: src/main.o src/crt0.o
	$(CC) -o kernel.elf src/main.o src/crt0.o $(LDFLAGS) -g
	$(QEMU) -fda kernel.elf -S
