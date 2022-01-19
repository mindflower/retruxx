#pragma once
#include <core/stringm3d.h>

namespace ai
{
    class DynamicQuest;

    class DynamicQuestManager
    {
    public:
        enum QuestType
        {
            TYPE_DESTROY = 0x0,
            TYPE_REACH = 0x1,
            TYPE_CONVOY = 0x2,
            TYPE_PEACE = 0x3,
            TYPE_HUNT = 0x4,
            TYPE_NUM_TYPES = 0x5,
        };

    public:
        static DynamicQuest* __fastcall CreateQuest(QuestType, CStr const&, CStr const&);
        static DynamicQuest* __fastcall CreateQuest(QuestType, int, int);
        static void __fastcall ConsiderPlayerKill(int);
    };
}
