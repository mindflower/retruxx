#include "dynamicquestmanager.h"

#include "retruxx/common.h"

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
}  // namespace ai
