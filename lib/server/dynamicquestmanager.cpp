#include "dynamicquestmanager.h"

#include "retruxx/common.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/dynamicquest.h"
#include "server/objects/dynamicquesthunt.h"

#include <core/log.h>
#include <m3dapp.h>

namespace ai
{
    namespace
    {
        // The prototype of each quest type is "dynamicQuest" plus its suffix.
        CStr const SUFFIXES[DynamicQuestManager::TYPE_NUM_TYPES] = {
            CStr("Destroy"),
            CStr("Reach"),
            CStr("Convoy"),
            CStr("Peace"),
            CStr("Hunt"),
        };
    }  // namespace

    DynamicQuest* DynamicQuestManager::CreateQuest(QuestType questType, int targetObjId, int hirerObjId)
    {
        // RVA 0x7E66F0 - creates the quest object of the given type and tells it who its target and hirer are. A peace
        // quest is made out with the target's whole clan rather than the one object.
        Obj* const target = theObjects->GetEntityByObjId(targetObjId);
        Obj* const hirer = theObjects->GetEntityByObjId(hirerObjId);
        if (!target || !hirer)
        {
            M3D_LOG_ERR("Error: couldn't create dynamic quest");
            return nullptr;
        }

        int const prototypeId = theObjects->GetPrototypeId((CStr("dynamicQuest") + SUFFIXES[questType]).c_str());
        int const questObjId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
        auto* const quest = static_cast<DynamicQuest*>(theObjects->GetEntityByObjId(questObjId));
        if (!quest)
        {
            M3D_LOG_ERR("Error: couldn't create dynamic quest");
            return nullptr;
        }

        quest->m_targetObjId = targetObjId;
        quest->m_hirerObjId = hirerObjId;
        quest->m_targetName = target->GetName();
        quest->m_hirerName = hirer->GetName();
        if (questType == TYPE_PEACE)
        {
            quest->m_targetObjId = target->GetBelong();
            quest->m_targetName = CStr("Belong_") + CStr(target->GetBelong());
        }
        quest->m_reward = quest->_CalcReward();
        quest->_OnCreate();

        M3D_LOG_INFO(quest->GetDebugDescription() + CStr(" created"));
        M3D_LOG_INFO(
            CStr("targetName = '") + quest->m_targetName + CStr("', hirerName = '") + quest->m_hirerName +
            CStr("', reward = ") + CStr(quest->m_reward));
        M3D_APP->EnqueueMessage(66546, questObjId, hirerObjId, 0, 0, CStr(), m3d::AIParam());
        if (questType == TYPE_PEACE)
        {
            M3D_APP->EnqueueMessage(66564, 3, 0, 0, 0, CStr(), m3d::AIParam());
        }
        return quest;
    }

    DynamicQuest* DynamicQuestManager::CreateQuest(QuestType questType, CStr const& targetName, CStr const& hirerName)
    {
        // RVA 0x7E6E00
        return CreateQuest(questType, theObjects->GetObjIdByObjName(targetName), theObjects->GetObjIdByObjName(hirerName));
    }

    void DynamicQuestManager::ConsiderPlayerKill(int belong)
    {
        // RVA 0x7E6690 - tells every running hunt quest that the player killed someone of this belong.
        auto& updating = theObjects->m_updatingObjects;
        for (int id = updating.m_firstNodeId; id != -1; id = updating.m_records[id].m_nextId)
        {
            Obj* const obj = updating.m_records[id].m_value;
            if (obj->IsKindOf(RT_CLASS_LOCAL(DynamicQuestHunt)))
            {
                static_cast<DynamicQuestHunt*>(obj)->ConsiderPlayerKill(belong);
            }
        }
    }
}  // namespace ai
