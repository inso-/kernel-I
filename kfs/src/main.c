void printk(const char *msg)
{
    volatile char *video = (volatile char*)0xB8500;

    while (*msg != 0)
    {
        *video++ = *msg++;
        *video++ = 42;
    }
}

int main()
{
    printk("Hello World!");
    return 0;
}
