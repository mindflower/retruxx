#include "event.h"
#include "core/ini.h"
#include "core/ref_ptr.h"
#include "processmanager.h"
#include "server/objects/base/objcontainer.h"

namespace ai
{
    Event::Event(const ai::Event& __that) :
        m_eventId(__that.m_eventId),
        m_recipientObjId(__that.m_recipientObjId),
        m_senderObjId(__that.m_senderObjId),
        m_timeOut(__that.m_timeOut),
        m_framesToPass(__that.m_framesToPass),
        m_timeStamp(__that.m_timeStamp),
        m_debugNum(__that.m_debugNum),
        m_param1(__that.m_param1),
        m_param2(__that.m_param2)
    {

    }

    Event::Event() :
        m_timeOut(0.0),
        m_framesToPass(1),
        m_timeStamp(0.0),
        m_eventId(GE_NUM_EVENTS),
        m_recipientObjId(-1),
        m_senderObjId(-1),
        m_debugNum(0)
    {
        
    }

    CStr Event::Dump() const
    {
        // RVA 0x8BA4F0 - "event <n> <<name>> From:<id> ( <class> <name> )  To:<id> ( ... )  time:<t> timeout = <t>"
        // followed by the parameters that are set.
        auto const describe = [](int objId) {
            if (objId >= 0)
            {
                if (auto* obj = theObjects->GetEntityByObjId(objId))
                {
                    return CStr(obj->GetClassNameA()) + CStr(" ") + CStr(obj->GetName());
                }
            }
            return CStr("object deleted");
        };

        CStr str = CStr("event ") + CStr(m_debugNum) + CStr(" <");
        str += theProcessManager->GetEventName(m_eventId);
        str += CStr("> From:") + CStr(m_senderObjId) + CStr(" ( ") + describe(m_senderObjId) + CStr(" ) ") +
            CStr(" To:") + CStr(m_recipientObjId) + CStr(" ( ") + describe(m_recipientObjId) + CStr(" ) ") +
            CStr(" time:") + CStr(m_timeStamp) + CStr(" timeout = ") + CStr(m_timeOut);
        if (m_param1.GetType())
        {
            str += CStr(" Param1=") + m_param1.ToStr();
        }
        if (m_param2.GetType())
        {
            str += CStr(" Param2=") + m_param2.ToStr();
        }
        return str;
    }

    void Event::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode)
    {
        // RVA 0x8BA100
        CStr eventAttr = OwnNode->GetAttribute("EventID");
        m_eventId = theProcessManager->GetEventId(eventAttr);

        m3d::SafeIntAttrib(m_recipientObjId, OwnNode, "RecipientObjID");
        m3d::SafeIntAttrib(m_senderObjId, OwnNode, "SenderObjID");
        m3d::SafeFloatAttrib(m_timeOut, OwnNode, "TimeOut");
        m3d::SafeFloatAttrib(m_timeStamp, OwnNode, "TimeStamp");
        m3d::SafeIntAttrib(m_debugNum, OwnNode, "DebugNum");

        ref_ptr node = xmlFile->CreateNode();
        OwnNode->GetFirstChild(node, "Param1");
        if (node->IsEmpty())
        {
            m_param1.SetType(m3d::AIPARAM_UNDEFINE);
        }
        else
        {
            m_param1.LoadFromXML(xmlFile, node);
        }

        OwnNode->GetFirstChild(node, "Param2");
        if (node->IsEmpty())
        {
            m_param2.SetType(m3d::AIPARAM_UNDEFINE);
        }
        else
        {
            m_param2.LoadFromXML(xmlFile, node);
        }

    }

    void Event::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        // RVA 0x8BA2D0
        OwnNode->SetAttribute("EventID", theProcessManager->GetEventName(m_eventId).c_str());
        OwnNode->SetAttribute("RecipientObjID", CStr(m_recipientObjId).c_str());
        OwnNode->SetAttribute("SenderObjID", CStr(m_senderObjId).c_str());
        OwnNode->SetAttribute("TimeOut", CStr(m_timeOut).c_str());
        OwnNode->SetAttribute("TimeStamp", CStr(m_timeStamp).c_str());
        OwnNode->SetAttribute("DebugNum", CStr(m_debugNum).c_str());
        if (m_param1.GetType())
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Param1");
            OwnNode->AddChild(node);
            m_param1.SaveToXML(xmlFile, node);
        }
        if (m_param2.GetType())
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Param2");
            OwnNode->AddChild(node);
            m_param2.SaveToXML(xmlFile, node);
        }

    }
}
