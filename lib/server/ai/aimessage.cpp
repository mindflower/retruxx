#include "aimessage.h"

#include <stdexcept>

#include "core/aiparam.h"
#include <core/ini.h>
#include <core/ref_ptr.h>

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

    AIMessage::AIMessage()
    {
        // RVA 0x7F28A0 - 0xFFFF stands for "no command".
        m_Num = 0xFFFF;
        m_RemoveAfterFinishing = 0;
    }

    void AIMessage::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* OwnNode)
    {
        // RVA 0x7F2D40 - the parameters are numbered child elements, read until one is missing.
        m3d::SafeIntAttrib(m_Num, OwnNode, "Num");
        m3d::SafeIntAttrib(m_RemoveAfterFinishing, OwnNode, "Remove");

        ref_ptr node = xmlFile->CreateNode();
        for (int i = 0;; ++i)
        {
            OwnNode->GetFirstChild(node, (CStr("Param_") + CStr(i)).c_str());
            if (node->IsEmpty())
            {
                break;
            }
            m3d::AIParam p;
            p.LoadFromXML(xmlFile, node);
            m_ParamList.push_back(p);
        }
    }

    void AIMessage::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        // RVA 0x7EFE20
        OwnNode->SetAttribute("Num", CStr(m_Num).c_str());
        OwnNode->SetAttribute("Remove", CStr(m_RemoveAfterFinishing).c_str());

        for (unsigned i = 0; i < m_ParamList.size(); ++i)
        {
            ref_ptr node = xmlFile->CreateNode(
                m3d::cmn::XML_NODE_ELEMENT, (CStr("Param_") + CStr(i)).c_str());
            OwnNode->AddChild(node);
            m_ParamList[i].SaveToXML(xmlFile, node);
        }
    }
}
