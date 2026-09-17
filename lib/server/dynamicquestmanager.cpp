#include "dynamicquestmanager.h"

#include "retruxx/common.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/dynamicquesthunt.h"

namespace ai
{
    DynamicQuest* DynamicQuestManager::CreateQuest(QuestType, int, int)
    {
        // TODO(RVA 0x7E66F0): generates a dynamic quest of the given type between
        // two objects. Left unported along with the rest of DynamicQuestManager and
        // the DynamicQuest class it builds; defined here only so that its one live
        // caller - DecToleranceWhenDamageFromPlayerInflicted, which offers a "make
        // peace" quest when the player turns a clan hostile - links.
        RETRUXX_NOT_IMPLEMENTED;
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
