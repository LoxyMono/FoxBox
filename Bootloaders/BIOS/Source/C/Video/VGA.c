#include <Video/VGA.h>

static WORD *mVgaMemory = NULL_PTR;

static BYTE mCursorX = 0;
static BYTE mCursorY = 0;
static BYTE mCurrentColor = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);

VOID VgaInit()
{
    mVgaMemory = (WORD *)VGA_MEMORY_ADDRESS;
    VgaClear();
}

VOID VgaClear()
{
    WORD blank = (mCurrentColor << 8) | '\0';
    for (DWORD i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        mVgaMemory[i] = blank;
    }

    VgaSetCursorEnabled(true);
    VgaSetCursorPosition(0, 0);
}

VOID VgaResetColor()
{
    VgaSetColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

VOID VgaSetColor(BYTE foreground, BYTE background)
{
    mCurrentColor = foreground | (background << 4);
}

VOID VgaSetCursorPosition(BYTE x, BYTE y)
{
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
    WORD entry = (WORD)character | (mCurrentColor << 8);
    mVgaMemory[(y * VGA_WIDTH) + x] = entry;
}

VOID VgaPutChar(CHAR character)
{
    if (character == '\n')
    {
        mCursorY++;
    }
    else if (character == '\r')
    {
        mCursorX = 0;
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
}

VOID VgaPrintStringC(const CHAR *string, DWORD count)
{
    for (DWORD i = 0; i < count; i++)
    {
        VgaPutChar(string[i]);
    }

    VgaSetCursorPosition(mCursorX, mCursorY);
}

VOID VgaPrintString(const CHAR *string)
{
    DWORD length = StringLength(string);
    VgaPrintStringC(string, length);
}

VOID VgaScroll()
{
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