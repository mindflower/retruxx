#pragma once
#include "objects/base/obj.h"
#include <core/aiparam.h>

namespace ai
{
    class Event
    {
    public:
        void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        CStr Dump() const;
        Event(Event const&);
        Event() = default;

    public:
        eGameEvent m_eventId;
        int m_recipientObjId;
        int m_senderObjId;
        float m_timeOut;
        int m_framesToPass;
        float m_timeStamp;
        int m_debugNum;
        m3d::AIParam m_param1;
        m3d::AIParam m_param2;
    };
}
