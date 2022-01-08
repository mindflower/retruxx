#pragma once

class ZnayuKakProdatWnd :  public ChildPanel
{
public:
    int GetWorkshopItemsCostToBuy() const ;
    static struct m3d::Class * GetBaseClass();
    enum Belong GetItemBelong(int) const ;
    int GetPlayerItemsCostToSell() const ;
    class ai::Vehicle * GetWorkshopVehicle() const ;
    enum TradeType GetTradeType() const ;
    virtual class m3d::Object * Clone();
    int SetupForTrade(int,int,int);
    int GetItemCost(int) const ;
    virtual ~ZnayuKakProdatWnd();
    int GetCurTradeCost() const ;
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
protected:
    int ClearCabinFromItems(int);
    int SaveGadgetsState(int,enum Belong,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    void OnRepositoryChanged(void *);
    bool CabinHasItems(int) const ;
    int RestoreCurValues(int,struct CurValues const &);
    void UpdateItemsCost();
    int ClearVehicleFromItems(int);
    int BuyCabin();
    bool IsItemRelevantForCurTradeState(struct ItemSaveInfo const *) const ;
    void Buy();
    int SaveVehicleItemsState(enum Belong,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    void ClearSaveInfo(class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    int GetVehicleId(enum Belong) const ;
    void UpdateOnCurTradeStateChanged();
    int TransferBasketItems(int,int);
    void FinishTrade(int);
    int RestoreTradeState();
    int BuyVehicle();
    int SaveCurValues(int,struct CurValues &);
    int GetBasketId(enum Belong) const ;
    int TransferVehicleParts(int,int,CStr const &);
    int SaveMainObject(int,enum Belong,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    int CalcOneItemCost(int,class ai::Workshop const *,enum Belong) const ;
    int RestoreGadgetsState(int,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > const &);
    virtual int OnAfterAddToWndStation();
    int ClearTradeState();
    class ai::Workshop * GetWorkshop() const ;
    int ClearBasketFromItems(int);
    bool BasketHasItems(int) const ;
    int ClearFromRepositoryItems(int);
    bool IsValid() const ;
    void UpdateTransferButtonsTooltips();
    int SaveBasketItemsState(enum Belong,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    int AddItemToWorkshop(int);
    bool VehicleHasItems(int) const ;
    int RestoreVehicleItemsState(int,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > const &);
    bool RemoveItemFromWorkshop(int);
    void TransferAll(enum Belong,enum Belong);
    int SaveTradeState(bool);
    int CheckLeftItems();
    enum Belong GetBelongByVehicleId(int) const ;
    int TransferGadgets(int,int);
    int RestoreRepositoryItemsState(int,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > const &);
    int TransferRepositoryItems(int,int);
    void OnVehiclePartChanged(void *);
    int GetCabinId(enum Belong) const ;
    void UpdatePlayerItemsCostWnd(int);
    void OnGadgetChanged(void *);
    int ReplaceParentPart(CStr const &);
    int RestoreBasketItemsState(int,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > const &);
    int ClearFromVehicleParts(int,CStr const &);
    virtual int OnAfterRemoveFromWndStation();
    class std::vector<CStr,class std::allocator<CStr> > GetChildVehiclePartNamesByParentPart(int,CStr const &) const ;
    void UpdateTradeCostWnd(int);
    int RepairThing(int);
    int RestoreVehiclePartsState(int,CStr const &,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > const &);
    int CancelTradeState();
    int ClearFromGadgets(int);
    void Cancel();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    int SellRepository();
    int TransferVehicleItems(int,int);
    ZnayuKakProdatWnd(class ZnayuKakProdatWnd const &);
    ZnayuKakProdatWnd();
    int RestoreCabinItemsState(int,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > const &);
    virtual int GameDataClear(bool);
    void UpdateWorkshopItemsCostWnd(int);
    int CheckGold(int);
    int SaveCabinItemsState(enum Belong,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    virtual int GameDataUpdate(void *,int);
    int MakeBottomPanel();
    int TransferCabinItems(int,int);
    virtual int GameDataSetup();
    int SaveVehiclePartsState(int,CStr const &,enum Belong,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    enum TradeType GetTradeTypeByObjectId(int) const ;
    int BuyBasket();
    int AddItem(struct ItemSaveInfo *,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    int SaveRepositoryItemsState(int,enum Belong,class std::map<int,struct ItemSaveInfo *,struct std::less<int>,class std::allocator<struct std::pair<int const ,struct ItemSaveInfo *> > > &);
    int CalcItemsCostByBelong(enum Belong) const ;
private:
    std::pair<int,ItemSaveInfo *>::pair<int,ItemSaveInfo *>(int const &,ItemSaveInfo * const &);
    ZnayuKakProdatWnd::AuxInfo m_aif;
    m3d::ui::Wnd *m_wndTradeCost;
    m3d::ui::Wnd *m_wndPlayerItemsCost;
    m3d::ui::Wnd *m_wndWorkshopItemsCost;
    m3d::ui::ButtonWnd *m_btnTransferItemsFromPlayerToWorkshop;
    m3d::ui::ButtonWnd *m_btnTransferItemsFromWorkshopToPlayer;
    int m_objectToTradeId;
    int m_workshopId;
    int m_workshopVehicleId;
    std::map<int,ZnayuKakProdatWnd::ItemSaveInfo *> m_originalPlayerItems;
    std::map<int,ZnayuKakProdatWnd::ItemSaveInfo *> m_originalWorkshopItems;
    std::map<int,ZnayuKakProdatWnd::ItemSaveInfo *> m_curPlayerItems;
    std::map<int,ZnayuKakProdatWnd::ItemSaveInfo *> m_curWorkshopItems;
    int m_playerItemsCostToSell;
    int m_workshopItemsCostToBuy;
    ZnayuKakProdatWnd::TradeType m_tradeType;
    ZnayuKakProdatWnd::CurValues m_playerCurValues;
    ZnayuKakProdatWnd::CurValues m_workshopCurValues;
};

namespace ZnayuKakProdatWnd
{
    class ItemSaveInfo
    {
    public:
        int SetupAsGadget(int,Belong,int);
        int GetGadgetSlotId() const ;
        Belong GetBelong() const ;
        ItemType GetItemType() const ;
        ItemSaveInfo();
        class PointBase<int> GetRepositoryPos() const ;
        ~ItemSaveInfo();
        int GetObjId() const ;
        int SetupAsVehiclePart(int,Belong,CStr const &,CStr const &);
        bool IsValid() const ;
        int SetupAsMainItem(int,Belong);
        void Invalidate();
        int SetupAsRepositoryItem(int,Belong,class PointBase<int> const &);
        CStr GetVehiclePartName() const ;
    protected:
    private:
        ItemType m_itemType;
        int m_gadgetSlotId;
        CStr m_vehiclePartName;
        PointBase<int> m_repositoryPos;
        int m_objId;
        Belong m_belong;
    };
}
