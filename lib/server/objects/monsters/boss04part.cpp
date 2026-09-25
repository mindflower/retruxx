#include "boss04part.h"

#include <stdexcept>

#include "core/kernel.h"
#include "server/colliders/vehiclecolliders.h"
#include "server/objects/base/complexphysicobj.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04Part)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04Part);

    Obj* Boss04PartPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x806900
        return new Boss04Part(*this);
    }

    bool Boss04PartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8068A0
        return VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    m3d::Class* Boss04Part::GetBaseClass()
    {
        // RVA 0x806880
        return RT_CLASS_LOCAL(VehiclePart);
    }

    Boss04Part::Boss04Part(Boss04PartPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        // RVA 0x8068C0
        m_bIsDamageable = false;
    }

    void Boss04Part::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x806970
        VehiclePart::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_bIsDamageable, xmlNode, "IsDamageable");
    }

    Boss04PartPrototypeInfo const* Boss04Part::GetPrototypeInfo() const
    {
        // RVA 0x806E10
        return static_cast<Boss04PartPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    int Boss04Part::CollideBoss04PartAndVehiclePart(Boss04Part* boss04Part, VehiclePart* vp, dContact* contact, unsigned& numContacts, bool reverse)
    {
        // RVA 0x8069A0 - the boss's parts do not collide with anything on the boss's side.
        if (vp->GetBelong() == boss04Part->GetBelong())
        {
            return 0;
        }
        return CollideVehiclePartAndVehiclePart(boss04Part, vp, contact, numContacts, reverse);
    }

    void Boss04Part::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x806D40
        VehiclePart::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("IsDamageable", CStr(static_cast<int>(m_bIsDamageable)).c_str());
    }

    m3d::Class* Boss04Part::GetClass() const
    {
        // RVA 0x806890
        return RT_CLASS_LOCAL(Boss04Part);
    }

    bool Boss04Part::bIsDamageable() const
    {
        // Declared in the PDB but never emitted in the shipped build.
        return m_bIsDamageable;
    }

    void Boss04Part::SetDamageable(bool damageable)
    {
        // Declared in the PDB but never emitted in the shipped build.
        m_bIsDamageable = damageable;
    }

    bool Boss04Part::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
    {
        // RVA 0x8068F0 - vetoes all damage until the part is made damageable.
        return !m_bIsDamageable;
    }

    Boss04Part::~Boss04Part()
    {
        // RVA 0x8068E0
    }

    void Boss04Part::_OnDurabilityValueAfterChange(float oldDurabilityValue)
    {
        // RVA 0x806DA0
        // NOTE: unlike Boss03Part this compares with the maximum durability, not the minimum, so a
        // part flies off when it is back at full durability rather than when it is destroyed.
        VehiclePart::_OnDurabilityValueAfterChange(oldDurabilityValue);
        if (!IsAlive() || Durability().value().get() != Durability().maxValue().get())
        {
            return;
        }
        // NOTE: the owner is assumed to be a ComplexPhysicObj without a type check.
        auto* const owner = static_cast<ComplexPhysicObj*>(GetOwner());
        if (owner)
        {
            owner->Flow(this, 10.0f);
            owner->FlowUnattachableParts(10.0f);
        }
    }

    m3d::Object* Boss04Part::CreateObject()
    {
        // RVA 0x806B80
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Boss04Part::Clone()
    {
        // RVA 0x8069C0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
