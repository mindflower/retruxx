#pragma once
#include <ui/ui.h>
#include <ui/uilistboxwnd.h>

namespace m3d
{
    namespace ui
    {
        class LineWnd;
        class ImageWnd;
    }
}

class MapSellItem : public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        PointBase<float> m_icoSz;
        unsigned int m_textColor;
        CStr m_linePaneName;
        float m_height;
    };

public:
    virtual m3d::Class* GetClass() const;
    int GetWarePrototypeId() const;
    int SetUp(int, int, int, PointBase<float> const&, float, bool);
    static m3d::Object* CreateObject();
    int GetBuyPrice() const;
    virtual m3d::Object* Clone();
    int GetSellPrice() const;
    static m3d::Class* GetBaseClass();
    virtual ~MapSellItem();

protected:
    MapSellItem(MapSellItem const&);
    MapSellItem();

public:
    RT_CLASS_DECLARE(MapSellItem);

private:
    MapSellItem::AuxInfo m_aif;
    m3d::ui::ImageWnd* m_imgWareIco;
    m3d::ui::Wnd* m_lblSellPrice;
    m3d::ui::Wnd* m_lblBuyPrice;
    m3d::ui::LineWnd* m_line;
    int m_warePrototypeId;
    int m_sellPrice;
    int m_buyPrice;
};

class MapSellList :  public m3d::ui::ListBoxWnd<MapSellItem *>
{
public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    int CreateFromPattern(m3d::ui::Wnd const *,bool);
    virtual ~MapSellList();
    int SetUpForObject(class ObjectInfo *);

protected:
    virtual int OnBeforeAddToWndStation();
    int CreateItemsFromSaveList();
    virtual int CompareItem(int,int);
    int CreateItemsFromRealObject();
    int CreateItem(int,int,int,bool);
    virtual int MeasureItem(int, BoundsBase<float> &) const ;
    virtual int DeleteItem(int);
    MapSellList(MapSellList const &);
    MapSellList();
    virtual int RenderItem(int, PointBase<float> const &, m3d::ui::DrawInfo const &);
    virtual int GameDataClear(bool);

public:
    RT_CLASS_DECLARE(MapSellList);

private:
    ObjectInfo *m_objectInfo;
};
