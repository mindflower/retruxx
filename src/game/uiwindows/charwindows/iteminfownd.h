#pragma once

class ItemInfoWnd :  public InfoWnd
{
public:
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    struct ai::GeomRepositoryItem const & GetItem() const ;
    void SetUpForRepositoryItem(struct ai::GeomRepositoryItem const &,class ref_ptr<class m3d::ui::Wnd>);
    void SetUpForObject(int,class ref_ptr<class m3d::ui::Wnd>);
    class ref_ptr<class m3d::ui::Wnd> GetSrcWnd() const ;
    virtual ~ItemInfoWnd();
    static struct m3d::Class * __fastcall GetBaseClass();
    class ai::Obj const * GetObjectA() const ;
    virtual class m3d::Object * Clone();
protected:
    int Update(bool);
    int ShowInfoForCabin(bool);
    int ShowInfoForVehiclePart(bool);
    int ShowPropertyInfo(enum Property,bool);
    int ShowInfoForGoods(bool);
    int ShowInfoForGold(bool);
    bool NeedShowChargeSize() const ;
    int ShowInfoForGun(bool);
    int ShowInfoForBasket(bool);
    ItemInfoWnd();
    ItemInfoWnd(class ItemInfoWnd const &);
    int ShowInfoBase(bool);
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    bool IsValid() const ;
    int ShowInfoForGadget(bool);
    int GetResourceId() const ;
    bool NeedShowPrice() const ;
private:
    ItemInfoWnd::InfoType m_infoType;
    ai::GeomRepositoryItem m_repositoryItem;
    int m_objId;
    CStr m_auxFullName;
    ref_ptr<m3d::ui::Wnd> m_srcWnd;
};
