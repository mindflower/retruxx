#pragma once
#include "stringm3d.h"

class StrW
{
public:
    class ZeroCharHolder
    {
    private:
        wchar_t m_zeroChar;
    };

public:
    CStr ToStr(unsigned int) const;

private:
    wchar_t* m_charPtr;
    int m_allocSz;
    ZeroCharHolder ZERO;
};
