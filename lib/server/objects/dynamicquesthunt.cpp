#include "dynamicquesthunt.h"
#include <stdexcept>

#include "core/ini.h"

#include "base/prototypemanager.h"
#include "server/dynamicscene.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/player.h"
#include "server/relationship.h"

#include <core/kernel.h>
#include <m3dapp.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestHunt)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestHunt);

    ai::DynamicQuest* DynamicQuestHuntPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x741630
        return new DynamicQuestHunt(*this);
    }

    DynamicQuestHuntPrototypeInfo::DynamicQuestHuntPrototypeInfo()
    {
        m_playerSchwarzPart = 0.0;
    }

    bool DynamicQuestHuntPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerSchwarzPart, xmlNode, "PlayerSchwarzPart");
            m3d::SafeFloatAttrib(m_huntSeasonLength, xmlNode, "HuntSeasonLength");
            return 1;
        }
        return result;
    }

    DynamicQuestHunt::DynamicQuestHunt(DynamicQuestHuntPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        // RVA 0x741400 - a negative m_timePassed means the season has not started yet. The base class is
        // told not to announce the money itself, because ProcessHuntCompletion announces the whole bag.
        m_fragsAtStart = 0;
        m_bShowMessageForAddMoney = false;
        m_timePassed = -1.0f;
        m_fadingMsgIdOnComplete = FM_HUNT_COMPLETED;
    }

    void DynamicQuestHunt::ConsiderPlayerKill(int victimBelong)
    {
        // RVA 0x741C00 - a kill counts when the victim is hostile to the player and not of the hirer's own belong.
        if (m_questStatus != STATUS_PROCESSING)
        {
            return;
        }
        Obj* const hirer = theObjects->GetEntityByObjId(m_hirerObjId);
        if (!hirer || !thePlayer)
        {
            return;
        }
        int const hirerBelong = hirer->GetBelong();
        if (theRelationship->CheckTolerance(victimBelong, thePlayer->GetBelong()) <= RS_ENEMY && victimBelong != hirerBelong)
        {
            ++m_fragsAtStart;
        }
    }

    DynamicQuestManager::QuestType DynamicQuestHunt::GetQuestType() const
    {
        // RVA 0x741450
        return DynamicQuestManager::TYPE_HUNT;
    }

    void DynamicQuestHunt::UpdateBeforeShowInConversation()
    {
        // RVA 0x7414E0 - the price per head is worked out afresh every time it is quoted.
        m_reward = _CalcReward();
    }

    void DynamicQuestHunt::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7417C0
        DynamicQuest::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TimePassed", CStr(m_timePassed).c_str());
        xmlNode->SetAttribute("FragsAtStart", CStr(m_fragsAtStart).c_str());
    }

    float DynamicQuestHunt::GetTimeBeforeHuntEnd() const
    {
        // RVA 0x741520
        if (m_timePassed < -0.5)
        {
            return 0.0f;
        }
        return GetPrototypeInfo()->m_huntSeasonLength - m_timePassed;
    }

    m3d::Class* DynamicQuestHunt::GetClass() const
    {
        // RVA 0x7413E0
        return RT_CLASS_LOCAL(DynamicQuestHunt);
    }

    void DynamicQuestHunt::Update(float elapsedTime, unsigned)
    {
        // RVA 0x741D80 - NOTE: unlike the other dynamic quests this never calls Obj::Update, so a hunt
        // quest object runs none of the base per-frame work.
        if (GetQuestStatus() != STATUS_PROCESSING)
        {
            return;
        }
        m_timePassed = elapsedTime + m_timePassed;
        if (m_timePassed >= GetPrototypeInfo()->m_huntSeasonLength)
        {
            ProcessHuntCompletion();
        }
    }

    void DynamicQuestHunt::ProcessHuntCompletion()
    {
        // RVA 0x741C80 - the season is over: the player is paid once per kill. _SetStatus already hands
        // over one reward, so only the remaining kills are added here, and the message names the total.
        if (GetQuestStatus() == STATUS_NOT_TAKEN || GetQuestStatus() == STATUS_FORGOTTEN)
        {
            _SetStatus(STATUS_FORGOTTEN);
            return;
        }

        _SetStatus(STATUS_COMPLETE);
        if (!thePlayer)
        {
            return;
        }
        thePlayer->AddMoney(m_reward * (m_fragsAtStart - 1));

        int const total = m_reward * m_fragsAtStart;
        if (total > 0)
        {
            M3D_APP->ImmediateMessage(66563, FM_PLAYER_ADD_MONEY, total, 0, 0, {}, {});
        }
        else if (total < 0)
        {
            M3D_APP->ImmediateMessage(66563, FM_PLAYER_GIVE_MONEY, -total, 0, 0, {}, {});
        }
    }

    void DynamicQuestHunt::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x741750
        DynamicQuest::LoadRuntimeValues(xmlFile, xmlNode);
        if (!xmlNode->IsEmpty())
        {
            char const* const timePassed = xmlNode->GetAttribute("TimePassed");
            if (timePassed)
            {
                m_timePassed = static_cast<float>(atof(timePassed));
            }
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const fragsAtStart = xmlNode->GetAttribute("FragsAtStart");
            if (fragsAtStart)
            {
                m_fragsAtStart = atoi(fragsAtStart);
            }
        }
    }

    DynamicQuestHuntPrototypeInfo const* DynamicQuestHunt::GetPrototypeInfo() const
    {
        // RVA 0x741BD0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), DynamicQuestHuntPrototypeInfo const);
    }

    float DynamicQuestHunt::GetTimeFromHuntStart() const
    {
        // RVA 0x741500
        return m_timePassed >= -0.5 ? m_timePassed : 0.0f;
    }

    m3d::Class* DynamicQuestHunt::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestHunt::PassToAnotherMap()
    {
        // RVA 0x7414A0 - leaving the map ends the season early; the kills made so far are still paid for.
        if (GetQuestStatus() == STATUS_NOT_TAKEN || GetQuestStatus() == STATUS_FORGOTTEN)
        {
            _SetStatus(STATUS_FORGOTTEN);
            return;
        }
        m_reward *= m_fragsAtStart;
        _SetStatus(STATUS_COMPLETE);
    }

    void DynamicQuestHunt::_OnTake()
    {
        // RVA 0x741480 - the season starts now.
        DynamicQuest::_OnTake();
        m_timePassed = 0.0f;
    }

    int DynamicQuestHunt::_CalcReward()
    {
        // RVA 0x7416C0 - the price per head is a share of the player's own rating.
        DynamicQuestHuntPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (!gDynamicScene->GetVehicleControlledByPlayer())
        {
            return prototypeInfo->m_minReward;
        }
        if (!thePlayer)
        {
            return 0;
        }
        int const reward = static_cast<int>(thePlayer->GetSchwarz() * prototypeInfo->m_playerSchwarzPart);
        return reward > prototypeInfo->m_minReward ? reward : prototypeInfo->m_minReward;
    }

    DynamicQuestHunt::~DynamicQuestHunt()
    {
        // RVA 0x741460 - the base class does all the work.
    }

    void DynamicQuestHunt::_OnCreate()
    {
        // RVA 0x741470 - a hunt quest subscribes to nothing; kills reach it through
        // DynamicQuestManager::ConsiderPlayerKill.
    }

    m3d::Object* DynamicQuestHunt::Clone()
    {
        // RVA 0x741850
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* DynamicQuestHunt::CreateObject()
    {
        // RVA 0x741A10
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}
