#pragma once

#include <Types.h>

#include <IO/Ports.h>

#include <Video/VGA.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY_ADDRESS 0xB8000

enum VGA_COLOR
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_ORANGE = 6,
    VGA_COLOR_LIGHTGREY = 7,
    VGA_COLOR_DARKGREY = 8,
    VGA_COLOR_LIGHTBLUE = 9,
    VGA_COLOR_LIGHTGREEN = 10,
    VGA_COLOR_LIGHTCYAN = 11,
    VGA_COLOR_LIGHTRED = 12,
    VGA_COLOR_LIGHTMAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
} VGA_COLOR;

VOID VgaInit();
VOID VgaClear();

VOID VgaResetColor();
VOID VgaSetColor(BYTE foreground, BYTE background);

VOID VgaSetCursorPosition(BYTE x, BYTE y);
VOID VgaSetCursorEnabled(BOOL enabled);

VOID VgaPutCharAt(CHAR character, BYTE x, BYTE y);
VOID VgaPutChar(CHAR character);
VOID VgaPrintStringC(const CHAR *string, DWORD count);
VOID VgaPrintString(const CHAR *string);

VOID VgaScroll();