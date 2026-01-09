#include <Utils/StringFormatting.h>

static CHAR FormatBuffer[64] = { 0 };
static const CHAR HexDigits[] = "0123456789ABCDEF";

CHAR *DwordToHexString(DWORD value, BYTE count)
{
    I32 i = 0;

    if (count > 8)
    {
        count = 8;
    }
    if (count < 0)
    {
        count = 0;
    }

    if (count <= 0)
    {
        DWORD tmp = value;
        count = 0;

        do
        {
            count++;
            tmp >>= 4;
        } while (tmp);
    }

    for (i = count - 1; i >= 0; i--)
    {
        FormatBuffer[i] = HexDigits[value & 0xF];
        value >>= 4;
    }

    FormatBuffer[count] = '\0';
    return FormatBuffer;
}

CHAR *DwordToDecimalString(DWORD value, BYTE count, BOOL isSigned)
{
    StringSet(FormatBuffer, 0x00, 64);
    I32 i = 0;

    BOOL negative = FALSE;
    if (isSigned && ((I32)value) < 0)
    {
        negative = TRUE;
        value = (DWORD)(-((I32)value));
    }

    do
    {
        FormatBuffer[i++] = (CHAR)((value % 10) + '0');
        value /= 10;
    } while (value > 0);

    if (negative)
    {
        FormatBuffer[i++] = '-';
    }
    while (i < count)
    {
        FormatBuffer[i++] = '0';
    }

    FormatBuffer[i] = '\0';

    for (I32 j = 0; j < i / 2; j++)
    {
        CHAR temp = FormatBuffer[j];
        FormatBuffer[j] = FormatBuffer[i - j - 1];
        FormatBuffer[i - j - 1] = temp;
    }
    
    return FormatBuffer;
}