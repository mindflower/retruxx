#include "mapselllist.h"

#include <core/kernel.h>
#include <core/log.h>

#include <math/vector2.h>

#include <game/m3dgame.h>
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/levelinfo.h"
#include "localmapwnd.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/town.h"

#include <ui/frame.h>
#include <ui/image.h>
#include <ui/linewnd.h>
#include <ui/ui_srv.h>

// ============================================================================
//  MapSellItem
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(MapSellItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapSellItem);

MapSellItem::AuxInfo::AuxInfo()
{
    // RVA 0xF16B0
    m_icoSz = PointBase<float>{38.0f, 38.0f};
    m_textColor = 0xFF404040u;
    m_linePaneName = "PaneLineMinimap";
    m_height = 40.0f;
}

MapSellItem::MapSellItem()
{
    m_imgWareIco = nullptr;
    m_lblSellPrice = nullptr;
    m_lblBuyPrice = nullptr;
    m_line = nullptr;
    m_warePrototypeId = -1;
    m_sellPrice = 0;
    m_buyPrice = 0;
}

MapSellItem::MapSellItem(MapSellItem const& rhs) : Wnd(rhs)
{
    m_imgWareIco = nullptr;
    m_lblSellPrice = nullptr;
    m_lblBuyPrice = nullptr;
    m_line = nullptr;
    m_warePrototypeId = -1;
    m_sellPrice = 0;
    m_buyPrice = 0;
}

MapSellItem::~MapSellItem()
{
    // The image / price labels / separator line are owned children, released by
    // the base destructor.
}

m3d::Object* MapSellItem::Clone()
{
    // RVA 0xF1570
    return new MapSellItem(*this);
}

m3d::Object* MapSellItem::CreateObject()
{
    // RVA 0xF1680
    return new MapSellItem;
}

m3d::Class* MapSellItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* MapSellItem::GetClass() const
{
    // RVA 0xF1670
    return RT_CLASS_LOCAL(MapSellItem);
}

int MapSellItem::GetWarePrototypeId() const
{
    // RVA 0xF1E70
    return m_warePrototypeId;
}

int MapSellItem::GetSellPrice() const
{
    // RVA 0xF1E80
    return m_sellPrice;
}

int MapSellItem::GetBuyPrice() const
{
    // RVA 0xF1E90
    return m_buyPrice;
}

int MapSellItem::SetUp(
    int warePrototypeId, int sellPrice, int buyPrice, PointBase<float> const& origin, float maxWidth, bool bLastItem)
{
    // RVA 0xF1870
    if (warePrototypeId == -1)
    {
        return 0;
    }

    BoundsBase<float> b{origin.x, origin.y, maxWidth, m_aif.m_height};
    if (!m3d::ui::Wnd::Create({}, 0x260u, b, 0))
    {
        return 0;
    }
    m_sellPrice = sellPrice;
    m_warePrototypeId = warePrototypeId;
    m_buyPrice = buyPrice;

    SetProperty(PROP_WND_TOOLTIP, &ai::thePrototypeManager->GetPrototypeFullName(warePrototypeId));

    float const columnW = maxWidth * 0.33333334f;

    // --- ware icon -----------------------------------------------------------
    m_imgWareIco = static_cast<m3d::ui::ImageWnd*>(M3D_KERNEL->New("ImageWnd"));
    if (!m_imgWareIco)
    {
        return 0;
    }
    BoundsBase<float> icoB;
    icoB.x0 = (columnW - m_aif.m_icoSz.x) * 0.5f;
    icoB.y0 = (m_aif.m_height - m_aif.m_icoSz.y) * 0.5f;
    icoB.width = m_aif.m_icoSz.x;
    icoB.height = m_aif.m_icoSz.y;

    m3d::rend::TexHandle const wareTex =
        M3D_APP->m_pInterfaceManager->GetIcoByName(ai::thePrototypeManager->GetPrototypeName(warePrototypeId), 0);
    if (!m_imgWareIco->CreateImageWnd(icoB, wareTex))
    {
        delete m_imgWareIco;
        m_imgWareIco = nullptr;
        return 0;
    }
    m_imgWareIco->SetStyle(768);
    AddChild(m_imgWareIco);

    // --- sell price label --------------------------------------------------
    m_lblSellPrice = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_lblSellPrice)
    {
        return 0;
    }
    BoundsBase<float> sellB{columnW, 0.0f, columnW, GetBounds().height};
    CStr const sellCaption = (m_sellPrice < 0) ? CStr("-") : CStr(m_sellPrice);
    if (!m_lblSellPrice->Create(sellCaption, 3904u, sellB, 0))
    {
        delete m_lblSellPrice;
        m_lblSellPrice = nullptr;
        return 0;
    }
    m_lblSellPrice->SetDefaultFont(1);
    m_lblSellPrice->SetWrapMode(m3d::TW_NOWRAP);
    m_lblSellPrice->SetTextColor(m_aif.m_textColor);
    AddChild(m_lblSellPrice);

    // --- buy price label -------------------------------------------------
    m_lblBuyPrice = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_lblBuyPrice)
    {
        return 0;
    }
    float const buyX = sellB.width + sellB.x0;
    BoundsBase<float> buyB{buyX, 0.0f, GetBounds().width - buyX, GetBounds().height};
    CStr const buyCaption = (m_buyPrice < 0) ? CStr("-") : CStr(m_buyPrice);
    if (!m_lblBuyPrice->Create(buyCaption, 3904u, buyB, 0))
    {
        delete m_lblBuyPrice;
        m_lblBuyPrice = nullptr;
        return 0;
    }
    m_lblBuyPrice->SetDefaultFont(1);
    m_lblBuyPrice->SetWrapMode(m3d::TW_NOWRAP);
    m_lblBuyPrice->SetTextColor(m_aif.m_textColor);
    AddChild(m_lblBuyPrice);

    // --- separator line (every row except the last) ---------------------
    if (!bLastItem)
    {
        m_line = static_cast<m3d::ui::LineWnd*>(M3D_KERNEL->New("LineWnd"));
        if (!m_line)
        {
            return 0;
        }
        float barWidth = 0.0f;
        if (m3d::ui::Pane* pane = GetGfxServer()->GetPane(m_aif.m_linePaneName))
        {
            if (m3d::ui::Frame* frame = pane->m_frame[0])
            {
                barWidth = static_cast<float>(frame->m_barUsedWidth);
            }
        }
        float const h = GetBounds().height;
        // NOTE: the shipped build computes the line height as
        // barWidth - (h - barWidth); reproduced verbatim.
        BoundsBase<float> lineB{0.0f, h - barWidth, GetBounds().width, barWidth - (h - barWidth)};
        if (!m_line->Create(lineB, 0x300u, m3d::ui::LINEWND_HORIZONTAL))
        {
            delete m_line;
            m_line = nullptr;
            return 0;
        }
        m_line->SetPane(m_aif.m_linePaneName);
        AddChild(m_line);
    }
    return 1;
}

// ============================================================================
//  MapSellList
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(MapSellList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapSellList);

MapSellList::MapSellList()
{
    // NOTE: the shipped ctor also seeds a couple of ListBoxWnd base fields (a -1
    // index and a 2.0f spacing) that the retruxx template ctor already sets.
    m_objectInfo = nullptr;
}

MapSellList::MapSellList(MapSellList const& rhs) : ListBoxWnd<MapSellItem*>(rhs)
{
    // RVA 0xF15B0: fresh list, no object bound.
    m_objectInfo = nullptr;
}

MapSellList::~MapSellList()
{
    // Items are owned children, released with the base destructor / RemoveAllItems.
}

m3d::Object* MapSellList::Clone()
{
    // RVA 0xF15B0
    return new MapSellList(*this);
}

m3d::Object* MapSellList::CreateObject()
{
    // RVA 0xF1EC0
    return new MapSellList;
}

m3d::Class* MapSellList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* MapSellList::GetClass() const
{
    // RVA 0xF1EB0
    return RT_CLASS_LOCAL(MapSellList);
}

int MapSellList::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    // RVA 0xF2170
    if (!patternWnd)
    {
        return 0;
    }

    if (!Create(patternWnd->GetBounds(), patternWnd->GetStyle(), patternWnd->GetId()))
    {
        return 0;
    }

    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());
    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetTextColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    SetDrawFlags(7);

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        delete const_cast<m3d::ui::Wnd*>(patternWnd);
    }

    m_gameDataFlags |= 1u;
    return 1;
}

int MapSellList::SetUpForObject(ObjectInfo* objectInfo)
{
    // RVA 0xF23E0
    if (!objectInfo)
    {
        m_objectInfo = nullptr;
        GameDataClear(false);
        return 1;
    }
    if (!LocalMapWnd::IsObjectInfoValidForPrices(objectInfo))
    {
        m_objectInfo = nullptr;
        GameDataClear(false);
        return 0;
    }

    m_objectInfo = objectInfo;

    ai::Town* currentTown = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    bool const useRealObject = currentTown != nullptr &&
        objectInfo->GetLevelName() == help::GetCurrentLevelName() && objectInfo->GetId() == currentTown->GetId();

    if (useRealObject)
    {
        return CreateItemsFromRealObject();
    }
    if (objectInfo->IsTown() && !objectInfo->GetSavedPrices()->empty())
    {
        return CreateItemsFromSaveList();
    }
    return GameDataClear(false);
}

int MapSellList::OnBeforeAddToWndStation()
{
    // RVA 0xF24F0
    SetUpForObject(m_objectInfo);
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int MapSellList::CreateItemsFromRealObject()
{
    // TODO(RVA 0xF2510: rebuild the list from the town's live shop prices via
    //      help::GetWarePricesForTown (RVA 0x155420), which itself needs the
    //      by-prototype-id buy/sell pricing helpers)
    RemoveAllItems();
    RETRUXX_NOT_IMPLEMENTED;
}

int MapSellList::CreateItemsFromSaveList()
{
    // RVA 0xF26B0
    RemoveAllItems();
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!m_objectInfo)
    {
        return 0;
    }

    int res = 1;
    if (m_objectInfo->IsTown())
    {
        auto const* saved = m_objectInfo->GetSavedPrices();
        for (auto it = saved->begin(); it != saved->end(); ++it)
        {
            auto nextIt = it;
            ++nextIt;
            bool const isLast = nextIt == saved->end();
            res &= CreateItem(it->first, static_cast<int>(it->second.x), static_cast<int>(it->second.y), isLast);
        }
    }
    return res;
}

int MapSellList::CreateItem(int warePrototypeId, int sellPrice, int buyPrice, bool bLastItem)
{
    // RVA 0xF2760
    if (warePrototypeId == -1)
    {
        return 0;
    }
    auto* item = static_cast<MapSellItem*>(M3D_KERNEL->New("MapSellItem"));
    if (!item)
    {
        return 0;
    }
    BoundsBase<float> const clientB = GetClientBounds();
    PointBase<float> const origin{clientB.x0, clientB.y0};
    if (!item->SetUp(warePrototypeId, sellPrice, buyPrice, origin, clientB.width, bLastItem))
    {
        delete item;
        return 0;
    }
    AddItem(item);
    AddChild(item);
    return 1;
}

int MapSellList::CompareItem(int, int)
{
    // RVA 0xF2160
    return 0;
}

int MapSellList::DeleteItem(int itemIdx)
{
    // RVA 0xF2120
    if (MapSellItem* item = m_items[itemIdx].m_item)
    {
        item->DecRef();
    }
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

int MapSellList::GameDataClear(bool)
{
    // RVA 0xF23C0
    RemoveAllItems();
    return 1;
}

int MapSellList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    // RVA 0xF2040
    MapSellItem* item = m_items[itemIdx].m_item;
    BoundsBase<float> const itemBounds = item->GetBounds();
    bounds.x0 = 0.0f;
    bounds.y0 = 0.0f;
    bounds.width = itemBounds.width;
    bounds.height = itemBounds.height;
    return 1;
}

int MapSellList::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    // RVA 0xF20B0
    MapSellItem* item = m_items[itemIdx].m_item;
    BoundsBase<float> bounds = item->GetBounds();
    bounds.y0 = (di.m_clientRect.y0 - di.m_originalRect.y0) + org.y;
    item->SetBounds(bounds, true);
    return 1;
}
