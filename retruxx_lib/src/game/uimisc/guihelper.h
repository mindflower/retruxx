#pragma once
#include "skelmodel.h"
#include "core/stringm3d.h"
#include "retruxx/common.h"

namespace m3d
{
    class AnimatedModel;
}
namespace ai
{
    class Obj;
}
namespace m3d
{
    namespace ui
    {
        class Wnd;
    }
}  // namespace m3d

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

    enum BlackWightTolerance
    {
        BW_TOLERANCE_ENEMY = 0x0,
        BW_TOLERANCE_FRIEND = 0x1,
        BW_TOLERANCE_NUM_BW_TOLERANCES = 0x2,
        BW_TOLERANCE_INVALID = 0x2,
    };

    enum Color
    {
        COLOR_RED = 0x0,
        COLOR_GREEN = 0x1,
        COLOR_YELLOW = 0x2,
        COLOR_WHITE = 0x3,
    };

    enum _ActionType
    {
        _AT_STAND = 0x0,
        _AT_MOVE = 0x1,
        _AT_NUMTYPES = 0x2,
    };

    enum ColorTrhreshold
    {
        THRESHOLD_DEAD = 0x0,
        THRESHOLD_RED = 0x21,
        THRESHOLD_YELLOW = 0x42,
        THRESHOLD_GREEN = 0x64,
    };

    void DeleteAllFilesInDirectory(char const*);
    CStr GetCurrentLevelName();
    int CloneWndWithChildren(m3d::ui::Wnd const*, m3d::ui::Wnd*);
    int GetWindowsSubDirs(CStr const&, retruxx::vector<CStr>&, CStr const&);
    int CreateWindowsDir(CStr const&);
    CStr GetMapNameFromFileName(CStr const&);
    CStr Color2Str(unsigned);
    void GetGunsForVehicle(int, retruxx::vector<ai::Obj*>&);
    ActionType GetRandomMoveAnimation(m3d::AnimatedModel*);
    bool IsBoss(ai::Obj const*);
    CStr GetClanAbbreviationByName(CStr const&);
    CStr GetClanFullNameByName(CStr const&);
    CStr GetClanNameByBelong(int);
    CStr GetCurrentLevelName();
    CStr GetKeysForImpulse(int);
    CStr GetMapNameFromFileName(CStr const&);
    CStr GetServiceSymbols();
    CStr GetServiceSymbolsForVisualisation();
    CStr GetVehiclePartNameByResourceId(int);

    void SetWndTextAlpha(m3d::ui::Wnd*, unsigned char);
}  // namespace help
