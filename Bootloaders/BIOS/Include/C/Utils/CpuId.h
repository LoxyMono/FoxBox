#pragma once

#include <Types.h>

extern I32 CpuIdAvailability();

/// Wrapper for the __get_cpuid intrinsic
VOID CpuId(DWORD function, DWORD *eax, DWORD *ebx, DWORD *ecx, DWORD *edx);