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

        m3d::sArg m_InArgs[16];
        m3d::sArg m_OutArgs[16];
        unsigned int m_numInArgs = 0;
        unsigned int m_numOutArgs = 0;
        unsigned int m_curInArg = 0;
        unsigned int m_curOutArg = 0;
    };
}
