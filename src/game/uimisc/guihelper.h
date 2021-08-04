#pragma once

namespace help
{
    enum QuestType
    {
        QUESTTYPE_STATIC = 0x0,
        QUESTTYPE_DYNAMIC = 0x1,
        QUESTTYPE_NUM_QUEST_TYPES = 0x2,
        QUESTTYPE_INVALID = 0x2,
    };

    void DeleteAllFilesInDirectory(char const*);
}