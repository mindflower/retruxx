#include "aipassagecommand.h"
#include "aiparamref.h"

#include <cstdio>

namespace ai
{
    // RVA 0x906290
    void AIPassageCommand::Dump()
    {
        printf("%u( ", m_StateNum);
        for (int i = 0; i < (int)m_ParamRefList.size(); ++i)
        {
            printf("%d", m_ParamRefList[i].m_Num);
            if (i != (int)m_ParamRefList.size() - 1)
            {
                printf(",");
            }
        }
        printf(" ) ");
    }
}
