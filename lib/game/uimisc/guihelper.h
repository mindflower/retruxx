#pragma once
#include "skelmodel.h"
#include "core/stringm3d.h"
#include "math/point2d.h"
#include "retruxx/common.h"
#include "server/relationship.h"

struct CVector;

namespace m3d
{
    class AnimatedModel;
}
namespace ai
{
    class Obj;
    class Bar;
    class Town;
    class Building;
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
    // TODO: implement (ExMachina 1.02 NoCD RVA 0x1555b0). Returns the unified completion
    // status (QUESTSTATUS_*) of the quest identified by (questType, questId).
    UnifyQuestStatus GetQuestUnifyStatusByQuestId(help::QuestType questType, int questId);
    int CloneWndWithChildren(m3d::ui::Wnd const*, m3d::ui::Wnd*);
    int GetWindowsSubDirs(CStr const&, retruxx::vector<CStr>&, CStr const&);
    int CreateWindowsDir(CStr const&);
    CStr GetMapNameFromFileName(CStr const&);
    CStr Color2Str(unsigned);
    // Formats a float with a fixed number of decimals. precision == -1 means "use
    // the interface manager's default"; the result is clamped to [0, 10] decimals.
    CStr ftoa(float fVal, int precision);
    void GetGunsForVehicle(int, retruxx::vector<ai::Obj*>&);
    // Fills the vector with the canonical "move" animation set
    // {AT_BLOCK1, AT_BLOCK2, AT_DEATH1, AT_DEATH2} / "stand" set
    // {AT_STAND1, AT_STAND2, AT_MOVE1}. (ExMachina 1.02 NoCD RVA 0x555EE0 / 0x555F60.)
    void GetAllMoveAnimations(retruxx::vector<ActionType>& moveAnimations);
    void GetAllStandAnimations(retruxx::vector<ActionType>& standAnimations);
    // Picks one animation at random from the corresponding set; returns
    // AT_NUMTYPES when model is null. (RVA 0x555FC0 / 0x556040.)
    ActionType GetRandomMoveAnimation(m3d::AnimatedModel* model);
    ActionType GetRandomStandAnimation(m3d::AnimatedModel* model);
    // Re-resolves a font id at a different point size: looks up the pattern font,
    // then asks the GfxServer for a font with the same name/type/style scaled to
    // wantedFontSz. Returns -1 when the pattern font id is unknown.
    int GetScaledFontId(int patternFontId, float wantedFontSz);
    bool IsBoss(ai::Obj const*);
    CStr GetClanAbbreviationByName(CStr const&);
    CStr GetClanFullNameByName(CStr const&);
    CStr GetClanNameByBelong(int);
    CStr GetKeysForImpulse(int);
    CStr GetMapNameFromFileName(CStr const&);
    CStr GetServiceSymbols();
    CStr GetServiceSymbolsForVisualisation();
    CStr GetVehiclePartNameByResourceId(int);
    int RoundHealth(float);

    void SetWndTextAlpha(m3d::ui::Wnd*, unsigned char);

    bool WindowsDirExists(CStr const& dirPath);
    bool WindowsFileExists(CStr const& filePath);

    // True when w is non-null, still a live registered window in the station, and
    // has finished being created (Wnd::Valid()).
    bool IsWndValid(m3d::ui::Wnd const* w);

    int DestroyVehicle(int);

    // Price the workshop in `townId` pays the player for the object `objId`
    // (-1 bad args / no such town or object, -2 when the town has no workshop
    // that deals in it).
    int GetSellPriceByObjId(int objId, int townId);

    // Price the player pays the workshop in `townId` to buy the object `objId`
    // (-1 bad args / no such town or object, -3 when the town has no workshop
    // that deals in it). (ExMachina 1.02 NoCD RVA 0x550AA0.)
    int GetBuyPriceByObjId(int objId, int townId);

    // Repairs a Vehicle or a VehiclePart in place; no-op for anything else.
    void RepairObj(ai::Obj* o);

    // True when the object `objId` is a VehiclePart or Gadget that is compatible
    // with the vehicle `vehicleId`.
    bool IsChildObjCompatibleWithVehicle(int objId, int vehicleId);

    // Projects a world-space point into normalized ("relative") screen space,
    // i.e. the same coordinate space m3d::ui::Wnd bounds live in. (ExMachina
    // 1.02 NoCD RVA 0x154350.)
    PointBase<float> GetRelScreenPtByWorldPos(CVector const& worldPos);

    // Tolerance the local player has toward objId (RS_MAX when there is no
    // local player, or objId does not resolve to a live object).
    ai::eTolerance GetObjTolerance(int objId);

    // Small dispatchers over the Gun / CompoundGun split: gun may be either
    // kind (or null), and these forward to the matching accessor, returning
    // a safe default (0 / false) for anything else.
    bool CanGunFire(ai::Obj const* gun);
    bool CanGunShotToSeenObj(ai::Obj const* gun);
    bool IsGunWithCharging(ai::Obj const* gun);
    unsigned int GetGunChargeSize(ai::Obj const* gun);
    unsigned int GetGunShellsInCurrentCharge(ai::Obj const* gun);
    unsigned int GetGunShellsInPool(ai::Obj const* gun);
    float GetGunRechargingTime(ai::Obj const* gun);
    float GetGunCurrentRechargingTime(ai::Obj const* gun);

    ai::Bar* GetBarWithBarmanForTown(ai::Town const*);
    ai::Bar* GetBarWithoutBarmanForTown(ai::Town const*);
    ai::Building* GetShopForTown(ai::Town const*);
    ai::Building* GetWorkshopForTown(ai::Town const*);
}  // namespace help
