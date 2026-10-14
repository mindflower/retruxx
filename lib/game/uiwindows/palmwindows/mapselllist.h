#pragma once
#include <ui/ui.h>
#include <ui/uilistboxwnd.h>

namespace m3d
{
    namespace ui
    {
        class LineWnd;
        class ImageWnd;
    }  // namespace ui
}  // namespace m3d

class ObjectInfo;

class MapSellItem : public m3d::ui::Wnd
{
public:
    int SetUp(
        int warePrototypeId,
        int sellPrice,
        int buyPrice,
        PointBase<float> const& origin,
        float maxWidth,
        bool bLastItem);
    int GetWarePrototypeId() const;
    int GetSellPrice() const;
    int GetBuyPrice() const;

    struct AuxInfo
    {
        PointBase<float> m_icoSz;
        unsigned int m_textColor;
        CStr m_linePaneName;
        float m_height;
        AuxInfo(MapSellItem::AuxInfo const&);
        AuxInfo();
    };

protected:
    MapSellItem::AuxInfo m_aif;
    m3d::ui::ImageWnd* m_imgWareIco;
    m3d::ui::Wnd* m_lblSellPrice;
    m3d::ui::Wnd* m_lblBuyPrice;
    m3d::ui::LineWnd* m_line;
    int m_warePrototypeId;
    int m_sellPrice;
    int m_buyPrice;

    MapSellItem();
    MapSellItem(MapSellItem const& rhs);

public:
    virtual ~MapSellItem() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMapSellItem;
};

class MapSellList : public m3d::ui::ListBoxWnd<MapSellItem*>
{
public:
    int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    int SetUpForObject(ObjectInfo* objectInfo);

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override;
    virtual int DeleteItem(int itemIdx) override;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override;
    int CreateItemsFromRealObject();
    int CreateItemsFromSaveList();
    int CreateItem(int warePrototypeId, int sellPrice, int buyPrice, bool bLastItem);
    ObjectInfo* m_objectInfo;
    MapSellList();
    MapSellList(MapSellList const& rhs);

public:
    virtual ~MapSellList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMapSellList;
};
