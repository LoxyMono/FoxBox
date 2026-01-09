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

VOID StringCopy(CHAR *dest, const CHAR *src, DWORD count)
{
    for (DWORD i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }
}

BOOL StringCompare(const CHAR *first, const CHAR *second, DWORD count)
{
    for (DWORD i = 0; i < count; i++)
    {
        if (first[i] != second[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

VOID CopyStringUntil(CHAR *dest, const CHAR *src, CHAR stopChar)
{
    DWORD i = 0;
    while (src[i] != stopChar && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
}

DWORD CharsUntilDelimiter(const CHAR *string, CHAR stopChar)
{
    DWORD count = 0;
    while (string[count] != stopChar && string[count] != '\0')
    {
        count++;
    }
    return count;
}