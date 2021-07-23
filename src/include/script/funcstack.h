#pragma once
#include "funcarg.h"

namespace m3d
{
    class sArgStack
    {
    public:
        ~sArgStack();
        sArg* popIn();
        unsigned int getNumOutArgs() const;
        sArg* popOut();
        sArgStack();
        unsigned int getNumInArgs() const;
        sArg* newOut();
        sArg* newIn();

    private:
        sArg m_InArgs[16];
        sArg m_OutArgs[16];
        unsigned int m_numInArgs;
        unsigned int m_numOutArgs;
        unsigned int m_curInArg;
        unsigned int m_curOutArg;
    };

}
