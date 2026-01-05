extern "C" void FoxBoxMain()
{
    asm volatile("1: hlt; jmp 1b");
}