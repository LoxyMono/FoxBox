#include <Utils/String.h>

DWORD StringLength(const CHAR *string)
{
    DWORD length = 0;
    
    while (string[length] != '\0')
    {
        length++;
    }
    
    return length;
}

VOID StringSet(VOID *dest, BYTE value, DWORD count)
{
    BYTE *ptr = (BYTE *)dest;
    
    for (DWORD i = 0; i < count; i++)
    {
        ptr[i] = value;
    }
}