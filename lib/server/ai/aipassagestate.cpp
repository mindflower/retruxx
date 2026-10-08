#include "aipassagestate.h"

#include "core/aiparam.h"
#include <core/ini.h>
#include <core/ref_ptr.h>

namespace ai
{
    void AIPassageState::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* OwnNode)
    {
        // RVA 0x7F2B60 - the parameters are numbered child elements, read until one is missing.
        m3d::SafeIntAttrib(m_StateNum, OwnNode, "StateNum");

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

    void AIPassageState::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        // RVA 0x7EFC30
        OwnNode->SetAttribute("StateNum", CStr(m_StateNum).c_str());

        for (unsigned i = 0; i < m_ParamList.size(); ++i)
        {
            ref_ptr node = xmlFile->CreateNode(
                m3d::cmn::XML_NODE_ELEMENT, (CStr("Param_") + CStr(i)).c_str());
            OwnNode->AddChild(node);
            m_ParamList[i].SaveToXML(xmlFile, node);
        }
    }
}
