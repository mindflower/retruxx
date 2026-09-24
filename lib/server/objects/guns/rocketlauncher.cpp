#include "rocketlauncher.h"

#include <math/matrix.h>
#include <stdexcept>

#include "core/kernel.h"
#include "rocket.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RocketLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RocketLauncher);

    Obj* RocketLauncherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7C36F0
        return new RocketLauncher(*this);
    }

    bool RocketLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeBoolAttrib(m_withAngleLimit, xmlNode, "WithAngleLimit");
        }
        return result;
    }

    RocketLauncherPrototypeInfo::RocketLauncherPrototypeInfo()
    {
        m_withAngleLimit = true;
        m_damageType = DAMAGE_BLAST;
        m_WithShellsPoolLimit = true;
    }

    void RocketLauncherPrototypeInfo::PostLoad()
    {
        // RVA 0x7C34D0
        GunPrototypeInfo::PostLoad();
    }

    void RocketLauncher::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x7C3D00
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Gun::GetPropertiesNames(Props);
    }

    RocketLauncher::RocketLauncher(RocketLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        // RVA 0x7C34E0
    }

    m3d::Class* RocketLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    RocketLauncherPrototypeInfo const* RocketLauncher::GetPrototypeInfo() const
    {
        // RVA 0x7C3AE0 - NOTE: the prototype is cast without a type check.
        return static_cast<RocketLauncherPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    bool RocketLauncher::isLookAtPoint(CVector const&, float) const
    {
        // RVA 0x7C3520 - rockets steer themselves, so the launcher is always on target.
        return true;
    }

    bool RocketLauncher::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x7C35A0
        return Gun::SetPropertyById(propertyId, newValue);
    }

    bool RocketLauncher::CanLookAtTarget() const
    {
        // RVA 0x7C3530 - a launcher that turns all the way round always can; otherwise the target
        // may lie up to 90 degrees outside the turning range, since the rockets turn themselves.
        // NOTE: the full-circle constant is 6.283184482..., slightly less than 2*pi.
        if (double(m_rightStopAngle) - m_leftStopAngle > 6.283184482025146)
        {
            return true;
        }
        return m_rightStopAngle + 1.5707964f > m_currentDesiredAlpha && m_currentDesiredAlpha > m_leftStopAngle - 1.5707964f;
    }

    int RocketLauncher::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x7C3C90
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return Gun::GetPropertyId(PropertyName);
    }

    m3d::Class* RocketLauncher::GetClass() const
    {
        // RVA 0x7C34C0
        return RT_CLASS_LOCAL(RocketLauncher);
    }

    void RocketLauncher::Registration()
    {
        // RVA 0x7C3510 - RocketLauncher has no properties of its own.
    }

    void RocketLauncher::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x7C3D80
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Gun::GetPropertiesIDs(Props);
    }

    CStr RocketLauncher::GetPropertyName(int id) const
    {
        // RVA 0x7C3E00 - a linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Gun::GetPropertyName(id);
    }

    eGObjPropertySaveStatus RocketLauncher::GetPropertySaveStatus(int id) const
    {
        // RVA 0x7C3C50
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Gun::GetPropertySaveStatus(id);
    }

    bool RocketLauncher::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7C35B0
        return Gun::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    bool RocketLauncher::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7C3590
        return Gun::_GetPropertyInternal(propertyId, retVal);
    }

    void RocketLauncher::_LaunchShells()
    {
        // RVA 0x7C3B10 - the rocket inherits the launcher's target, the owner's velocity and the
        // prototype's angle limit.
        Gun::_LaunchShells();
        RocketLauncherPrototypeInfo const* prototype = GetPrototypeInfo();

        int const rocketId = theObjects->CreateNewObject(GetShellPrototypeId(), "", -1, -1);
        // NOTE: neither the rocket nor the owner is checked for null before use.
        Rocket* rocket = static_cast<Rocket*>(theObjects->GetEntityByObjId(rocketId));
        rocket->SetGunObjId(GetId());
        rocket->m_targetObjId = m_targetObjId;
        PhysicObj* owner = GetOwner();
        if (owner)
        {
            rocket->SetBelong(owner->GetBelong());
        }
        rocket->SetPosition(_CalcPosForNextShot());
        rocket->SetDirection(_CalcDirForNextShot());
        rocket->m_initialDirection = rocket->GetDirection();
        rocket->SetInitialVelocity(owner->GetLinearVelocity());
        rocket->m_withAngleLimit = prototype->m_withAngleLimit;
        rocket->SetDeadTimer(100000, false);
        rocket->RelinkToSpace(owner->GetSpaceId());
    }

    // RVA 0x7C3500
    RocketLauncher::~RocketLauncher() = default;

    m3d::Object* RocketLauncher::CreateObject()
    {
        // RVA 0x7C3920
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* RocketLauncher::Clone()
    {
        // RVA 0x7C3760
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
