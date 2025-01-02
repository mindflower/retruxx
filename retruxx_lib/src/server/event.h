#pragma once
#include "objects/base/obj.h"
#include <core/aiparam.h>

namespace ai
{
    struct Event
    {
        /* 0x0000 */ ai::eGameEvent m_eventId;
        /* 0x0004 */ int m_recipientObjId;
        /* 0x0008 */ int m_senderObjId;
        /* 0x000c */ float m_timeOut;
        /* 0x0010 */ int m_framesToPass;
        /* 0x0014 */ float m_timeStamp;
        /* 0x0018 */ int m_debugNum;
        /* 0x001c */ m3d::AIParam m_param1;
        /* 0x0038 */ m3d::AIParam m_param2;
        Event(const ai::Event& __that);
        Event();
        CStr Dump() const;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
    }; /* size: 0x0054 */

    static_assert(sizeof(ai::Event) == 0x0054);
}
