#include <Utils/CpuId.h>

#include <cpuid.h>

VOID CpuId(DWORD function, DWORD *eax, DWORD *ebx, DWORD *ecx, DWORD *edx)
{
    __get_cpuid(function, eax, ebx, ecx, edx);
}