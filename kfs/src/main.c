void            outb(char val, unsigned short port)
{
    asm volatile("outb %0, %1\n\t"
                 : /* No output */
                 : "a" (val), "d" (port));
}

void write_serial(char a)
{
    outb(a,0x3f8); /* COM1 */
}

void init_serial() {
    unsigned short port = 0x3f8;

    outb(0x00, port + 1);    // Disable all interrupts
    outb(0x80, port + 3);    // Enable DLAB (set baud rate divisor)
    outb(0x03, port + 0);    // Set divisor to 3 (lo byte) 38400 baud
    outb(0x00, port + 1);    //                  (hi byte)
    outb(0x03, port + 3);    // 8 bits, no parity, one stop bit
}

void printk(const char *msg)
{
    volatile char *video = (volatile char*)0xB8500;

    while (*msg != 0)
    {
        write_serial(*msg);
        *video++ = *msg++;
        *video++ = 42;
    }
}

int main()
{
    init_serial();
    printk("Hello World!");
    return 0;
}
