#include <Video/VGA.h>

#include <stdarg.h>

static WORD *mVgaMemory = NULL_PTR;

static BYTE mCursorX = 0;
static BYTE mCursorY = 0;
static BYTE mCurrentColor = VGA_COLOR_LIGHTGREY | (VGA_COLOR_BLACK << 4);

VOID VgaInit()
{

#ifdef SILENT
    return;
#endif

    mVgaMemory = (WORD *)VGA_MEMORY_ADDRESS;
    VgaClear();
}

VOID VgaClear()
{

#ifdef SILENT
    return;
#endif

    WORD blank = (mCurrentColor << 8) | '\0';
    for (DWORD i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        mVgaMemory[i] = blank;
    }

    VgaSetCursorEnabled(TRUE);
    VgaSetCursorPosition(0, 0);
}

VOID VgaResetColor()
{

#ifdef SILENT
    return;
#endif

    VgaSetColor(VGA_COLOR_LIGHTGREY, VGA_COLOR_BLACK);
}

VOID VgaSetColor(BYTE foreground, BYTE background)
{

#ifdef SILENT
    return;
#endif

    mCurrentColor = foreground | (background << 4);
}

VOID VgaSetCursorPosition(BYTE x, BYTE y)
{

#ifdef SILENT
    return;
#endif

    mCursorX = x;
    mCursorY = y;

    WORD position = (mCursorY * VGA_WIDTH) + mCursorX;

    OutByte(0x3D4, 0x0F);
    OutByte(0x3D5, (BYTE)(position & 0xFF));
    OutByte(0x3D4, 0x0E);
    OutByte(0x3D5, (BYTE)((position >> 8) & 0xFF));
}

VOID VgaSetCursorEnabled(BOOL enabled)
{

#ifdef SILENT
    return;
#endif

    if (enabled)
    {
        OutByte(0x3D4, 0x0A);
        OutByte(0x3D5, (InByte(0x3D5) & 0xC0) | 14);
        OutByte(0x3D4, 0x0B);
        OutByte(0x3D5, (InByte(0x3D5) & 0xE0) | 15);
    }
    else
    {
        OutByte(0x3D4, 0x0A);
        OutByte(0x3D5, 0x20);
    }
}

VOID VgaPutCharAt(CHAR character, BYTE x, BYTE y)
{

#ifdef SILENT
    return;
#endif

    WORD entry = (WORD)character | (mCurrentColor << 8);
    mVgaMemory[(y * VGA_WIDTH) + x] = entry;
}

VOID VgaPutChar(CHAR character)
{

#ifdef SILENT
    return;
#endif

    if (character == '\n')
    {
        mCursorY++;
    }
    else if (character == '\r')
    {
        mCursorX = 0;
    }
    else if (character == '\t')
    {
        mCursorX += 4;
    }
    else
    {
        VgaPutCharAt(character, mCursorX, mCursorY);
        mCursorX++;
        if (mCursorX >= VGA_WIDTH)
        {
            mCursorX = 0;
            mCursorY++;
        }
    }

    if (mCursorY >= VGA_HEIGHT)
    {
        VgaScroll();
        mCursorY = VGA_HEIGHT - 1;
    }

    VgaSetCursorPosition(mCursorX, mCursorY);
}

VOID VgaPrintStringC(const CHAR *string, DWORD count)
{

#ifdef SILENT
    return;
#endif

    for (DWORD i = 0; i < count; i++)
    {
        VgaPutChar(string[i]);
    }
}

VOID VgaPrintString(const CHAR *string, ...)
{

#ifdef SILENT
    return;
#endif

    va_list args;
    va_start(args, string);

    while (*string != '\0')
    {
        if (*string == '%')
        {
            BYTE fmtSize = 0;
            
            string++;

            if (*string >= '0' || *string <= '9')
            {
                while (*string >= '0' && *string <= '9')
                {
                    fmtSize = (fmtSize * 10) + (*string - '0');
                    string++;
                }
            }

            switch (*string)
            {
            case 'c':
            {
                CHAR c = (CHAR)va_arg(args, DWORD);
                VgaPutChar(c);
                break;
            }
            case 's':
            {
                const CHAR *str = va_arg(args, const CHAR *);
                
                VgaPrintStringC(str, StringLength(str));

                break;
            }
            case 'x':
            {
                DWORD value = va_arg(args, DWORD);
                CHAR *hexStr = DwordToHexString(value, fmtSize);
                VgaPrintStringC(hexStr, StringLength(hexStr));
                break;
            }
            case 'd':
            {
                I32 value = va_arg(args, I32);
                CHAR *decStr = DwordToDecimalString(value, fmtSize, TRUE);
                VgaPrintStringC(decStr, StringLength(decStr));
                break;
            }
            case 'u':
            {
                DWORD value = va_arg(args, DWORD);
                CHAR *decStr = DwordToDecimalString((DWORD)value, fmtSize, FALSE);
                VgaPrintStringC(decStr, StringLength(decStr));
                break;
            }
            default:
            {
                VgaPutChar(*string);
                break;
            }
            }
        }
        else
        {
            VgaPutChar(*string);
        }
        string++;
    }

    va_end(args);
}

VOID VgaScroll()
{

#ifdef SILENT
    return;
#endif

    for (DWORD y = 1; y < VGA_HEIGHT; y++)
    {
        for (DWORD x = 0; x < VGA_WIDTH; x++)
        {
            WORD entry = mVgaMemory[(y * VGA_WIDTH) + x];
            mVgaMemory[((y - 1) * VGA_WIDTH) + x] = entry;
        }
    }

    WORD blank = (mCurrentColor << 8) | '\0';
    for (DWORD x = 0; x < VGA_WIDTH; x++)
    {
        mVgaMemory[((VGA_HEIGHT - 1) * VGA_WIDTH) + x] = blank;
    }
}