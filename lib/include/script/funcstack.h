#pragma once
#include "funcarg.h"

namespace m3d
{
    struct sArgStack
    {
        sArgStack(const m3d::sArgStack&);
        sArgStack();
        ~sArgStack();
        unsigned int getNumInArgs() const;
        unsigned int getNumOutArgs() const;
        m3d::sArg* getIn(unsigned int);
        m3d::sArg* getOut(unsigned int);
        m3d::sArg* newIn();
        m3d::sArg* popIn();
        m3d::sArg* newOut();
        m3d::sArg* popOut();
        void clear();
        void Rewind();

        //enum __unnamed;

        /* 0x0000 */ m3d::sArg m_InArgs[16];
        /* 0x0140 */ m3d::sArg m_OutArgs[16];
        /* 0x0280 */ unsigned int m_numInArgs = 0;
        /* 0x0284 */ unsigned int m_numOutArgs = 0;
        /* 0x0288 */ unsigned int m_curInArg = 0;
        /* 0x028c */ unsigned int m_curOutArg = 0;
    }; /* size: 0x0290 */
}
