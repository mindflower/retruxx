#include "boss03part.h"

#include <stdexcept>

#include "core/kernel.h"
#include "server/colliders/vehiclecolliders.h"
#include "server/objects/base/complexphysicobj.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss03Part)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss03Part);

    Obj* Boss03PartPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x806EC0
        return new Boss03Part(*this);
    }

    bool Boss03PartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x806E60
        return VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    int Boss03Part::CollideBoss03PartAndVehiclePart(Boss03Part* boss03Part, VehiclePart* vp, dContact* contact, unsigned& numContacts, bool reverse)
    {
        // RVA 0x806F60 - the boss's parts do not collide with anything on the boss's side.
        if (vp->GetBelong() == boss03Part->GetBelong())
        {
            return 0;
        }
        return CollideVehiclePartAndVehiclePart(boss03Part, vp, contact, numContacts, reverse);
    }

    Boss03PartPrototypeInfo const* Boss03Part::GetPrototypeInfo() const
    {
        // RVA 0x8073D0
        return static_cast<Boss03PartPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    Boss03Part::Boss03Part(Boss03PartPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        // RVA 0x806E80
        m_bIsDamageable = false;
    }

    void Boss03Part::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x806F30
        VehiclePart::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_bIsDamageable, xmlNode, "IsDamageable");
    }

    bool Boss03Part::bIsDamageable() const
    {
        // Declared in the PDB but never emitted in the shipped build.
        return m_bIsDamageable;
    }

    void Boss03Part::SetDamageable(bool damageable)
    {
        // RVA 0x737A30
        m_bIsDamageable = damageable;
    }

    m3d::Class* Boss03Part::GetBaseClass()
    {
        // RVA 0x806E40
        return RT_CLASS_LOCAL(VehiclePart);
    }

    m3d::Class* Boss03Part::GetClass() const
    {
        // RVA 0x806E50
        return RT_CLASS_LOCAL(Boss03Part);
    }

    void Boss03Part::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x807300
        VehiclePart::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("IsDamageable", CStr(static_cast<int>(m_bIsDamageable)).c_str());
    }

    Boss03Part::~Boss03Part()
    {
        // RVA 0x806EA0
    }

    bool Boss03Part::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
    {
        // RVA 0x806EB0 - vetoes all damage until the part is made damageable.
        return !m_bIsDamageable;
    }

    void Boss03Part::_OnDurabilityValueAfterChange(float oldDurabilityValue)
    {
        // RVA 0x807360 - a destroyed part flies off the boss, taking loose parts with it.
        VehiclePart::_OnDurabilityValueAfterChange(oldDurabilityValue);
        if (!IsAlive() || Durability().value().get() != Durability().minValue().get())
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

    m3d::Object* Boss03Part::Clone()
    {
        // RVA 0x806F80
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Boss03Part::CreateObject()
    {
        // RVA 0x807140
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
