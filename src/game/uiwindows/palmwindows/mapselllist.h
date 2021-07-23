#pragma once

class MapSellList :  public m3d::ui::ListBoxWnd<MapSellItem *>
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    virtual ~MapSellList();
    int SetUpForObject(class ObjectInfo *);
protected:
    virtual int OnBeforeAddToWndStation();
    int CreateItemsFromSaveList();
    virtual int CompareItem(int,int);
    int CreateItemsFromRealObject();
    int CreateItem(int,int,int,bool);
    virtual int MeasureItem(int,struct BoundsBase<float> &) const ;
    virtual int DeleteItem(int);
    MapSellList(class MapSellList const &);
    MapSellList();
    virtual int RenderItem(int,class PointBase<float> const &,struct m3d::ui::DrawInfo const &);
    virtual int GameDataClear(bool);
private:
    ObjectInfo *m_objectInfo;
};

class MapSellItem :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    int GetWarePrototypeId() const ;
    int SetUp(int,int,int,class PointBase<float> const &,float,bool);
    static class m3d::Object * __fastcall CreateObject();
    int GetBuyPrice() const ;
    virtual class m3d::Object * Clone();
    int GetSellPrice() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~MapSellItem();
protected:
    MapSellItem(class MapSellItem const &);
    MapSellItem();
private:
    MapSellItem::AuxInfo m_aif;
    m3d::ui::ImageWnd *m_imgWareIco;
    m3d::ui::Wnd *m_lblSellPrice;
    m3d::ui::Wnd *m_lblBuyPrice;
    m3d::ui::LineWnd *m_line;
    int m_warePrototypeId;
    int m_sellPrice;
    int m_buyPrice;
};
