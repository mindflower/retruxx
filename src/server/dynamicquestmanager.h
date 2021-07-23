#pragma once

class CStr;

namespace ai
{
    class DynamicQuest;

    class DynamicQuestManager
    {
    public:
        static DynamicQuest* __fastcall CreateQuest(QuestType, CStr const&, CStr const&);
        static DynamicQuest* __fastcall CreateQuest(QuestType, int, int);
        static void __fastcall ConsiderPlayerKill(int);
    };
}
