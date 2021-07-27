#pragma once

enum eAllocCheckOutput
{
    ACOutput_Simple = 0x0,
    ACOutput_Advanced = 0x1,
};

int InitAllocCheck(eAllocCheckOutput, int, unsigned long);