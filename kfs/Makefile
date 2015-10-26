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
	$(QEMU) -kernel kernel.elf -m 800

debug: src/main.o src/crt0.o
	$(CC) -o kernel.elf src/main.o src/crt0.o $(LDFLAGS) -g
	$(QEMU) -kernel kernel.elf -m 800 -S
