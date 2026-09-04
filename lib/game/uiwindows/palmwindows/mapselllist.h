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
        /* 0x0000 */ PointBase<float> m_icoSz;
        /* 0x0008 */ unsigned int m_textColor;
        /* 0x000c */ CStr m_linePaneName;
        /* 0x0018 */ float m_height;
        AuxInfo(MapSellItem::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x001c */

protected:
    /* 0x0220 */ MapSellItem::AuxInfo m_aif;
    /* 0x023c */ m3d::ui::ImageWnd* m_imgWareIco;
    /* 0x0240 */ m3d::ui::Wnd* m_lblSellPrice;
    /* 0x0244 */ m3d::ui::Wnd* m_lblBuyPrice;
    /* 0x0248 */ m3d::ui::LineWnd* m_line;
    /* 0x024c */ int m_warePrototypeId;
    /* 0x0250 */ int m_sellPrice;
    /* 0x0254 */ int m_buyPrice;

    MapSellItem();
    MapSellItem(MapSellItem const& rhs);

public:
    virtual ~MapSellItem() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classMapSellItem;
}; /* size: 0x0258 */

class MapSellList : public m3d::ui::ListBoxWnd<MapSellItem*>
{
public:
    int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    int SetUpForObject(ObjectInfo* objectInfo);

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override /* 0x138 */;
    virtual int DeleteItem(int itemIdx) override /* 0x13c */;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
    int CreateItemsFromRealObject();
    int CreateItemsFromSaveList();
    int CreateItem(int warePrototypeId, int sellPrice, int buyPrice, bool bLastItem);
    /* 0x0238 */ ObjectInfo* m_objectInfo;
    MapSellList();
    MapSellList(MapSellList const& rhs);

public:
    virtual ~MapSellList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classMapSellList;
}; /* size: 0x023c */
