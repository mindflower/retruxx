#pragma once

class GadgetWnd :  public ItemWnd
{
public:
    int GetSlotId() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    void SetSlotId(int);
    virtual int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual struct BoundsBase<float> GeomToWndBounds(struct BoundsBase<int> const &);
    void SetResourceName(CStr const &);
    virtual class m3d::Object * Clone();
    virtual ~GadgetWnd();
    virtual int GetResourceId() const ;
    CStr const & GetResourceName() const ;
protected:
    virtual void UpdateItemValue();
    virtual void DrawItemIco(struct m3d::ui::DrawInfo const &);
    virtual bool IsValid() const ;
    virtual bool CanContainItem() const ;
    virtual void LaunchEventOnItemDeactivation() const ;
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    virtual int GiveUpItem(struct ai::GeomRepositoryItem const &,class m3d::ui::Wnd *);
    virtual void SetupInfoWnd(class ItemInfoWnd *);
    void DrawBackground(struct m3d::ui::DrawInfo const &);
    GadgetWnd(class GadgetWnd const &);
    GadgetWnd();
    virtual void DrawFrame(struct m3d::ui::DrawInfo const &);
    virtual void DrawSelection(struct m3d::ui::DrawInfo const &);
    virtual bool SetItemObjId(int);
    virtual int GameDataUpdate(void *,int);
    virtual bool NeedUpdateInfoWnd(class ItemInfoWnd *) const ;
private:
    CStr m_resourceName;
    int m_slotId;
    BoundsBase<float> m_itemIcoBounds;
};
