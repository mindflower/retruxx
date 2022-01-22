#pragma once
#include "infownd.h"
#include <core/ref_ptr.h>
#include <server/geomrepositoryitem.h>

class ItemInfoWnd :  public InfoWnd
{
public:
    enum InfoType
    {
        INFOTYPE_REPOSITORY_ITEM = 0x0,
        INFOTYPE_OBJECT = 0x1,
    };

public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    ai::GeomRepositoryItem const & GetItem() const ;
    void SetUpForRepositoryItem(ai::GeomRepositoryItem const &,ref_ptr<m3d::ui::Wnd>);
    void SetUpForObject(int,ref_ptr<m3d::ui::Wnd>);
    ref_ptr<m3d::ui::Wnd> GetSrcWnd() const ;
    virtual ~ItemInfoWnd();
    static m3d::Class * GetBaseClass();
    ai::Obj const * GetObjectA() const ;
    virtual m3d::Object * Clone();

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
    ItemInfoWnd(ItemInfoWnd const &);
    int ShowInfoBase(bool);
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    bool IsValid() const ;
    int ShowInfoForGadget(bool);
    int GetResourceId() const ;
    bool NeedShowPrice() const ;

public:
    RT_CLASS_DECLARE(ItemInfoWnd);

private:
    ItemInfoWnd::InfoType m_infoType;
    ai::GeomRepositoryItem m_repositoryItem;
    int m_objId;
    CStr m_auxFullName;
    ref_ptr<m3d::ui::Wnd> m_srcWnd;
};
