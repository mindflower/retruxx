#pragma once
#include "skelmodel.h"
#include "core/stringm3d.h"
#include "math/point2d.h"
#include "retruxx/common.h"
#include "server/relationship.h"
#include "game/uimisc/navpoint.h"
#include "server/objects/guns/gun.h"

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
    class Vehicle;
    class GeomRepository;
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

    enum ObjectOwnerType
    {
        OWNER_PLAYER = 0,
        OWNER_TOWN = 1,
        OWNER_OTHER = 2,
        OWNER_INVALID = 3,
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
    // Unified completion status (QUESTSTATUS_*) of the quest identified by
    // (questType, questId); QUESTSTATUS_INVALID when it does not resolve.
    UnifyQuestStatus GetQuestUnifyStatusByQuestId(help::QuestType questType, int questId);

    // True when the quest is still in progress and its quest-info knows where
    // the target sits on the current level - only then is there somewhere for a
    // navigation point to actually point.
    bool CanNavPointBeAddedOnQuest(help::QuestType questType, int questId);

    // True when the player is at war with `enemyBelong` and a peace quest for
    // that clan is on offer (created but not yet finished), so the reputation
    // screen can advertise that the war can still be ended.
    bool IsPeaceWithEnemyAvailable(int enemyBelong);

    // Maps a quest kind onto the nav-point object kind used to look navigation
    // points up; anything that is not a real quest type maps to OBJECT_TYPE_INVALID.
    NavPoint::ObjectType GetNpObjectTypeByQuestType(help::QuestType questType);
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
    // Human-readable list of the key combinations bound to `impulseId`, e.g.
    // "Ctrl" + "F1", "F5" - alternatives separated by ", ", keys within one
    // combination joined with " + ".
    CStr GetKeysForImpulse(int impulseId);
    // The above prefixed with the localized "KeySet" caption, ready to be used
    // as a window tooltip.
    CStr CreateTooltipForImpulse(int impulseId);
    // The local player's current vehicle (null when there is no local player or
    // it is not driving anything), and its object id (-1 in the same cases).
    ai::Vehicle* GetPlayerVehicle();
    int GetPlayerVehicleId();
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

    // Removes the vehicle `vehicleId` from the world. Returns 1 when there was
    // nothing to do (id == -1) or the vehicle was removed, 0 when the id does
    // not resolve to a live Vehicle.
    int DestroyVehicle(int vehicleId);

    // Spawns a fresh, unowned vehicle from `prototypeId` (named "VehicleToSell";
    // used to build the throwaway "hacked" vehicle the workshop/trade UI previews
    // parts on). Null when the prototype is unset or does not produce a Vehicle.
    ai::Vehicle* CreateVehicleFromPrototype(int prototypeId);

    // Detaches and removes every part of the vehicle except its chassis.
    void RemoveAllPartsFromVehicle(ai::Vehicle* vehicle);

    // Price the workshop in `townId` pays the player for the object `objId`
    // (-1 bad args / no such town or object, -2 when the town has no workshop
    // that deals in it).
    int GetSellPriceByObjId(int objId, int townId);

    // Price the player pays the workshop in `townId` to buy the object `objId`
    // (-1 bad args / no such town or object, -3 when the town has no workshop
    // that deals in it). (ExMachina 1.02 NoCD RVA 0x550AA0.)
    int GetBuyPriceByObjId(int objId, int townId);
    // Price the town's workshop pays for one unit of `objPrototypeId`, or -1 for
    // bad ids and -2 when no workshop in the town deals in it at all.
    int GetSellPriceByPrototypeId(int objPrototypeId, int townId);
    // Price the town's workshop charges for one unit, or -1 for bad ids and -3
    // when the town will not sell it (no workshop, or the article is not on
    // sale).
    int GetBuyPriceByPrototypeId(int objPrototypeId, int townId);
    bool CanWareBeBuyed(int warePrototypeId, int townId);

    // True when the vehicle-part name maps onto a resource that is a kind of
    // GUN, i.e. the slot holds a weapon rather than any other attachment.
    bool IsVehiclePartNameAGunPartName(CStr const& vpName);
    // Every part name the vehicle's prototype declares that can currently be
    // attached to it; `vpNames` is cleared first.
    void GetAllVehiclePartsThatCanBeAttached(int vehicleId, std::vector<CStr, std::allocator<CStr>>& vpNames);
    // The same list narrowed to the gun slots.
    void GetGunPartNamesThatCanBeAttached(int vehicleId, std::vector<CStr, std::allocator<CStr>>& gunPartNames);

    // Repairs a Vehicle or a VehiclePart in place; no-op for anything else.
    void RepairObj(ai::Obj* o);

    // True when the object `objId` is a VehiclePart or Gadget that is compatible
    // with the vehicle `vehicleId`.
    bool IsChildObjCompatibleWithVehicle(int objId, int vehicleId);

    // Object ids of everything sitting in `repository` whose prototype resource
    // is a kind of `resourceId`. `objIds` is cleared first.
    void GetObjetsInRepositoryByResourceType(
        ai::GeomRepository const* repository,
        int resourceId,
        std::vector<int, std::allocator<int>>& objIds);

    // True when the vehicle has at least one attachable part slot whose resource
    // the vehicle part `vpId` is a kind of.
    bool IsVehiclePartCompatibleWithVehicle(int vpId, int vehicleId);

    // Object ids of the vehicle parts on sale in `workshopId` (in the repository
    // that deals in `vpResourceId`) that fit the vehicle `vehicleId`. `ids` is
    // cleared first, and stays empty for anything that is not a workshop, not a
    // vehicle, or a resource that is not a VEHICLE_PART.
    void GetCompatibleVehiclePartsFromWorkshop(
        int workshopId,
        int vpResourceId,
        int vehicleId,
        std::vector<int, std::allocator<int>>& ids);

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
    void SetGunShellsInCurrentCharge(ai::Obj* gun, int value);
    void SetGunShellsInPool(ai::Obj* gun, int value);
    // Prototype id of the shell the gun fires, or -1 when `gun` is neither a
    // Gun nor a CompoundGun.
    int GetGunShellPrototypeId(ai::Obj const* gun);
    float GetGunRechargingTime(ai::Obj const* gun);
    float GetGunCurrentRechargingTime(ai::Obj const* gun);

    ai::Bar* GetBarWithBarmanForTown(ai::Town const*);
    ai::Bar* GetBarWithoutBarmanForTown(ai::Town const*);
    ai::Building* GetShopForTown(ai::Town const*);
    ai::Building* GetWorkshopForTown(ai::Town const*);

    ai::FiringTypes GetGunFiringType(ai::Obj const* gun);

    // More of the same Gun / CompoundGun dispatchers.
    bool IsGunWithShellsPoolLimit(ai::Obj const* gun);
    // True for a gun that both charges and draws from a limited shell pool, i.e.
    // one the workshop can actually reload.
    bool CanGunBeReloaded(ai::Obj const* gun);
    // Price of one unit of fuel for `vehicleId` in `townId`; -1 when either does
    // not resolve.
    int GetFuelPriceForOneUnit(int vehicleId, int townId);
    unsigned int GetGunShellsPoolSize(ai::Obj const* gun);
    ai::DamageType GetGunDamageType(ai::Obj const* gun);
    float GetGunDamage(ai::Obj const* gun);
    float GetGunDurability(ai::Obj const* gun);
    float GetGunMaxDurability(ai::Obj const* gun);
    bool IsGunDurabilityEnoughForFiring(ai::Obj const* gun);
    // Descends through compound guns to the first bullet launcher and reports its
    // clamped accuracy; 100 for a gun that is not a bullet launcher at all, 0 for
    // null or a compound with nothing in it.
    float GetGunAccuracy(ai::Obj const* gun);

    // Localizable name of a damage type ("Piercing", "Blast", "Energy",
    // "Water"); empty for anything else.
    CStr DamageType2Str(ai::DamageType damageType);

    // Who the object currently belongs to, used to decide whether a price is a
    // buy price or a sell price.
    ObjectOwnerType GetObjectOwnerType(int objId);

    // The object's undiscounted list price (Obj::GetPrice with no coefficient
    // provider); -1 when objId does not resolve.
    int GetBaseBuyPriceByObjId(int objId);

    // The price to show for `objId` in whatever context it is being looked at:
    // the trade window's own valuation when that window is up, otherwise the
    // town's sell price for the player's goods, its buy price for the town's, and
    // half the list price for anything else.
    int GetPriceSmart(int objId);

    // Normalizes an angle (radians) to [0, 2*pi).
    float Angle0To2Pi(float angle);
    // Normalizes an angle (radians) to [-pi, pi].
    float AngleMinusPiToPi(float angle);

    // Sum of obj's own property value across every object the given vehicle
    // property is sourced from (e.g. MaxSpeed/Torque/Control come from the
    // cabin; Piercing/Blast/Energy/MaxDurability come from the chassis),
    // reversing any installed gadget's modification of that property so the
    // result is the vehicle's pre-upgrade baseline. Returns 0 if the property
    // does not resolve or is not numeric.
    float GetBasePropertyValFromVehicle(ai::Vehicle const* vehicle, int propertyId);

    // As above but for a single object rather than a whole vehicle - obj's own
    // current property value with every applicable installed gadget
    // modification on vehicle undone.
    float GetBasePropertyValFromObj(ai::Obj const* obj, ai::Vehicle const* vehicle, int propertyId);
}  // namespace help
