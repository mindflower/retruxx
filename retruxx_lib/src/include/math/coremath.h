#pragma once

//TODO: add static functions

struct CBrezLine
{
    int m_x0;
    int m_x1;
    int m_y0;
    int m_y1;
    int m_numsteps;
    int m_d;
    int m_dinc0;
    int m_dinc1;
    int m_xinc0;
    int m_xinc1;
    int m_yinc0;
    int m_yinc1;
    int m_x;
    int m_y;
    int m_i;

public:
    int step(int&, int&);
    int start(int, int, int, int);
};