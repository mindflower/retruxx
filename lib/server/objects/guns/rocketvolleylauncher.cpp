#include "rocketvolleylauncher.h"

#include <math/matrix.h>
#include <stdexcept>

#include "core/kernel.h"
#include "server/utils.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RocketVolleyLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RocketVolleyLauncher);

    bool RocketVolleyLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = RocketLauncherPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_actionDist, xmlNode, "ActionDist");
        }
        return result;
    }

    Obj* RocketVolleyLauncherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7DC680
        return new RocketVolleyLauncher(*this);
    }

    RocketVolleyLauncherPrototypeInfo::RocketVolleyLauncherPrototypeInfo()
    {
        m_actionDist = 0.0f;
        m_WithShellsPoolLimit = 1;
    }

    void RocketVolleyLauncher::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7DC310
        Gun::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("IsVolleyFiring", CStr(static_cast<int>(m_bIsVolleyFiring)).c_str());
        for (int const targetId : m_hadToLaunch)
        {
            ref_ptr targetNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "HadToLaunch");
            targetNode->SetAttribute("Id", CStr(targetId).c_str());
            xmlNode->AddChild(targetNode);
        }
    }

    bool RocketVolleyLauncher::Fire(bool enable)
    {
        // RVA 0x7DC810 - starting a volley queues one target per barrel (bounded by the shells in
        // the charge): the nearest enemies first, then random ones if there are fewer enemies than
        // barrels.
        if (!enable || !IsDurabilityEnoughForFiring())
        {
            return false;
        }

        if (!m_bIsVolleyFiring)
        {
            m_bIsVolleyFiring = true;
            RocketVolleyLauncherPrototypeInfo const* prototype = GetPrototypeInfo();
            // NOTE: the owner is assumed to be a vehicle and is not checked for null.
            Vehicle* owner = static_cast<Vehicle*>(GetOwner());
            retruxx::vector<int> targets;
            owner->GetEnemiesInNeighborhood(prototype->m_actionDist, targets);
            sortPhysicObjsByDistance(targets, GetPosition());
            if (!targets.empty())
            {
                unsigned const shellsInCharge = GetShellsInCurrentCharge();
                unsigned const barrels = GetBarrelsNum();
                int const count = static_cast<int>(shellsInCharge >= barrels ? barrels : shellsInCharge);
                unsigned next = 0;
                for (int i = 0; i < count; ++i)
                {
                    int const targetId = next < targets.size() ? targets[next++] : targets[rand() % targets.size()];
                    m_hadToLaunch.push_back(targetId);
                }
            }
        }
        _TryToLaunch();
        return true;
    }

    void RocketVolleyLauncher::SetInvisible()
    {
        // RVA 0x7DC490 - a hidden launcher drops its volley.
        Gun::SetInvisible();
        std::vector<int>().swap(m_hadToLaunch);
        m_bIsVolleyFiring = false;
    }

    void RocketVolleyLauncher::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x7DC660
        Gun::Update(elapsedTime, workTime);
        _TryToLaunch();
    }

    RocketVolleyLauncher::RocketVolleyLauncher(ai::RocketVolleyLauncherPrototypeInfo const& prototype) :
        RocketLauncher(prototype),
        m_bIsVolleyFiring(false)
    {
        // RVA 0x7DC5C0
    }

    m3d::Class* RocketVolleyLauncher::GetClass() const
    {
        // RVA 0x7DBF10
        return RT_CLASS_LOCAL(RocketVolleyLauncher);
    }

    m3d::Class* RocketVolleyLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(RocketLauncher);
    }

    void RocketVolleyLauncher::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7DC6D0 - NOTE: the queue is appended to, not cleared first, and a HadToLaunch
        // entry without an Id repeats the previous id (the shipped code keeps it in a local that
        // starts uninitialized; here it starts at 0).
        Gun::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_bIsVolleyFiring, xmlNode, "IsVolleyFiring");

        int targetId = 0;
        ref_ptr targetNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(targetNode, "HadToLaunch"); !targetNode->IsEmpty();
             targetNode->GetNextSibling(targetNode, "HadToLaunch"))
        {
            m3d::SafeIntAttrib(targetId, targetNode, "Id");
            m_hadToLaunch.push_back(targetId);
        }
    }

    ai::RocketVolleyLauncherPrototypeInfo const* RocketVolleyLauncher::GetPrototypeInfo() const
    {
        // RVA 0x7DC590 - NOTE: the prototype is cast without a type check.
        return static_cast<RocketVolleyLauncherPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    bool RocketVolleyLauncher::_bIsUsingVolley() const
    {
        // RVA 0x7DBF30
        return true;
    }

    bool RocketVolleyLauncher::_bIsVolleyFiring() const
    {
        // RVA 0x7DBF20
        return m_bIsVolleyFiring;
    }

    // RVA 0x7DC440
    RocketVolleyLauncher::~RocketVolleyLauncher() = default;

    void RocketVolleyLauncher::_TryToLaunch()
    {
        // RVA 0x7DC4D0 - fires at every queued target the gun is ready for; the volley ends when the
        // queue is empty.
        if (m_bIsVolleyFiring)
        {
            unsigned i = 0;
            while (i < m_hadToLaunch.size())
            {
                SetTargetId(m_hadToLaunch[i]);
                if (_DoFire())
                {
                    m_hadToLaunch.erase(m_hadToLaunch.begin() + i);
                }
                else
                {
                    ++i;
                }
            }
            if (m_hadToLaunch.empty())
            {
                m_bIsVolleyFiring = false;
            }
        }
    }

    m3d::Object* RocketVolleyLauncher::CreateObject()
    {
        // RVA 0x7DC150
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* RocketVolleyLauncher::Clone()
    {
        // RVA 0x7DBF90
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
