#pragma once

#include <Types.h>

DWORD StringLength(const CHAR *string);

VOID StringSet(VOID *dest, BYTE value, DWORD count);
VOID StringCopy(CHAR *dest, const CHAR *src, DWORD count);
BOOL StringCompare(const CHAR *first, const CHAR *second, DWORD count);