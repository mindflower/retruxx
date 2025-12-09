#include "aimessage.h"

#include <stdexcept>

#include "core/aiparam.h"

namespace ai
{
    AIMessage::AIMessage(int Num, const m3d::AIParam& Param1, const m3d::AIParam& Param2, const m3d::AIParam& Param3)
    {
        this->m_Num = Num;
        this->m_RemoveAfterFinishing = 0;
        if (Param1.GetType())
            m_ParamList.push_back(Param1);
        if (Param2.GetType())
            m_ParamList.push_back(Param2);
        if (Param3.GetType())
            m_ParamList.push_back(Param3);
    }
}
