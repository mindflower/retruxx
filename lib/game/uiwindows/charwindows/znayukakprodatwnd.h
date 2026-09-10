#pragma once
#include "childpanel.h"
#include <map>

namespace ai
{
    class Workshop;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}  // namespace m3d

class MotherPanel;

class ZnayuKakProdatWnd : public ChildPanel
{
    // MotherPanel drives the trade panel and reads its state directly.
    friend class MotherPanel;

public:
    enum TradeType
    {
        TRADETYPE_VEHICLE = 0,
        TRADETYPE_CABIN = 1,
        TRADETYPE_BASKET = 2,
        TRADETYPE_NUM_TRADETYPES = 3,
        TRADETYPE_INVALID = 3,
    };
    enum Belong
    {
        BELONG_PLAYER = 0,
        BELONG_WORKSHOP = 1,
        BELONG_INVALID = 2,
    };

    int SetupForTrade(int workshopId, int objectToTradeId, int workshopVehicleId);
    ai::Vehicle* GetWorkshopVehicle() const;
    int GetCurTradeCost() const;
    int GetPlayerItemsCostToSell() const;
    int GetWorkshopItemsCostToBuy() const;
    int GetItemCost(int itemId) const;
    ZnayuKakProdatWnd::TradeType GetTradeType() const;
    ZnayuKakProdatWnd::Belong GetItemBelong(int itemId) const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndTradeCostName;
        /* 0x000c */ CStr m_wndPlayerItemsCostName;
        /* 0x0018 */ CStr m_wndWorkshopItemsCostName;
        /* 0x0024 */ CStr m_wndBottomPanelName;
        /* 0x0030 */ CStr m_btnTransferItemsFromPlayerToWorkshopName;
        /* 0x003c */ CStr m_btnTransferItemsFromWorkshopToPlayerName;
        /* 0x0048 */ CStr m_strIdLeftItemsInCabin;
        /* 0x0054 */ CStr m_strIdLeftItemsInBasket;
        /* 0x0060 */ CStr m_strIdLeftItemsInVehicle;
        /* 0x006c */ CStr m_strIdTooltipTransferItemsFromPlayerToWorkshop[3];
        /* 0x0090 */ CStr m_strIdTooltipTransferItemsFromWorkshopToPlayer[3];
        AuxInfo(ZnayuKakProdatWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x00b4 */

    enum ItemType
    {
        ITEMTYPE_GADGET = 0,
        ITEMTYPE_VEHICLE_PART_CABIN = 1,
        ITEMTYPE_VEHICLE_PART_BASKET = 2,
        ITEMTYPE_REPOSITORY_ITEM = 3,
        ITEMTYPE_MAIN_ITEM = 4,
        ITEMTYPE_INVALID = 5,
    };

    struct ItemSaveInfo
    {
        ItemSaveInfo(ZnayuKakProdatWnd::ItemSaveInfo const&);
        ItemSaveInfo();
        ~ItemSaveInfo();
        ZnayuKakProdatWnd::ItemType GetItemType() const;
        int GetObjId() const;
        ZnayuKakProdatWnd::Belong GetBelong() const;
        int SetupAsGadget(int objId, ZnayuKakProdatWnd::Belong belong, int gadgetSlotId);
        int SetupAsVehiclePart(
            int objId,
            ZnayuKakProdatWnd::Belong belong,
            CStr const& vehiclePartName,
            CStr const& parentPartName);
        int SetupAsRepositoryItem(int objId, ZnayuKakProdatWnd::Belong belong, PointBase<int> const& repositoryPos);
        int SetupAsMainItem(int objId, ZnayuKakProdatWnd::Belong belong);
        int GetGadgetSlotId() const;
        CStr GetVehiclePartName() const;
        PointBase<int> GetRepositoryPos() const;
        bool IsValid() const;
        void Invalidate();
        /* 0x0000 */ ZnayuKakProdatWnd::ItemType m_itemType;
        /* 0x0004 */ int m_gadgetSlotId;
        /* 0x0008 */ CStr m_vehiclePartName;
        /* 0x0014 */ PointBase<int> m_repositoryPos;
        /* 0x001c */ int m_objId;
        /* 0x0020 */ ZnayuKakProdatWnd::Belong m_belong;
    }; /* size: 0x0024 */

    using PartFloatValueMap = std::map<CStr, float>;
    using PartFloatValuePair = std::pair<CStr, float>;
    using PartIntValueMap = std::map<CStr, int>;
    using PartIntValuePair = std::pair<CStr, int>;

    struct CurValues
    {
        /* 0x0000 */ float m_hp;
        /* 0x0004 */ float m_fuel;
        /* 0x0008 */ std::map<CStr, float> m_durability;
        /* 0x0014 */ std::map<CStr, int> m_shellsInCurCharge;
        /* 0x0020 */ std::map<CStr, int> m_shellsInPool;
        CurValues(ZnayuKakProdatWnd::CurValues const&);
        CurValues();
        void Clear();
    }; /* size: 0x002c */

    using ItemSaveInfoMap = std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>;
    using ItemSaveInfoPair = std::pair<int, ZnayuKakProdatWnd::ItemSaveInfo*>;

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    void UpdateTradeCost();
    void Buy();
    void Cancel();
    void TransferAll(ZnayuKakProdatWnd::Belong srcBelong, ZnayuKakProdatWnd::Belong dstBelong);
    ZnayuKakProdatWnd::TradeType GetTradeTypeByObjectId(int objectToTradeId) const;
    int SaveTradeState(bool bOriginaltState);
    int ClearTradeState();
    int RestoreTradeState();
    int SaveVehicleItemsState(
        ZnayuKakProdatWnd::Belong belong,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    int SaveCabinItemsState(
        ZnayuKakProdatWnd::Belong belong,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    int SaveBasketItemsState(
        ZnayuKakProdatWnd::Belong belong,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    int SaveMainObject(
        int objId,
        ZnayuKakProdatWnd::Belong belong,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    int SaveGadgetsState(
        int vehicleId,
        ZnayuKakProdatWnd::Belong belong,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    int SaveVehiclePartsState(
        int vehicleId,
        CStr const& parentPartName,
        ZnayuKakProdatWnd::Belong belong,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    int SaveRepositoryItemsState(
        int vehicleId,
        ZnayuKakProdatWnd::Belong belong,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    int SaveCurValues(int vehicleId, ZnayuKakProdatWnd::CurValues& curValues);
    int ClearVehicleFromItems(int vehicleId);
    int ClearCabinFromItems(int vehicleId);
    int ClearBasketFromItems(int vehicleId);
    int ClearFromGadgets(int vehicleId);
    int ClearFromVehicleParts(int vehicleId, CStr const& parentPartName);
    int ClearFromRepositoryItems(int vehicleId);
    std::vector<CStr> GetChildVehiclePartNamesByParentPart(int vehicleId, CStr const& parentPartName) const;

    int RestoreVehicleItemsState(
        int vehicleId,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> const& containerToRestoreFrom);
    int RestoreCabinItemsState(
        int vehicleid,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> const& containerToRestoreFrom);
    int RestoreBasketItemsState(
        int vehicleId,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> const& containerToRestoreFrom);
    int RestoreGadgetsState(
        int vehicleId,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> const& containerToRestoreFrom);
    int RestoreVehiclePartsState(
        int vehicleId,
        CStr const& parentPartName,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> const& containerToRestoreFrom);
    int RestoreRepositoryItemsState(
        int vehicleId,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> const& containerToRestoreFrom);
    int RestoreCurValues(int vehicleId, ZnayuKakProdatWnd::CurValues const& curValues);

    int GetVehicleId(ZnayuKakProdatWnd::Belong belong) const;
    int GetCabinId(ZnayuKakProdatWnd::Belong belong) const;
    int GetBasketId(ZnayuKakProdatWnd::Belong belong) const;
    int AddItem(
        ZnayuKakProdatWnd::ItemSaveInfo* item,
        std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& containerToSave);
    void ClearSaveInfo(std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*>& items);
    bool IsValid() const;
    void FinishTrade(int ret);
    int CalcItemsCostByBelong(ZnayuKakProdatWnd::Belong belong) const;
    int CalcOneItemCost(int itemId, ai::Workshop const* workshop, ZnayuKakProdatWnd::Belong belong) const;
    void UpdateItemsCost();
    void UpdateTradeCostWnd(int summaryCost);
    void UpdatePlayerItemsCostWnd(int playerItemsCost);
    void UpdateWorkshopItemsCostWnd(int workshopItemsCost);
    void UpdateOnCurTradeStateChanged();
    void OnVehiclePartChanged(void* data);
    void OnGadgetChanged(void* data);
    void OnRepositoryChanged(void* data);
    int BuyVehicle();
    int BuyCabin();
    int BuyBasket();
    int ReplaceParentPart(CStr const& parentPartName);
    int SellRepository();
    int AddItemToWorkshop(int itemId);
    bool RemoveItemFromWorkshop(int itemId);
    void DestroyHackedVehicle();
    bool IsItemRelevantForCurTradeState(ZnayuKakProdatWnd::ItemSaveInfo const* item) const;
    int CancelTradeState();
    int TransferVehicleItems(int srcVehicleId, int dstVehicleId);
    int TransferCabinItems(int srcVehicleId, int dstVehicleId);
    int TransferBasketItems(int srcVehicleId, int dstVehicleId);
    int TransferGadgets(int srcVehicleId, int dstVehicleId);
    int TransferVehicleParts(int srcVehicleId, int dstVehicleId, CStr const& parentPartName);
    int TransferRepositoryItems(int srcVehicleId, int dstVehicleId);
    ZnayuKakProdatWnd::Belong GetBelongByVehicleId(int vehicleId) const;
    ai::Workshop* GetWorkshop() const;
    int RepairThing(int itemId);
    int CheckGold(int tradeCost);
    int CheckLeftItems();
    bool CabinHasItems(int vehicleId) const;
    bool BasketHasItems(int vehicleId) const;
    bool VehicleHasItems(int vehicleId) const;
    int MakeBottomPanel();
    void UpdateTransferButtonsTooltips();

    /* 0x0224 */ ZnayuKakProdatWnd::AuxInfo m_aif;
    /* 0x02d8 */ m3d::ui::Wnd* m_wndTradeCost;
    /* 0x02dc */ m3d::ui::Wnd* m_wndPlayerItemsCost;
    /* 0x02e0 */ m3d::ui::Wnd* m_wndWorkshopItemsCost;
    /* 0x02e4 */ m3d::ui::ButtonWnd* m_btnTransferItemsFromPlayerToWorkshop;
    /* 0x02e8 */ m3d::ui::ButtonWnd* m_btnTransferItemsFromWorkshopToPlayer;
    /* 0x02ec */ int m_objectToTradeId;
    /* 0x02f0 */ int m_workshopId;
    /* 0x02f4 */ int m_workshopVehicleId;
    /* 0x02f8 */ std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> m_originalPlayerItems;
    /* 0x0304 */ std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> m_originalWorkshopItems;
    /* 0x0310 */ std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> m_curPlayerItems;
    /* 0x031c */ std::map<int, ZnayuKakProdatWnd::ItemSaveInfo*> m_curWorkshopItems;
    /* 0x0328 */ int m_playerItemsCostToSell;
    /* 0x032c */ int m_workshopItemsCostToBuy;
    /* 0x0330 */ ZnayuKakProdatWnd::TradeType m_tradeType;
    /* 0x0334 */ ZnayuKakProdatWnd::CurValues m_playerCurValues;
    /* 0x0360 */ ZnayuKakProdatWnd::CurValues m_workshopCurValues;

    ZnayuKakProdatWnd();
    ZnayuKakProdatWnd(ZnayuKakProdatWnd const& rhs);

public:
    virtual ~ZnayuKakProdatWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classZnayuKakProdatWnd;
}; /* size: 0x038c */
