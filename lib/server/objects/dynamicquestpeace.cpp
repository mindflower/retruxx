#include "dynamicquestpeace.h"
#include <stdexcept>

#include "core/ini.h"

#include "base/prototypemanager.h"
#include "server/dynamicscene.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"
#include "server/processmanager.h"
#include "server/relationship.h"

#include <core/kernel.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestPeace)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestPeace);

    DynamicQuestPeacePrototypeInfo::DynamicQuestPeacePrototypeInfo()
    {
        m_playerMoneyPart = 0.0;
    }

    ai::DynamicQuest* DynamicQuestPeacePrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x746C90
        return new DynamicQuestPeace(*this);
    }

    bool DynamicQuestPeacePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerMoneyPart, xmlNode, "PlayerMoneyPart");
            return 1;
        }
        return result;
    }

    DynamicQuestPeace::DynamicQuestPeace(DynamicQuestPeacePrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        // RVA 0x746B60 - nothing of its own to set up.
    }

    m3d::Class* DynamicQuestPeace::GetClass() const
    {
        // RVA 0x746B40
        return RT_CLASS_LOCAL(DynamicQuestPeace);
    }

    DynamicQuestPeacePrototypeInfo const* DynamicQuestPeace::GetPrototypeInfo() const
    {
        // RVA 0x747130
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), DynamicQuestPeacePrototypeInfo const);
    }

    int DynamicQuestPeace::OnEvent(Event const& evn)
    {
        // RVA 0x746B50 - nothing of its own to handle.
        return DynamicQuest::OnEvent(evn);
    }

    DynamicQuestManager::QuestType DynamicQuestPeace::GetQuestType() const
    {
        // RVA 0x746B80
        return DynamicQuestManager::TYPE_PEACE;
    }

    m3d::Class* DynamicQuestPeace::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestPeace::PassToAnotherMap()
    {
        // RVA 0x746BD0 - a peace quest survives the change of map untouched; it is settled the moment
        // it is taken.
    }

    void DynamicQuestPeace::_OnCreate()
    {
        // RVA 0x746BA0 - a placeholder price to show before the real one is worked out on taking.
        m_reward = static_cast<int>(GetPrototypeInfo()->m_playerMoneyPart * 100.0f);
    }

    int DynamicQuestPeace::_CalcReward()
    {
        // RVA 0x746D50 - making peace costs the player a share of their money, so the reward is negative.
        DynamicQuestPeacePrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (!gDynamicScene->GetVehicleControlledByPlayer())
        {
            // NOTE: this fallback is positive, unlike every other path through this function.
            return prototypeInfo->m_minReward;
        }
        if (!thePlayer)
        {
            return 0;
        }
        int const price = static_cast<int>(thePlayer->GetMoney() * prototypeInfo->m_playerMoneyPart);
        return -(price > prototypeInfo->m_minReward ? price : prototypeInfo->m_minReward);
    }

    void DynamicQuestPeace::_OnTake()
    {
        // RVA 0x746D00 - taking the quest is the whole quest: the price is paid and the clan stops being
        // hostile at once. The target id holds the clan, not an object.
        DynamicQuest::_OnTake();
        m_reward = _CalcReward();
        _SetStatus(STATUS_COMPLETE);
        if (thePlayer)
        {
            theRelationship->SetTolerance(thePlayer->GetBelong(), GetTargetObjId(), 2.0f);
        }
    }

    void DynamicQuestPeace::_OnHirerBecamesEnemyWithPlayer()
    {
        // RVA 0x746BE0 - a peace quest is not dropped when its hirer turns hostile; making peace with
        // an enemy is the point of it.
    }

    DynamicQuestPeace::~DynamicQuestPeace()
    {
        // RVA 0x746B90 - the base class does all the work.
    }

    m3d::Object* DynamicQuestPeace::CreateObject()
    {
        // RVA 0x746F70
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* DynamicQuestPeace::Clone()
    {
        // RVA 0x746DB0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}
