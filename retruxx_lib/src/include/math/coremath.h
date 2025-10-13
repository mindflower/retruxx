#pragma once

//TODO: add static functions

class CBrezLine
{
public:
    int start(int srcx, int srcy, int dstx, int dsty);
    int step(int& curx, int& cury);

protected:
    /* 0x0000 */ int m_x0;
    /* 0x0004 */ int m_x1;
    /* 0x0008 */ int m_y0;
    /* 0x000c */ int m_y1;
    /* 0x0010 */ int m_numsteps;
    /* 0x0014 */ int m_d;
    /* 0x0018 */ int m_dinc0;
    /* 0x001c */ int m_dinc1;
    /* 0x0020 */ int m_xinc0;
    /* 0x0024 */ int m_xinc1;
    /* 0x0028 */ int m_yinc0;
    /* 0x002c */ int m_yinc1;
    /* 0x0030 */ int m_x;
    /* 0x0034 */ int m_y;
    /* 0x0038 */ int m_i;
}; /* size: 0x003c */

inline int CBrezLine::start(int srcx, int srcy, int dstx, int dsty)
{
    this->m_y1 = dsty;
    auto v5 = dstx - srcx;
    auto v6 = dsty - srcy;
    this->m_x0 = srcx;
    this->m_x1 = dstx;
    this->m_y0 = srcy;
    if (dstx - srcx < 0)
        v5 = srcx - dstx;
    if (v6 < 0)
        v6 = srcy - dsty;

    int v7 = 0;
    if (v5 <= v6)
    {
        this->m_numsteps = v6 + 1;
        this->m_d = 2 * v5 - v6;
        this->m_dinc1 = (v5 - v6) >> 1;
        v7 = v5 >> 1;
        this->m_xinc1 = 1;
        this->m_yinc1 = 1;
        this->m_yinc0 = 1;
        this->m_xinc0 = 0;
    }
    else
    {
        this->m_numsteps = v5 + 1;
        this->m_d = 2 * v6 - v5;
        v7 = v6 >> 1;
        this->m_xinc0 = 1;
        this->m_xinc1 = 1;
        this->m_yinc1 = 1;
        this->m_dinc1 = (v6 - v5) >> 1;
        this->m_yinc0 = 0;
    }
    this->m_dinc0 = v7;
    if (srcx > dstx)
    {
        this->m_xinc0 = -this->m_xinc0;
        this->m_xinc1 = -1;
    }
    if (srcy > dsty)
    {
        this->m_yinc0 = -this->m_yinc0;
        this->m_yinc1 = -1;
    }
    this->m_x = srcx;
    this->m_y = srcy;
    this->m_i = 0;
    return this->m_numsteps;
}

inline int CBrezLine::step(int& curx, int& cury)
{
    if (m_i >= this->m_numsteps)
        return 0;
    this->m_i = m_i + 1;
    curx = this->m_x;
    cury = this->m_y;
    m_d = this->m_d;
    if (m_d >= 0)
    {
        auto v8 = m_d + this->m_dinc1;
        this->m_x += this->m_xinc1;
        this->m_d = v8;
        m_yinc1 = this->m_yinc1;
    }
    else
    {
        auto v6 = m_d + this->m_dinc0;
        this->m_x += this->m_xinc0;
        this->m_d = v6;
        m_yinc1 = this->m_yinc0;
    }
    this->m_y += m_yinc1;
    return 1;
}
