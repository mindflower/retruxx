#pragma once
#include <core/stringm3d.h>

namespace help
{
    enum QuestType
    {
        QUESTTYPE_STATIC = 0x0,
        QUESTTYPE_DYNAMIC = 0x1,
        QUESTTYPE_NUM_QUEST_TYPES = 0x2,
        QUESTTYPE_INVALID = 0x2,
    };

    enum UnifyQuestStatus
    {
        QUESTSTATUS_NONCOMPLETE = 0x0,
        QUESTSTATUS_COMPLETE = 0x1,
        QUESTSTATUS_FAILED = 0x2,
        QUESTSTATUS_NUM_QUESTSTATUSES = 0x3,
        QUESTSTATUS_INVALID = 0x3,
    };

    void DeleteAllFilesInDirectory(char const*);
    CStr GetCurrentLevelName();
}
