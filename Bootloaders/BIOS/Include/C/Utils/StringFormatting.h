#pragma once

#include <Types.h>

#include <Utils/String.h>

CHAR *DwordToHexString(DWORD value, BYTE count);
CHAR *DwordToDecimalString(DWORD value, BYTE count, BOOL isSigned);