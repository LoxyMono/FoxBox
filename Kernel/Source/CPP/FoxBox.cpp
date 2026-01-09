extern "C" void FoxBoxMain()
{
    *(volatile short *)0xB8000 = 'L';

    asm volatile("1: hlt; jmp 1b");
}