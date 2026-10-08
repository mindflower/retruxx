#include "temporarylocation.h"

#include <stdexcept>

#include "core/kernel.h"
#include "base/prototypemanager.h"
#include "physicbodies/physicbody.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TemporaryLocation)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TemporaryLocation);

    TemporaryLocationPrototypeInfo::TemporaryLocationPrototypeInfo()
    {
        m_TTL = 0.0;
        m_TimeForActivation = 0.0;
    }

    Obj* TemporaryLocationPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x906500 - TemporaryLocation is abstract; subclasses build their own objects.
        return nullptr;
    }

    bool TemporaryLocationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = LocationPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_TTL, xmlNode, "TTL");
            m3d::SafeFloatAttrib(m_TimeForActivation, xmlNode, "ActivateTime");
            m3d::SafeStrAttrib(m_EffectName, xmlNode, "Effect");
        }
        return result;
    }

    void TemporaryLocation::CreateEffectNode()
    {
        // RVA 0x906760
        CStr const& effectName = GetPrototypeInfo()->m_EffectName;
        if (effectName.c_str() && strlen(effectName.c_str()))
        {
            PhysicBody::CreateEffectNode(effectName, GetPosition(), GetRotation(), true, 1.0f);
        }
    }

    void TemporaryLocation::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x906B80
        Location::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TL", CStr(m_TL).c_str());
        xmlNode->SetAttribute("TLState", CStr(static_cast<int>(m_TemporaryLocationState)).c_str());
    }

    void TemporaryLocation::Remove()
    {
        // RVA 0x906600
        Location::Remove();
    }

    TemporaryLocation::TemporaryLocation(TemporaryLocationPrototypeInfo const& prototype) :
        Location(prototype),
        m_TL(0.0f),
        m_TemporaryLocationState(tlsActivation)
    {
        // RVA 0x906510 - inactive until the activation time has passed.
        SetActive(false);
    }

    m3d::Class* TemporaryLocation::GetClass() const
    {
        // RVA 0x9064F0
        return RT_CLASS_LOCAL(TemporaryLocation);
    }

    TemporaryLocationPrototypeInfo const* TemporaryLocation::GetPrototypeInfo() const
    {
        // RVA 0x906C10 - NOTE: the prototype is cast without a type check.
        return static_cast<TemporaryLocationPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void TemporaryLocation::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x9066E0 - a missing TLState means still activating.
        Location::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeFloatAttrib(m_TL, xmlNode, "TL");
        int state = tlsActivation;
        m3d::SafeIntAttrib(state, xmlNode, "TLState");
        m_TemporaryLocationState = static_cast<TemporaryLocationState>(state);
    }

    m3d::Class* TemporaryLocation::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Location);
    }

    void TemporaryLocation::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x906580 - activates after TimeForActivation seconds, then lives for TTL more.
        Location::Update(elapsedTime, workTime);
        m_TL = elapsedTime + m_TL;
        TemporaryLocationPrototypeInfo const* prototype = GetPrototypeInfo();
        if (m_TemporaryLocationState == tlsActivation)
        {
            if (m_TL > prototype->m_TimeForActivation)
            {
                OnActivate();
            }
        }
        else if (m_TemporaryLocationState == tlsWaiting && m_TL > prototype->m_TTL)
        {
            Remove();
        }
    }

    // RVA 0x906550
    TemporaryLocation::~TemporaryLocation() = default;

    m3d::Object* TemporaryLocation::Clone()
    {
        // RVA 0x9067C0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* TemporaryLocation::CreateObject()
    {
        // RVA 0x906980
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void TemporaryLocation::OnActivate()
    {
        // RVA 0x906560 - the TTL is counted from activation.
        m_TL = 0.0f;
        m_TemporaryLocationState = tlsWaiting;
        SetActive(true);
    }
}  // namespace ai
