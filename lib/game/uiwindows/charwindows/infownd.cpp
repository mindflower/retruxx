#include "infownd.h"

#include <cstring>

#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <renderer/i_renderer.h>
#include <ui/frame.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>

RT_CLASS_EXPORTS_BEGIN(InfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(InfoWnd);

InfoWnd::InfoItem::AuxInfo InfoWnd::InfoItem::m_aif;
InfoWnd::AuxInfo InfoWnd::m_aif0;

namespace
{
    // The layout is laid out against a fixed 1024x768 design resolution and then
    // scaled to the real viewport.
    float const DESIGN_W = 1024.0f;
    float const DESIGN_H = 768.0f;
}  // namespace

// ===========================================================================
//  InfoWnd::InfoItem::AuxInfo
// ===========================================================================

InfoWnd::InfoItem::AuxInfo::AuxInfo()
{
    // RVA 0x44E230
    m_defIcoSize = {40.0f, 40.0f};
    m_space = 10.0f;
    m_strDefaultColor = help::Color2Str(0xFF404040u);
    m_strGreenColor = help::Color2Str(0xFF23961Bu);
    m_strRedColor = help::Color2Str(0xFF904040u);
    m_maxStringW = 400.0f;
}

InfoWnd::InfoItem::AuxInfo::AuxInfo(InfoWnd::InfoItem::AuxInfo const& rhs) :
    m_defIcoSize(rhs.m_defIcoSize),
    m_space(rhs.m_space),
    m_strDefaultColor(rhs.m_strDefaultColor),
    m_strGreenColor(rhs.m_strGreenColor),
    m_strRedColor(rhs.m_strRedColor),
    m_maxStringW(rhs.m_maxStringW)
{
}

// ===========================================================================
//  InfoWnd::InfoItem
// ===========================================================================

InfoWnd::InfoItem::InfoItem(int flags, CStr const& string, m3d::rend::TexHandle ico, PointBase<float> icoSize) :
    m_string(string),
    m_ico(ico),
    m_flags(flags),
    m_id(-1)
{
    // RVA 0x44F0C0 - an icon size of (-1, -1) means "use the default".
    if (m_ico.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_ico);
    }
    m_icoSize = (icoSize.x == -1.0f && icoSize.y == -1.0f) ? m_aif.m_defIcoSize : icoSize;
    m_flags = flags;
    CalculateStringSize();
}

InfoWnd::InfoItem::InfoItem(InfoWnd::InfoItem const& rhs) : m_ico()
{
    // RVA 0x44F180 - starts from an empty string and an invalid icon, then lets
    // operator= do the copying.
    *this = rhs;
}

InfoWnd::InfoItem::~InfoItem()
{
    // RVA 0x44F200
    if (m_ico.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_ico);
    }
}

InfoWnd::InfoItem& InfoWnd::InfoItem::operator=(InfoWnd::InfoItem const& rhs)
{
    // RVA 0x44F1B0 - note the already-measured m_auxStringSize is copied across
    // rather than recomputed, and the incoming icon is referenced without
    // releasing whatever this item held before.
    m_id = rhs.m_id;
    m_string = rhs.m_string;
    m_icoSize = rhs.m_icoSize;
    m_flags = rhs.m_flags;
    m_auxStringSize = rhs.m_auxStringSize;
    m_ico = rhs.m_ico;
    if (m_ico.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_ico);
    }
    return *this;
}

void InfoWnd::InfoItem::SetString(CStr const string)
{
    // RVA 0x44F610
    m_string = string;
    CalculateStringSize();
}

void InfoWnd::InfoItem::SetIco(m3d::rend::TexHandle icoId)
{
    // RVA 0x44F640
    if (m_ico.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_ico);
    }
    m_ico = icoId;
    if (m_ico.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_ico);
    }
}

void InfoWnd::InfoItem::SetIcoSize(PointBase<float> const& sz)
{
    // RVA 0x44F690
    m_icoSize = (sz.x == -1.0f && sz.y == -1.0f) ? m_aif.m_defIcoSize : sz;
}

void InfoWnd::InfoItem::SetFlags(int flags)
{
    // RVA 0x44F6E0
    m_flags = flags;
    CalculateStringSize();
}

CStr const& InfoWnd::InfoItem::GetString() const
{
    return m_string;
}

m3d::rend::TexHandle InfoWnd::InfoItem::GetIco() const
{
    return m_ico;
}

PointBase<float> const& InfoWnd::InfoItem::GetIcoSize() const
{
    return m_icoSize;
}

int InfoWnd::InfoItem::GetFlags() const
{
    return m_flags;
}

int InfoWnd::InfoItem::GetId() const
{
    return m_id;
}

void InfoWnd::InfoItem::SetId(int id)
{
    m_id = id;
}

void InfoWnd::InfoItem::CalculateStringSize()
{
    // RVA 0x44F2C0
    if (m_string.empty())
    {
        m_auxStringSize = {};
        return;
    }

    // NOTE: the font id here is ~(m_flags >> 1) with no mask, unlike Draw() which
    // masks it to 0/1. For every flag combination it therefore comes out negative
    // - an id GfxServer rejects - so measuring silently keeps whatever font was
    // current while drawing switches to the big/normal one. Preserved as shipped.
    m_auxStringSize =
        GetGfxServer()->MeasureText(m_string, ~(m_flags >> 1), m3d::TW_WORD_WRAP, m_aif.m_maxStringW);
}

PointBase<float> InfoWnd::InfoItem::GetDrawSize() const
{
    // RVA 0x44F240 - icon and string sit side by side, so the row is as wide as
    // both together and as tall as the taller of the two.
    float w = 0.0f;
    float h = 0.0f;

    if (m_ico.IsValid())
    {
        w = m_icoSize.x + m_aif.m_space;
        h = m_icoSize.y;
    }

    if (!m_string.empty())
    {
        w += m_auxStringSize.x;
        float const icoH = m_ico.IsValid() ? m_icoSize.y : 0.0f;
        h = m_auxStringSize.y > icoH ? m_auxStringSize.y : icoH;
    }

    return {w, h};
}

void InfoWnd::InfoItem::Draw(m3d::ui::DrawInfo const& di, BoundsBase<float> const& rc) const
{
    // RVA 0x44F330
    if (m_ico.IsValid())
    {
        float x0;
        if ((m_flags & INFOFLAG_CENTER) != 0)
        {
            float const stringW = m_string.empty() ? 0.0f : m_auxStringSize.x + m_aif.m_space;
            x0 = (rc.width - (m_icoSize.x + stringW)) * 0.5f + rc.x0;
        }
        else
        {
            x0 = rc.x0;
        }

        BoundsBase<float> icoB;
        icoB.x0 = x0;
        icoB.y0 = (rc.height - m_icoSize.y) * 0.5f + rc.y0;
        icoB.width = m_icoSize.x;
        icoB.height = m_icoSize.y;
        m3d::ui::Wnd::m_gfx->AddImagedRect(di, icoB, 0, m_ico);
    }

    if (m_string.empty())
    {
        return;
    }

    float const icoW = m_ico.IsValid() ? m_icoSize.x + m_aif.m_space : 0.0f;
    PointBase<float> at;
    if ((m_flags & INFOFLAG_CENTER) != 0)
    {
        at.x = (rc.width - (m_auxStringSize.x + icoW)) * 0.5f + rc.x0;
    }
    else
    {
        at.x = rc.x0 + icoW;
    }
    at.y = (rc.height - m_auxStringSize.y) * 0.5f + rc.y0;

    CStr strColor = m_aif.m_strDefaultColor;
    if ((m_flags & INFOFLAG_SELECTED_COLOR) != 0)
    {
        strColor = m_aif.m_strGreenColor;
    }
    else if ((m_flags & INFOFLAG_WARN_COLOR) != 0)
    {
        strColor = m_aif.m_strRedColor;
    }

    // The text is clipped to at most m_maxStringW from where it starts.
    m3d::ui::DrawInfo fakeDi(di);
    float const maxRight = m_aif.m_maxStringW + at.x;
    float const width = fakeDi.m_clientRect.width > maxRight ? maxRight : fakeDi.m_clientRect.width;
    fakeDi.m_clientClippedRect.width = width;
    fakeDi.m_clientRect.width = width;

    m3d::ui::Wnd::m_gfx->AddText(
        fakeDi, at, strColor + m_string, (~(m_flags >> 1)) & 1, m3d::TW_WORD_WRAP, m3d::TextFormatFlags(1));
}

// ===========================================================================
//  InfoWnd::AuxInfo
// ===========================================================================

InfoWnd::AuxInfo::AuxInfo()
{
    // RVA 0x44E200
    m_edgeSpace = 15.0f;
    m_xSpace = 10.0f;
    m_ySpace = 10.0f;
    m_titleYSpace = 15.0f;
}

// ===========================================================================
//  InfoWnd
// ===========================================================================

InfoWnd::InfoWnd() : m_numberOfColumns(1)
{
}

InfoWnd::InfoWnd(InfoWnd const&) : InfoWnd()
{
    // NOTE: as with the other windows here, the shipped copy ctor only builds the
    // base and empties m_items; nothing is copied from rhs.
}

InfoWnd::~InfoWnd() = default;

m3d::Object* InfoWnd::Clone()
{
    // RVA 0x44E140
    return new InfoWnd(*this);
}

m3d::Object* InfoWnd::CreateObject()
{
    return new InfoWnd;
}

m3d::Class* InfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* InfoWnd::GetClass() const
{
    return RT_CLASS_LOCAL(InfoWnd);
}

void InfoWnd::SetNumberOfColumns(int number)
{
    // RVA 0x44F050
    m_numberOfColumns = number;
}

int InfoWnd::GetNumberOfColumns() const
{
    return m_numberOfColumns;
}

int InfoWnd::GameDataClear(bool)
{
    // RVA 0x44F060
    m_items.clear();
    return 1;
}

InfoWnd::InfoItem* InfoWnd::GetItemById(int id)
{
    // RVA 0x44E5B0
    for (auto& item : m_items)
    {
        if (item.m_id == id)
        {
            return &item;
        }
    }
    return nullptr;
}

int InfoWnd::AddItemById(int id, InfoWnd::InfoItem const& item)
{
    // RVA 0x44E540 - ids are unique. The id is stamped on the copy after
    // operator= has run, because operator= would otherwise carry rhs's id over.
    if (id == -1 || GetItemById(id))
    {
        return 0;
    }

    InfoItem itemToAdd(0, CStr(), m3d::rend::TexHandle(), {});
    itemToAdd = item;
    itemToAdd.SetId(id);
    m_items.push_back(itemToAdd);
    return 1;
}

void InfoWnd::RecalcLayot(
    m3d::ui::DrawInfo const& srcDi,
    m3d::ui::DrawInfo& newDi,
    std::vector<InfoWnd::RowInfo, std::allocator<InfoWnd::RowInfo>>& rowsInfo,
    std::vector<InfoWnd::ColumnInfo, std::allocator<InfoWnd::ColumnInfo>>& columnsInfo,
    std::map<int, PointBase<int>, std::less<int>, std::allocator<std::pair<int const, PointBase<int>>>>&
        itemPositions)
{
    // RVA 0x44E850 - flows the items into a grid, sizes the window to fit and
    // parks it next to the mouse cursor without letting it leave the screen.
    newDi = srcDi;
    rowsInfo.clear();
    columnsInfo.clear();
    itemPositions.clear();

    // How many columns the first row actually manages to fill.
    int numColumns = 0;
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        if ((m_items[i].m_flags & INFOFLAG_FULL_ROW) != 0)
        {
            numColumns = 0;
        }
        else
        {
            ++numColumns;
        }
        if (numColumns == m_numberOfColumns)
        {
            break;
        }
    }
    if (!numColumns)
    {
        numColumns = 1;
    }

    float const edgeSpace2 = m_aif0.m_edgeSpace * 2.0f;
    float w = edgeSpace2;
    float h = edgeSpace2;
    float maxColumnW = 0.0f;
    float maxFullRowW = 0.0f;
    float maxRowH = 0.0f;
    int column = 0;
    int row = 0;

    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        InfoItem const& item = m_items[i];

        float itemW = 0.0f;
        float itemH = 0.0f;
        if (item.m_ico.IsValid())
        {
            itemW = item.m_icoSize.x + InfoItem::m_aif.m_space;
            itemH = item.m_icoSize.y;
        }
        if (!item.m_string.empty())
        {
            itemW += item.m_auxStringSize.x;
            float const icoH = item.m_ico.IsValid() ? item.m_icoSize.y : 0.0f;
            itemH = item.m_auxStringSize.y > icoH ? item.m_auxStringSize.y : icoH;
        }

        itemPositions.insert(ItemPositionPair(item.m_id, PointBase<int>{column, row}));

        bool endOfRow = false;
        if ((item.m_flags & INFOFLAG_FULL_ROW) != 0)
        {
            if (itemW > maxFullRowW)
            {
                maxFullRowW = itemW;
            }
            endOfRow = true;
        }
        else
        {
            if (itemW > maxColumnW)
            {
                maxColumnW = itemW;
            }
            // The last item, or the last column of a row, closes it.
            if (i == static_cast<int>(m_items.size()) - 1 || m_numberOfColumns == 1 ||
                (column && !(column % (m_numberOfColumns - 1))))
            {
                endOfRow = true;
            }
        }

        if (itemH > maxRowH)
        {
            maxRowH = itemH;
        }

        if (endOfRow)
        {
            RowInfo rInfo;
            rInfo.m_y0 = h - m_aif0.m_edgeSpace;
            rInfo.m_h = maxRowH;
            rowsInfo.push_back(rInfo);

            h += maxRowH;
            if (i < static_cast<int>(m_items.size()) - 1)
            {
                h += ((item.m_flags & INFOFLAG_BIG_SPACE) != 0) ? m_aif0.m_titleYSpace : m_aif0.m_ySpace;
            }
            column = 0;
            ++row;
            maxRowH = 0.0f;
        }
        else
        {
            ++column;
        }
    }

    float const gridW =
        static_cast<float>(numColumns - 1) * m_aif0.m_xSpace + static_cast<float>(numColumns) * maxColumnW;
    w += (maxFullRowW > gridW) ? maxFullRowW : gridW;

    for (int i = 0; i < numColumns; ++i)
    {
        ColumnInfo cInfo;
        cInfo.m_x0 = (m_aif0.m_xSpace + maxColumnW) * static_cast<float>(i) + m_aif0.m_edgeSpace;
        cInfo.m_w = maxColumnW;
        columnsInfo.push_back(cInfo);
    }

    if (w <= 0.0f || h <= 0.0f)
    {
        return;
    }

    // Grow by the pane frame and the client edges, then place the window.
    float paneBar = 0.0f;
    if ((m_paneFlags & 2) != 0 && (m_style & 0x40) == 0)
    {
        if (auto const* pane = m3d::ui::Wnd::m_gfx->GetPane(m_paneName))
        {
            if (auto const* frame = pane->m_frame[0])
            {
                paneBar = static_cast<float>(frame->m_barUsedWidth);
            }
        }
    }

    float const paneBar2 = paneBar * 2.0f;
    BoundsBase<float> b;
    b.x0 = 0.0f;
    b.y0 = 0.0f;
    b.width = m_clientEdges[2] + m_clientEdges[0] + paneBar2 + w;
    b.height = m_clientEdges[3] + m_clientEdges[1] + paneBar2 + h;
    SetBounds(b, true);

    float const x = static_cast<float>(M3D_APP->GetMouseX()) * DESIGN_W /
                    static_cast<float>(M3D_RENDERER->GetViewport().m_width);
    float const y = static_cast<float>(M3D_APP->GetMouseY()) * DESIGN_H /
                    static_cast<float>(M3D_RENDERER->GetViewport().m_height);
    float const cursorH = GetStation()->GetCurrentCursor().m_sz.y;

    float const x0 = (x + b.width >= DESIGN_W) ? DESIGN_W - b.width : x;
    float const y0 = (y + cursorH + b.height >= DESIGN_H) ? y : y + cursorH;

    m_bounds.x0 = x0;
    m_bounds.y0 = y0;
    newDi.m_originalRect = m_bounds;
    newDi.m_clippedRect = m_bounds;

    BoundsBase<float> const clientBounds = GetClientBounds();
    newDi.m_clientRect.x0 = m_bounds.x0 + clientBounds.x0;
    newDi.m_clientRect.y0 = m_bounds.y0 + clientBounds.y0;
    newDi.m_clientRect.width = clientBounds.width;
    newDi.m_clientRect.height = clientBounds.height;
    newDi.m_clientClippedRect = newDi.m_clientRect;
}

int InfoWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x44E620
    RowInfoVector rowsInfo;
    ColumnInfoVector columnsInfo;
    ItemPositionMap itemPositions;

    m3d::ui::DrawInfo newDi(di);
    RecalcLayot(di, newDi, rowsInfo, columnsInfo, itemPositions);

    unsigned const clr = ((m_style & 2) != 0 || (m_style & 0x80000) != 0) ? 3u : m_curClr;
    DrawNonClient(newDi, clr);

    for (auto const& [id, pos] : itemPositions)
    {
        InfoItem const* item = GetItemById(id);
        if (!item)
        {
            continue;
        }

        BoundsBase<float> rc;
        rc.x0 = columnsInfo[pos.x].m_x0;
        rc.y0 = rowsInfo[pos.y].m_y0;
        rc.width = ((item->m_flags & INFOFLAG_FULL_ROW) != 0)
                       ? m_bounds.width - m_aif0.m_edgeSpace * 2.0f
                       : columnsInfo[pos.x].m_w;
        rc.height = rowsInfo[pos.y].m_h;
        item->Draw(newDi, rc);
    }
    return 1;
}
