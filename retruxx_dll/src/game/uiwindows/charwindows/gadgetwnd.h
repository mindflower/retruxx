#pragma once
#include "itemwnd.h"

class GadgetWnd :  public ItemWnd
{
public:
    int GetSlotId() const ;
    static m3d::Class * GetBaseClass();
    void SetSlotId(int);
    virtual int CreateFromPattern(m3d::ui::Wnd const *,bool);
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual BoundsBase<float> GeomToWndBounds(BoundsBase<int> const &);
    void SetResourceName(CStr const &);
    virtual m3d::Object * Clone();
    virtual ~GadgetWnd();
    virtual int GetResourceId() const ;
    CStr const & GetResourceName() const ;

protected:
    virtual void UpdateItemValue();
    virtual void DrawItemIco(m3d::ui::DrawInfo const &);
    virtual bool IsValid() const ;
    virtual bool CanContainItem() const ;
    virtual void LaunchEventOnItemDeactivation() const ;
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    virtual int GiveUpItem(ai::GeomRepositoryItem const &,m3d::ui::Wnd *);
    virtual void SetupInfoWnd(ItemInfoWnd *);
    void DrawBackground(m3d::ui::DrawInfo const &);
    GadgetWnd(GadgetWnd const &);
    GadgetWnd();
    virtual void DrawFrame(m3d::ui::DrawInfo const &);
    virtual void DrawSelection(m3d::ui::DrawInfo const &);
    virtual bool SetItemObjId(int);
    virtual int GameDataUpdate(void *,int);
    virtual bool NeedUpdateInfoWnd(ItemInfoWnd *) const ;

public:
    RT_CLASS_DECLARE(GadgetWnd);

private:
    CStr m_resourceName;
    int m_slotId;
    BoundsBase<float> m_itemIcoBounds;
};
