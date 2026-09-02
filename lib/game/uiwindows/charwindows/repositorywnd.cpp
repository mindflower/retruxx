#include "repositorywnd.h"

#include "iteminfownd.h"
#include "videownd.h"
#include "warewnd.h"

#include <algorithm>
#include <cmath>

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <m3dapp.h>
#include <game/m3dgame.h>
#include <game/uimisc/guihelper.h>

#include <ui/frame.h>
#include <ui/image.h>
#include <ui/scroll.h>
#include <ui/ui_srv.h>

#include <renderer/i_renderer.h>

#include <server/geomrepository.h>
#include <server/geomrepositoryitem.h>
#include <server/resourcemanager.h>
#include <server/objects/gadget.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/vehicle.h>
#include <server/objects/workshop.h>
#include <server/objects/base/obj.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/physicbodies/vehiclepart.h>

RT_CLASS_EXPORTS_BEGIN(RepositoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepositoryWnd);

namespace
{
    // Axis-aligned intersection of two float rects; width/height come back 0 when
    // they do not overlap.
    BoundsBase<float> IntersectRectsF(BoundsBase<float> const& a, BoundsBase<float> const& b)
    {
        BoundsBase<float> res{0.0f, 0.0f, 0.0f, 0.0f};
        if (b.x0 > a.x0 + a.width || a.x0 > b.x0 + b.width || b.y0 > a.y0 + a.height || a.y0 > b.y0 + b.height)
        {
            return res;
        }
        float x0 = a.x0 > b.x0 ? a.x0 : b.x0;
        float right = (a.x0 + a.width) < (b.x0 + b.width) ? (a.x0 + a.width) : (b.x0 + b.width);
        float y0 = a.y0 > b.y0 ? a.y0 : b.y0;
        float bottom = (a.y0 + a.height) < (b.y0 + b.height) ? (a.y0 + a.height) : (b.y0 + b.height);
        res.x0 = x0;
        res.y0 = y0;
        res.width = right - x0;
        res.height = bottom - y0;
        return res;
    }
}  // namespace

// ---------------------------------------------------------------------------
// nested helper structs
// ---------------------------------------------------------------------------

RepositoryWnd::CellBunch::CellBunch()
{
    Reset();
}

RepositoryWnd::CellBunch::CellBunch(RepositoryWnd::CellBunch const& o) : m_bounds(o.m_bounds), m_state(o.m_state)
{
}

RepositoryWnd::CellBunch::~CellBunch()
{
}

void RepositoryWnd::CellBunch::Reset()
{
    m_bounds.x0 = 0;
    m_bounds.y0 = 0;
    m_bounds.width = 0;
    m_bounds.height = 0;
    m_state = SLOTSTATE_UNSELECTED;
}

RepositoryWnd::TwinkleInfo::TwinkleInfo(int itemObjId)
{
    m_itemObjId = itemObjId;
    m_startTime = 0;
}

void RepositoryWnd::TwinkleInfo::Invalidate()
{
    m_itemObjId = -1;
    m_startTime = 0;
}

bool RepositoryWnd::TwinkleInfo::IsValid() const
{
    return m_itemObjId != -1;
}

RepositoryWnd::AuxInfo::AuxInfo()
{
    m_cellTexNames[0] = "Cell";
    m_cellTexNames[1] = "CellSelected";
    m_cellTexNames[2] = "CellSuitable";
    m_cellTexNames[3] = "CellUnsuitable";
    m_cellTexNames[4] = "CellSuitable";
    // m_cellTexNames[5] intentionally left empty
    m_unsuitableBgName = "BgUnsuitable";
    m_twinklePeriod = 300;
    m_numTwinkles = 2;
}

RepositoryWnd::AuxInfo::AuxInfo(RepositoryWnd::AuxInfo const& o)
{
    for (int i = 0; i < 6; ++i)
    {
        m_cellTexNames[i] = o.m_cellTexNames[i];
    }
    m_unsuitableBgName = o.m_unsuitableBgName;
    m_twinklePeriod = o.m_twinklePeriod;
    m_numTwinkles = o.m_numTwinkles;
}

// ---------------------------------------------------------------------------
// lifetime / rtti
// ---------------------------------------------------------------------------

RepositoryWnd::RepositoryWnd()
{
    m_isInited = true;
    m_geomSize = {1, 1};
    m_cellSize = {32.0f, 32.0f};
    m_maxBounds = {32.0f, 32.0f, 0.0f, 0.0f};
    m_repository = nullptr;
    m_type = REPOSITORYTYPE_PLAYER;
    m_isSpecialized = false;
    m_defaultItemStyle = 0;
    m_prevOverroledSlotId = -1;
    m_curBunch.Reset();
    for (auto& tex : m_cellTextures)
    {
        tex.SetInvalid();
    }
    m_unsuitableBg.SetInvalid();
}

RepositoryWnd::RepositoryWnd(RepositoryWnd const&) : RepositoryWnd()
{
    // NOTE: the shipped copy ctor default-constructs the base and initialises only
    // a subset of the members (leaving several POD fields indeterminate). We
    // delegate to the default ctor so a Clone()d instance is always fully valid.
}

RepositoryWnd::~RepositoryWnd()
{
    RemoveAllItems();
    m_repository = nullptr;
    m_twinkleItems.clear();
    for (auto& tex : m_cellTextures)
    {
        if (tex.IsValid())
        {
            M3D_RENDERER->ReleaseTexture(tex);
        }
    }
    if (m_unsuitableBg.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_unsuitableBg);
    }
    // m_itemStyles / m_compatibleResourceIds / m_aif / m_slots free their own storage
}

m3d::Object* RepositoryWnd::CreateObject()
{
    return new RepositoryWnd();
}

m3d::Object* RepositoryWnd::Clone()
{
    return new RepositoryWnd(*this);
}

m3d::Class* RepositoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

m3d::Class* RepositoryWnd::GetClass() const
{
    return RT_CLASS_LOCAL(RepositoryWnd);
}

// ---------------------------------------------------------------------------
// trivial accessors
// ---------------------------------------------------------------------------

ai::GeomRepository* RepositoryWnd::GetRepository() const
{
    return m_repository;
}

PointBase<int> RepositoryWnd::GetGeomSize() const
{
    return m_geomSize;
}

PointBase<float> RepositoryWnd::GetCellSize() const
{
    return m_cellSize;
}

BoundsBase<float> const& RepositoryWnd::GetMaxBounds() const
{
    return m_maxBounds;
}

RepositoryWnd::RepositoryType RepositoryWnd::GetRepositoryType() const
{
    return m_type;
}

bool RepositoryWnd::IsValid() const
{
    return m_isInited && m_repository != nullptr;
}

void RepositoryWnd::SetItemToTwinkle(int itemObjId)
{
    m_twinkleItems.push_back(RepositoryWnd::TwinkleInfo(itemObjId));
}

// ---------------------------------------------------------------------------
// geometry <-> cell coordinate conversions
// ---------------------------------------------------------------------------

BoundsBase<float> RepositoryWnd::GeomToWndBounds(BoundsBase<int> const& geomB)
{
    float sy = m_scrollVWnd ? m_scrollVWnd->GetCurPos() : 0.0f;
    float sx = m_scrollHWnd ? m_scrollHWnd->GetCurPos() : 0.0f;

    float left = static_cast<float>(geomB.x0) * m_cellSize.x;
    float top = static_cast<float>(geomB.y0) * m_cellSize.y;

    BoundsBase<float> res;
    res.x0 = m_clientEdges[0] + (left - sx);
    res.y0 = m_clientEdges[1] + (top - sy);
    res.width = static_cast<float>(geomB.x0 + geomB.width) * m_cellSize.x - left;
    res.height = static_cast<float>(geomB.y0 + geomB.height) * m_cellSize.y - top;
    return res;
}

PointBase<int> RepositoryWnd::WndToGeomPt(PointBase<float> const& wndPt)
{
    float sy = m_scrollVWnd ? m_scrollVWnd->GetCurPos() : 0.0f;
    float sx = m_scrollHWnd ? m_scrollHWnd->GetCurPos() : 0.0f;

    PointBase<int> res;
    res.x = static_cast<int>((wndPt.x - m_clientEdges[0] + sx) / m_cellSize.x);
    res.y = static_cast<int>((wndPt.y - m_clientEdges[1] + sy) / m_cellSize.y);
    return res;
}

BoundsBase<int> RepositoryWnd::WndToGeomBounds(BoundsBase<float> const& wndB)
{
    float sy = m_scrollVWnd ? m_scrollVWnd->GetCurPos() : 0.0f;
    float sx = m_scrollHWnd ? m_scrollHWnd->GetCurPos() : 0.0f;

    int lx = static_cast<int>((wndB.x0 - m_clientEdges[0] + sx) / m_cellSize.x);
    int ly = static_cast<int>((wndB.y0 - m_clientEdges[1] + sy) / m_cellSize.y);
    int rx = static_cast<int>((wndB.x0 + wndB.width - m_clientEdges[0] + sx) / m_cellSize.x);
    int ry = static_cast<int>((wndB.y0 + wndB.height - m_clientEdges[1] + sy) / m_cellSize.y);

    return BoundsBase<int>{lx, ly, rx - lx, ry - ly};
}

// ---------------------------------------------------------------------------
// window / cell sizing
// ---------------------------------------------------------------------------

void RepositoryWnd::SetGeomSize(PointBase<int> const& geomSize)
{
    if (!m_isInited || !m_repository)
    {
        return;
    }
    m_geomSize = geomSize;
    AdjustWndToCells();
}

void RepositoryWnd::SetCellSize(PointBase<float> const& size)
{
    m_cellSize = size;
    AdjustWndToCells();
}

void RepositoryWnd::SetMaxBounds(BoundsBase<float> const& maxBounds)
{
    m_maxBounds = maxBounds;
    AdjustWndToCells();
}

void RepositoryWnd::SetBounds(BoundsBase<float> const& bounds, bool bUpdateBaseOrigin)
{
    m_bounds = bounds;
    if (bUpdateBaseOrigin)
    {
        m_baseOrigin.x = m_bounds.x0;
        m_baseOrigin.y = m_bounds.y0;
    }
    AdjustCellsToWnd();
    UpdateScrollWnd();
}

void RepositoryWnd::AdjustCellsToWnd()
{
    if (!m_isInited || !m_repository || !m_geomSize.x)
    {
        return;
    }
    float scrollW = 0.0f;
    if (m_scrollVWnd)
    {
        scrollW = m_scrollVWnd->GetBounds().width + m_clientEdges[2];
    }
    float v = ((m_bounds.width - m_clientEdges[0] * 2.0f) - scrollW) / static_cast<float>(m_geomSize.x);
    m_cellSize.x = v;
    m_cellSize.y = v;
}

void RepositoryWnd::AdjustWndToCells()
{
    if (!m_isInited || !m_repository)
    {
        return;
    }

    float cellsW = static_cast<float>(m_geomSize.x) * m_cellSize.x;
    float cellsH = static_cast<float>(m_geomSize.y) * m_cellSize.y;
    bool fits = true;

    float scrollWidth = m_scrollVWnd ? m_scrollVWnd->GetBounds().width : 0.0f;
    float scrollEdge = m_scrollVWnd ? m_clientEdges[2] : 0.0f;
    float horzMargin = m_clientEdges[0] * 2.0f + scrollEdge;
    float vertMargin = m_clientEdges[3] + m_clientEdges[1];

    float wantW = cellsW + horzMargin + scrollWidth;
    if (m_maxBounds.width <= wantW)
    {
        m_bounds.width = m_maxBounds.width;
        m_bounds.x0 = m_maxBounds.x0;
        fits = false;
    }
    else
    {
        m_bounds.x0 = (m_maxBounds.width - wantW) * 0.5f + m_maxBounds.x0;
        m_bounds.width = wantW;
    }

    float wantH = cellsH + vertMargin;
    if (m_maxBounds.height < wantH)
    {
        m_bounds.height = m_maxBounds.height;
        m_bounds.y0 = m_maxBounds.y0;
    }
    else
    {
        m_bounds.y0 = (m_maxBounds.height - wantH) * 0.5f + m_maxBounds.y0;
        m_bounds.height = wantH;
    }

    if (!fits)
    {
        AdjustCellsToWnd();
    }
    UpdateScrollWnd();
}

// ---------------------------------------------------------------------------
// scroll window
// ---------------------------------------------------------------------------

int RepositoryWnd::CreateScrollWnd()
{
    m_scrollVWnd = static_cast<m3d::ui::ScrollWnd*>(M3D_KERNEL->New("ScrollWnd"));
    if (!m_scrollVWnd)
    {
        return 0;
    }

    float scrollSz = 0.0f;
    CStr scrollPaneName = m_scrollPaneName;
    if (auto* sp = GetGfxServer()->GetScrollPane(scrollPaneName))
    {
        scrollSz = sp->GetWidth();
    }

    BoundsBase<float> rect{0.0f, 0.0f, scrollSz, 0.0f};
    m_scrollVWnd->Create(rect, 1);
    m_scrollVWnd->SetPane(scrollPaneName);

    float right = m_bounds.width - m_clientEdges[0];
    rect.x0 = right - scrollSz;
    rect.y0 = m_clientEdges[1];
    rect.width = scrollSz;
    rect.height = (m_bounds.height - m_clientEdges[3]) - m_clientEdges[1];
    m_scrollVWnd->SetBounds(rect, true);

    m_scrollVWnd->SetScrollRect(0.0f, ((m_bounds.height - m_clientEdges[1]) - m_clientEdges[3]) * 2.0f);
    m_scrollVWnd->SetCurPos(0.0f);
    AddChild(m_scrollVWnd);
    return 1;
}

void RepositoryWnd::UpdateScrollWnd()
{
    if (!m_scrollVWnd)
    {
        return;
    }

    BoundsBase<float> scrollB = m_scrollVWnd->GetBounds();
    float ce0 = m_clientEdges[0];
    float ce1 = m_clientEdges[1];
    float ce3 = m_clientEdges[3];

    BoundsBase<float> place;
    place.x0 = m_bounds.width - scrollB.width - ce0;
    place.y0 = ce1;
    place.width = scrollB.width;
    place.height = (m_bounds.height - ce3) - ce1;
    m_scrollVWnd->SetBounds(place, true);

    float contentH = static_cast<float>(m_geomSize.y) * m_cellSize.y;
    float clientH = (m_bounds.height - ce1) - ce3;
    float oldScrollMax = m_scrollVWnd->GetMaxPos();

    if ((contentH - clientH) <= clientH * 2.0f)
    {
        m_scrollVWnd->SetScrollRect(0.0f, contentH);
    }
    else
    {
        GetMaxSlotY();  // NOTE: the original calls this here; its result is discarded
        float ext2 = clientH * 2.0f;
        float ext3 = ext2 + clientH;
        m_scrollVWnd->SetScrollRect(0.0f, ext2 > ext3 ? ext2 : ext3);
    }

    float newPos = 0.0f;
    if (m_scrollVWnd->GetMaxPos() > 0.001f)
    {
        newPos = (m_scrollVWnd->GetCurPos() * oldScrollMax) / m_scrollVWnd->GetMaxPos();
    }
    m_scrollVWnd->SetCurPos(newPos);
    m_scrollVWnd->EnableWindow(m_scrollVWnd->GetMaxPos() > 0.001f);
}

void RepositoryWnd::ScrollSlots()
{
    for (auto* slot : m_slots)
    {
        if (!slot)
        {
            continue;
        }
        BoundsBase<float> wndB = GeomToWndBounds(slot->GetItem().GetBounds());
        slot->SetBounds(wndB, true);
    }
}

float RepositoryWnd::GetMaxSlotY()
{
    int maxBottom = 0;
    for (auto* slot : m_slots)
    {
        if (!slot)
        {
            continue;
        }
        BoundsBase<int> b = slot->GetItem().GetBounds();
        if (b.y0 + b.height >= maxBottom)
        {
            maxBottom = b.y0 + b.height;
        }
    }
    return static_cast<float>(maxBottom);
}

// ---------------------------------------------------------------------------
// items / slots
// ---------------------------------------------------------------------------

int RepositoryWnd::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    if (!patternWnd)
    {
        return 0;
    }

    if (!m3d::ui::Wnd::Create(
            patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        return 0;
    }

    m_style = patternWnd->GetStyle();
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    m_textWrap = patternWnd->GetWrapMode();
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

    m_maxBounds = m_bounds;
    m_style = 512;

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        return 0;
    }
    parent->AddChild(this);
    parent->MoveChildToFirstPosition(this);
    if (deleteSrc)
    {
        auto* src = const_cast<m3d::ui::Wnd*>(patternWnd);
        parent->RemoveChild(src);
        src->DecRef();
    }

    for (int i = 0; i < 6; ++i)
    {
        m_cellTextures[i] = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_cellTexNames[i], 0);
        if (m_cellTextures[i].IsValid())
        {
            M3D_RENDERER->ReferenceTexture(m_cellTextures[i]);
        }
    }
    m_unsuitableBg = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_unsuitableBgName, 0);
    if (m_unsuitableBg.IsValid())
    {
        M3D_RENDERER->ReferenceTexture(m_unsuitableBg);
    }

    CreateScrollWnd();
    m_isInited = true;
    return 1;
}

int RepositoryWnd::CreateItems()
{
    if (!m_isInited || !m_repository)
    {
        return 0;
    }

    int res = 1;
    if (!m_slots.empty())
    {
        RemoveAllItems();
    }

    unsigned int numItems = m_repository->GetNumItems();
    for (unsigned int i = 0; i < numItems; ++i)
    {
        auto* slot = new GeomSlot();
        if (!slot)
        {
            res = 0;
            continue;
        }
        AddChild(slot);
        ai::GeomRepositoryItem item = m_repository->GetItem(static_cast<int>(i));
        slot->SetItem(item);
        slot->m_gsStyle = GetItemDrawStyleByResourceId(item.m_resourceId);
        m_slots.push_back(slot);
        numItems = m_repository->GetNumItems();
    }

    UpdateScrollWnd();

    // If the cursor is already hovering this window, refresh the hover state so a
    // freshly created item shows its info panel straight away.
    PointBase<float> mouseScreen{static_cast<float>(M3D_APP->GetMouseX()), static_cast<float>(M3D_APP->GetMouseY())};
    M3D_RENDERER->AbsToRel(mouseScreen.x, mouseScreen.y);
    if (auto* parentWnd = RT_DYNCAST(GetParent(), m3d::ui::Wnd))
    {
        BoundsBase<float> b = GetBounds();
        PointBase<float> tl = parentWnd->ToScreen(PointBase<float>{b.x0, b.y0});
        if (mouseScreen.x >= tl.x && mouseScreen.x < tl.x + b.width && mouseScreen.y >= tl.y &&
            mouseScreen.y < tl.y + b.height)
        {
            PointBase<float> origin = ToScreen(PointBase<float>{0.0f, 0.0f});
            OnUpdateWhileNoDrag(PointBase<float>{mouseScreen.x - origin.x, mouseScreen.y - origin.y});
        }
    }

    UpdateSlotsStylesCompatibleWithVehicle();
    UpdateSlotsStylesTooRich();
    AddInfoToEncyclopaedia();
    return res;
}

void RepositoryWnd::RemoveAllItems()
{
    for (auto* slot : m_slots)
    {
        if (DragDropItemsWnd::m_dragSlot && slot &&
            ai::operator==(DragDropItemsWnd::m_dragSlot->GetItem(), slot->GetItem()))
        {
            DragDropItemsWnd::RemoveDragSlot();
        }
        if (slot)
        {
            slot->DecRef();
        }
    }
    m_slots.clear();
    m_curBunch.Reset();
}

void RepositoryWnd::FullUpdate()
{
    RemoveAllItems();
    CreateItems();
}

int RepositoryWnd::AddItem(ai::GeomRepositoryItem const& item)
{
    if (!m_repository)
    {
        return 0;
    }

    ai::GeomRepositoryItem itemToAdd = item;

    if (m_type != REPOSITORYTYPE_SHOP)
    {
        if (DragDropItemsWnd::m_dragSlot && ai::operator==(DragDropItemsWnd::m_dragSlot->GetItem(), item))
        {
            PointBase<int> origin{m_curBunch.m_bounds.x0, m_curBunch.m_bounds.y0};
            return m_repository->AddThingToPlace(itemToAdd, origin) ? 1 : 0;
        }
        if (m_repository->AddThing(itemToAdd, 0))
        {
            SetItemToTwinkle(itemToAdd.GetObjId());
            PlayDropSound();
            return 1;
        }
        return 0;
    }

    // Shop repositories re-stock the repaired object.
    if (!m_repository->AddThing(itemToAdd, 0))
    {
        return 0;
    }
    help::RepairObj(ai::theObjects->GetEntityByObjId(itemToAdd.GetObjId()));
    m_twinkleItems.push_back(RepositoryWnd::TwinkleInfo(itemToAdd.GetObjId()));
    PlayDropSound();
    return 1;
}

int RepositoryWnd::GetItemDrawStyleByResourceId(int rcId) const
{
    for (auto const& kv : m_itemStyles)
    {
        if (ai::theResourceManager->bResourceIsKindOf(rcId, kv.first))
        {
            return kv.second;
        }
    }
    return m_defaultItemStyle;
}

void RepositoryWnd::SetItemDrawStyleByResourceId(int style, int rcId)
{
    m_itemStyles.insert(std::make_pair(rcId, style));

    for (auto* slot : m_slots)
    {
        if (!slot)
        {
            continue;
        }
        if (rcId == -1 || ai::theResourceManager->bResourceIsKindOf(slot->GetItem().m_resourceId, rcId))
        {
            slot->m_gsStyle = style;
        }
    }

    if (rcId == -1)
    {
        m_itemStyles.clear();
        m_defaultItemStyle = style;
    }
}

void RepositoryWnd::SetSpecialized(std::vector<int> const& compatibleRcIds)
{
    m_compatibleResourceIds = compatibleRcIds;
    m_isSpecialized = true;
}

bool RepositoryWnd::IsItemCompatibleByResourceType(ai::GeomRepositoryItem const& item) const
{
    if (!m_isSpecialized)
    {
        return true;
    }
    for (int rcId : m_compatibleResourceIds)
    {
        if (ai::theResourceManager->bResourceIsKindOf(item.m_resourceId, rcId))
        {
            return true;
        }
    }
    return false;
}

GeomSlot* RepositoryWnd::GetSlotByItemObjId(int itemObjId) const
{
    for (auto* slot : m_slots)
    {
        if (slot && slot->GetItem().GetObjId() == itemObjId)
        {
            return slot;
        }
    }
    return nullptr;
}

GeomSlot* RepositoryWnd::GetDraggingSlot() const
{
    if (!DragDropItemsWnd::m_dragSlot)
    {
        return nullptr;
    }
    if (!DragDropItemsWnd::m_dragSlot->IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
    {
        return nullptr;
    }
    if (DragDropItemsWnd::m_dragSlot->m_srcItemsWnd != this)
    {
        return nullptr;
    }
    for (auto* slot : m_slots)
    {
        if (slot && ai::operator==(slot->GetItem(), DragDropItemsWnd::m_dragSlot->GetItem()))
        {
            return slot;
        }
    }
    return nullptr;
}

ai::GeomRepositoryItem RepositoryWnd::GetItemFromOrigin(PointBase<float> const& wndPt)
{
    ai::GeomRepositoryItem item;
    if (!m_repository)
    {
        return item;
    }
    PointBase<int> geomPt = WndToGeomPt(wndPt);
    int slot = m_repository->GetSlotByPlace(geomPt);
    if (slot != -1)
    {
        item = m_repository->GetItem(slot);
    }
    return item;
}

// ---------------------------------------------------------------------------
// repository binding
// ---------------------------------------------------------------------------

int RepositoryWnd::SetRepository(ai::GeomRepository* repository, RepositoryWnd::RepositoryType type)
{
    if (!m_isInited)
    {
        return 0;
    }
    GameDataClear(false);
    m_type = type;
    m_repository = repository;
    if (!repository)
    {
        return 0;
    }

    PointBase<int> geomSize = repository->GetGeomSize();
    if (m_isInited && m_repository)
    {
        m_geomSize.x = geomSize.x;
        m_geomSize.y = geomSize.y;
        AdjustWndToCells();
    }

    int res = CreateItems();
    m_isSpecialized = false;
    return res;
}

int RepositoryWnd::GameDataClear(bool)
{
    RemoveAllItems();
    m_repository = nullptr;
    m_twinkleItems.clear();
    return 1;
}

int RepositoryWnd::GameDataUpdate(void* data, int dataType)
{
    if (!m_isInited || !m_repository)
    {
        return 0;
    }

    m3d::Object* station = static_cast<m3d::Object*>(M3D_APP);
    switch (dataType)
    {
    case 58:  // repository contents changed
        if (IsChildOf(station))
        {
            OnRepositoryChanged(data);
        }
        return 1;
    case 64:  // player vehicle changed
        if (IsChildOf(station))
        {
            OnPlayerVehicleChanged();
        }
        return 1;
    case 65:  // vehicle part changed
        if (IsChildOf(station))
        {
            OnVehiclePartChanged(data);
        }
        return 1;
    case 75:  // player money changed
        if (IsChildOf(station))
        {
            OnPlayerMoneyChanged();
        }
        return 1;
    case 89:  // new frame
        OnNewFrame();
        return 1;
    case 99:  // item quick-drop request
        if (data && IsChildOf(station))
        {
            ItemAcceptInfo info(*static_cast<ItemAcceptInfo const*>(data));
            UpdateOnItemQuickDrop(info);
        }
        return 1;
    default:
        DragDropItemsWnd::GameDataUpdate(data, dataType);
        return 1;
    }
}

// ---------------------------------------------------------------------------
// notifications from the game
// ---------------------------------------------------------------------------

void RepositoryWnd::OnRepositoryChanged(void* data)
{
    if (!data)
    {
        return;
    }
    auto* changed = reinterpret_cast<ai::GeomRepository* const*>(data)[13];
    if (changed == m_repository)
    {
        FullUpdate();
    }
}

void RepositoryWnd::OnPlayerVehicleChanged()
{
    UpdateSlotsStylesCompatibleWithVehicle();
}

void RepositoryWnd::OnPlayerMoneyChanged()
{
    UpdateSlotsStylesTooRich();
}

void RepositoryWnd::OnVehiclePartChanged(void* data)
{
    if (!data)
    {
        return;
    }
    int changedVehicleId = reinterpret_cast<int const*>(data)[13];
    int playerVehicleId = -1;
    if (ai::thePlayer)
    {
        if (auto* v = ai::thePlayer->GetVehicle())
        {
            playerVehicleId = v->GetId();
        }
    }
    if (changedVehicleId == playerVehicleId)
    {
        UpdateSlotsStylesCompatibleWithVehicle();
    }
}

void RepositoryWnd::OnNewFrame()
{
    for (auto& t : m_twinkleItems)
    {
        TwinkleItem(t);
    }
    m_twinkleItems.erase(
        std::remove_if(
            m_twinkleItems.begin(),
            m_twinkleItems.end(),
            [](RepositoryWnd::TwinkleInfo const& t)
            {
                return t.m_itemObjId == -1;
            }),
        m_twinkleItems.end());
}

void RepositoryWnd::TwinkleItem(RepositoryWnd::TwinkleInfo& twinkleInfo)
{
    if (twinkleInfo.m_itemObjId == -1)
    {
        return;
    }
    GeomSlot* slot = GetSlotByItemObjId(twinkleInfo.m_itemObjId);
    if (!slot)
    {
        return;
    }

    unsigned int now = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    if (!twinkleInfo.m_startTime)
    {
        twinkleInfo.m_startTime = now;
    }

    unsigned int period = m_aif.m_twinklePeriod;
    if (now < twinkleInfo.m_startTime + period * m_aif.m_numTwinkles)
    {
        unsigned int half = period / 2;
        bool evenPhase = half ? (((now - twinkleInfo.m_startTime) / half) % 2 == 0) : true;
        if (evenPhase)
        {
            if (slot->GetStyle() & 0x200)
            {
                slot->ShowWindow(false);
            }
        }
        else if ((slot->GetStyle() & 0x200) == 0)
        {
            slot->ShowWindow(true);
        }
    }
    else
    {
        slot->ShowWindow(true);
        twinkleInfo.m_itemObjId = -1;
        twinkleInfo.m_startTime = 0;
    }
}

void RepositoryWnd::SetSlotCompatibleWithVehicle(int slotIdx, bool bCompatible)
{
    if (slotIdx < 0 || slotIdx >= static_cast<int>(m_slots.size()))
    {
        return;
    }
    GeomSlot* slot = m_slots[slotIdx];
    if (!slot)
    {
        return;
    }
    if (bCompatible)
    {
        slot->m_gsStyle &= ~0x10;
    }
    else
    {
        slot->m_gsStyle |= 0x10;
    }
}

void RepositoryWnd::SetSlotTooRich(int slotIdx, bool bTooRich)
{
    if (slotIdx < 0 || slotIdx >= static_cast<int>(m_slots.size()))
    {
        return;
    }
    GeomSlot* slot = m_slots[slotIdx];
    if (!slot)
    {
        return;
    }
    if (bTooRich)
    {
        slot->m_gsStyle |= 0x20;
    }
    else
    {
        slot->m_gsStyle &= ~0x20;
    }
}

void RepositoryWnd::UpdateSlotsStylesCompatibleWithVehicle()
{
    if (!m_repository || m_type != REPOSITORYTYPE_SHOP)
    {
        return;
    }

    int playerVehicleId = -1;
    if (ai::thePlayer)
    {
        if (auto* v = ai::thePlayer->GetVehicle())
        {
            playerVehicleId = v->GetId();
        }
    }

    for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
    {
        GeomSlot* slot = m_slots[i];
        if (!slot)
        {
            continue;
        }
        int objId = slot->GetItem().GetObjId();
        if (objId < 0)
        {
            continue;
        }
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
        if (!obj || !obj->IsKindOf(&ai::Obj::m_classObj))
        {
            continue;
        }
        if (!obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart) && !obj->IsKindOf(&ai::Gadget::m_classGadget))
        {
            continue;
        }
        if (playerVehicleId != -1)
        {
            SetSlotCompatibleWithVehicle(i, help::IsChildObjCompatibleWithVehicle(objId, playerVehicleId));
        }
        else
        {
            SetSlotCompatibleWithVehicle(i, false);
        }
    }
}

void RepositoryWnd::UpdateSlotsStylesTooRich()
{
    if (!m_repository || m_type != REPOSITORYTYPE_SHOP)
    {
        return;
    }

    ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    int townId = town ? town->GetId() : -1;
    int playerMoney = ai::thePlayer->GetMoney();

    for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
    {
        GeomSlot* slot = m_slots[i];
        if (!slot)
        {
            continue;
        }
        int objId = slot->GetItem().GetObjId();
        if (objId < 0)
        {
            continue;
        }
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
        if (!obj || !obj->IsKindOf(&ai::Obj::m_classObj))
        {
            continue;
        }
        if (townId != -1)
        {
            int price = help::GetBuyPriceByObjId(objId, townId);
            SetSlotTooRich(i, price < 0 || price > playerMoney);
        }
        else
        {
            SetSlotTooRich(i, false);
        }
    }
}

void RepositoryWnd::AddInfoToEncyclopaedia()
{
    if (!m_repository)
    {
        return;
    }
    unsigned int numItems = m_repository->GetNumItems();
    for (unsigned int i = 0; i < numItems; ++i)
    {
        ai::GeomRepositoryItem item = m_repository->GetItem(static_cast<int>(i));
        M3D_APP->EnqueueMessage(66557, item.GetObjId(), 0, 0, 0, {}, {});
    }
}

// ---------------------------------------------------------------------------
// drag / drop
// ---------------------------------------------------------------------------

int RepositoryWnd::CanAddDragItem(bool)
{
    if (!DragDropItemsWnd::m_dragSlot || !m_repository)
    {
        return 0;
    }

    if (m_type != REPOSITORYTYPE_SHOP)
    {
        if (m_curBunch.m_state == SLOTSTATE_SUITABLE)
        {
            PointBase<int> origin{m_curBunch.m_bounds.x0, m_curBunch.m_bounds.y0};
            return m_repository->CanAddThingToPlace(DragDropItemsWnd::m_dragSlot->GetItem(), origin, nullptr) ? 1 : 0;
        }
        return 0;
    }

    if (DragDropItemsWnd::m_dragSlot->m_srcItemsWnd == this ||
        !IsItemCompatibleByResourceType(DragDropItemsWnd::m_dragSlot->GetItem()))
    {
        return 0;
    }
    return static_cast<int>(DragDropItemsWnd::m_dragSlot->GetItem().GetAmount());
}

bool RepositoryWnd::CanWindowQuickDropToMe(m3d::ui::Wnd const* srcWnd, ai::GeomRepositoryItem const& item) const
{
    if (!srcWnd || srcWnd == this)
    {
        return false;
    }

    if (srcWnd->IsKindOf(&WareWnd::m_classWareWnd))
    {
        if (m_type != REPOSITORYTYPE_PLAYER)
        {
            return false;
        }
    }
    else
    {
        auto* srcRepo = dynamic_cast<RepositoryWnd const*>(srcWnd);
        if (!srcRepo)
        {
            return false;
        }
        if (m_type != REPOSITORYTYPE_PLAYER)
        {
            if (m_type <= REPOSITORYTYPE_PLAYER || m_type > REPOSITORYTYPE_WORKSHOP_VEHICLE)
            {
                return false;
            }
            if (srcRepo->m_type != REPOSITORYTYPE_PLAYER)
            {
                return false;
            }
        }
    }

    if (!item.IsValid())
    {
        return false;
    }
    return IsItemCompatibleByResourceType(item);
}

int RepositoryWnd::UpdateOnItemQuickDrop(ItemAcceptInfo const& info)
{
    m3d::ui::Wnd* src = info.m_eventSrcWnd;
    if (!src || src == this)
    {
        return 0;
    }
    if (!CanWindowQuickDropToMe(src, info.m_item))
    {
        return 0;
    }
    if (!m_repository->CanAddThing(info.m_item))
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NotEnoughSpace")), 1u, false);
        return 0;
    }

    ItemAcceptInfo out(this, src, info.m_item);
    M3D_APP->m_pInterfaceManager->LaunchEvent(97, GUI_EVENT_CUSTOM, &out);
    return 1;
}

int RepositoryWnd::GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd)
{
    if (!m_repository)
    {
        return 0;
    }
    if (item.m_parentRepository != m_repository || !item.IsValid())
    {
        return 0;
    }

    unsigned int amount = 0;
    PointBase<int> pt{0, 0};

    if (m_type == REPOSITORYTYPE_SHOP)
    {
        ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
        if (!town)
        {
            M3D_APP->m_pInterfaceManager->RemoveWindow(36);
            return 0;
        }
        int unitPrice = help::GetBuyPriceByObjId(item.GetObjId(), town->GetId());
        if (unitPrice < 0)
        {
            M3D_APP->m_pInterfaceManager->RemoveWindow(36);
            return 0;
        }
        int total = unitPrice * static_cast<int>(item.GetAmount());
        if (ai::thePlayer->GetMoney() < total)
        {
            M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NotEnoughGold")), 1u, false);
            M3D_APP->m_pInterfaceManager->RemoveWindow(36);
            return 0;
        }
        ai::thePlayer->AddMoney(-total);
        BoundsBase<int> b = item.GetBounds();
        pt = {b.x0, b.y0};
        amount = item.GetAmount();
    }
    else if (m_type != REPOSITORYTYPE_PLAYER)
    {
        BoundsBase<int> b = item.GetBounds();
        pt = {b.x0, b.y0};
        amount = item.GetAmount();
    }
    else
    {
        auto* targetRepo = dynamic_cast<RepositoryWnd*>(targetWnd);
        bool sellingTarget = (targetWnd && targetWnd->IsKindOf(&WareWnd::m_classWareWnd)) ||
            (targetRepo && targetRepo->m_type == REPOSITORYTYPE_SHOP);
        if (sellingTarget)
        {
            ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
            if (town)
            {
                int unitPrice = help::GetSellPriceByObjId(item.GetObjId(), town->GetId());
                if (unitPrice >= 0)
                {
                    ai::thePlayer->AddMoney(unitPrice * static_cast<int>(item.GetAmount()));
                }
            }
        }
        BoundsBase<int> b = item.GetBounds();
        pt = {b.x0, b.y0};
        amount = item.GetAmount();
    }

    unsigned int slot = static_cast<unsigned int>(m_repository->GetSlotByPlace(pt));
    unsigned int given = m_repository->GiveUpThingFromSlot(slot, amount);
    M3D_APP->m_pInterfaceManager->RemoveWindow(36);
    return static_cast<int>(given);
}

void RepositoryWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    UpdateCellBunchWhileDrag();
}

void RepositoryWnd::OnUpdateWhileNoDrag(PointBase<float> const& wndPt)
{
    UpdateCellBunchWhileNoDrag(wndPt);
}

void RepositoryWnd::OnDragOut()
{
    m_curBunch.m_bounds.x0 = 0;
    m_curBunch.m_bounds.y0 = 0;
    m_curBunch.m_bounds.width = 0;
    m_curBunch.m_bounds.height = 0;
    m_curBunch.m_state = SLOTSTATE_UNSELECTED;

    M3D_APP->m_pInterfaceManager->RemoveWindow(36);
    HideVideo();

    if (!DragDropItemsWnd::IsDragging() && m_prevOverroledSlotId != -1)
    {
        M3D_APP->m_pInterfaceManager->LaunchEvent(95, GUI_EVENT_CUSTOM, nullptr);
        m_prevOverroledSlotId = -1;
    }
}

int RepositoryWnd::OnDragRemove()
{
    int result = DragDropItemsWnd::OnDragRemove();
    if (!result)
    {
        return result;
    }
    m_curBunch.m_bounds.x0 = 0;
    m_curBunch.m_bounds.y0 = 0;
    m_curBunch.m_bounds.width = 0;
    m_curBunch.m_bounds.height = 0;
    m_curBunch.m_state = SLOTSTATE_UNSELECTED;
    return 1;
}

int RepositoryWnd::StartDrag()
{
    M3D_APP->m_pInterfaceManager->RemoveWindow(36);
    HideVideo();
    return DragDropItemsWnd::StartDrag();
}

void RepositoryWnd::HideDragSrc()
{
    if (GeomSlot* slot = GetDraggingSlot())
    {
        slot->ShowWindow(false);
    }
}

void RepositoryWnd::ShowDragSrc()
{
    if (GeomSlot* slot = GetDraggingSlot())
    {
        slot->ShowWindow(true);
    }
}

void RepositoryWnd::UpdateCellBunchWhileDrag()
{
    if (!m_isInited || !m_repository || !DragDropItemsWnd::m_dragSlot)
    {
        return;
    }

    DragSlot* dragSlot = DragDropItemsWnd::m_dragSlot;
    BoundsBase<float> dragB = dragSlot->GetBounds();
    PointBase<float> selfScreen = ToScreen(PointBase<float>{0.0f, 0.0f});
    float localX = (dragB.x0 - selfScreen.x) - m_clientEdges[0];
    float localY = (dragB.y0 - selfScreen.y) - m_clientEdges[1];

    PointBase<float> origin = GetOriginPoint();
    float fGeomX = (origin.x + localX) / m_cellSize.x;
    float fGeomY = (origin.y + localY) / m_cellSize.y;

    PointBase<int> geomOrigin;

    double ip = 0.0;
    double fr = std::modf(static_cast<double>(fGeomX), &ip);
    geomOrigin.x = (fr >= 0.5) ? static_cast<int>(ip) + 1 : static_cast<int>(ip);
    int maxX = m_geomSize.x - dragSlot->GetItem().GetGeomSize().x;
    if (geomOrigin.x < 0)
    {
        geomOrigin.x = 0;
    }
    if (geomOrigin.x > maxX)
    {
        geomOrigin.x = maxX;
    }

    fr = std::modf(static_cast<double>(fGeomY), &ip);
    geomOrigin.y = (fr >= 0.5) ? static_cast<int>(ip) + 1 : static_cast<int>(ip);
    int maxY = m_geomSize.y - dragSlot->GetItem().GetGeomSize().y;
    if (geomOrigin.y < 0)
    {
        geomOrigin.y = 0;
    }
    if (geomOrigin.y > maxY)
    {
        geomOrigin.y = maxY;
    }

    m_curBunch.m_bounds = dragSlot->GetItem().GetBounds();
    m_curBunch.m_bounds.x0 = geomOrigin.x;
    m_curBunch.m_bounds.y0 = geomOrigin.y;

    if (!IsItemCompatibleByResourceType(dragSlot->GetItem()))
    {
        m_curBunch.m_state = SLOTSTATE_UNSUITABLE;
        return;
    }
    if (m_type == REPOSITORYTYPE_SHOP)
    {
        m_curBunch.m_state = (dragSlot->m_srcItemsWnd == this) ? SLOTSTATE_UNSUITABLE : SLOTSTATE_SUITABLE;
        return;
    }
    if (m_repository->CanAddThingToPlace(dragSlot->GetItem(), geomOrigin, nullptr))
    {
        m_curBunch.m_state = SLOTSTATE_SUITABLE;
        return;
    }

    PointBase<int> const& itemOrigin = dragSlot->GetItem().m_origin;
    if (m_isInited && dragSlot->m_srcItemsWnd == this && itemOrigin.x == geomOrigin.x && itemOrigin.y == geomOrigin.y)
    {
        m_curBunch.m_state = SLOTSTATE_OWN_PLACE_IN_HIDDEN_MODE;
    }
    else
    {
        m_curBunch.m_state = SLOTSTATE_UNSUITABLE;
    }
}

void RepositoryWnd::UpdateCellBunchWhileNoDrag(PointBase<float> const& mousePt)
{
    if (!m_isInited || !m_repository || DragDropItemsWnd::m_dragSlot)
    {
        return;
    }

    PointBase<int> cell = WndToGeomPt(mousePt);
    int slot = m_repository->GetSlotByPlace(cell);

    if (slot < 0 || slot >= static_cast<int>(m_slots.size()))
    {
        if (m_curBunch.m_bounds.y0 != 0 || m_curBunch.m_bounds.width != 0)
        {
            m_curBunch.Reset();
            M3D_APP->m_pInterfaceManager->RemoveWindow(36);
            HideVideo();
        }
        if (m_prevOverroledSlotId != -1)
        {
            M3D_APP->m_pInterfaceManager->LaunchEvent(95, GUI_EVENT_CUSTOM, nullptr);
            m_prevOverroledSlotId = -1;
        }
        return;
    }

    ai::GeomRepositoryItem item = m_repository->GetItem(slot);
    m_curBunch.m_bounds = item.GetBounds();
    m_curBunch.m_state = SLOTSTATE_SELECTED;
    ShowInfo(slot);
    ShowVideo(slot);
    OnSlotIn(slot);
}

// ---------------------------------------------------------------------------
// info / video panels
// ---------------------------------------------------------------------------

int RepositoryWnd::ShowInfo(int itemId)
{
    if (!m_isInited || !m_repository)
    {
        return 0;
    }
    ai::GeomRepositoryItem item = m_repository->GetItem(itemId);
    if (!item.IsValid())
    {
        return 0;
    }

    ref_ptr<m3d::ui::Wnd> infoWnd = M3D_APP->m_pInterfaceManager->GetWindow(36);
    if (!infoWnd)
    {
        infoWnd = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("ItemInfoWnd"));
        if (!infoWnd)
        {
            return 0;
        }
        auto* iiw = RT_DYNCAST(infoWnd.get(), ItemInfoWnd);
        if (!iiw)
        {
            return 0;
        }
        iiw->SetUpForRepositoryItem(item, this);
        if (!M3D_APP->m_pInterfaceManager->AddWindowById(infoWnd.get(), 36, false, true))
        {
            return 0;
        }
        M3D_APP->MoveChildToFirstPosition(infoWnd.get());
        return 1;
    }

    if (auto* iiw = RT_DYNCAST(infoWnd.get(), ItemInfoWnd))
    {
        // NOTE: the original first checks the panel is not already showing this
        // exact item (private ItemInfoWnd field); we just refresh it.
        iiw->SetUpForRepositoryItem(item, this);
        M3D_APP->MoveChildToFirstPosition(infoWnd.get());
    }
    return 1;
}

int RepositoryWnd::HideInfo()
{
    return M3D_APP->m_pInterfaceManager->RemoveWindow(36);
}

int RepositoryWnd::ShowVideo(int itemId)
{
    if (!m_isInited || !m_repository)
    {
        return 0;
    }
    ai::GeomRepositoryItem item = m_repository->GetItem(itemId);
    if (!item.IsValid())
    {
        return 0;
    }

    ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(77);
    auto* videoWnd = w ? RT_DYNCAST(w.get(), VideoWnd) : nullptr;
    if (!videoWnd)
    {
        return 0;
    }
    // NOTE: the original also early-outs when the VideoWnd is already showing this
    // exact object for this window (private VideoWnd src/obj fields).
    return videoWnd->ShowModelByObjId(item.GetObjId(), this);
}

int RepositoryWnd::HideVideo()
{
    ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(77);
    if (!w)
    {
        return 0;
    }
    auto* videoWnd = RT_DYNCAST(w.get(), VideoWnd);
    if (!videoWnd)
    {
        return 0;
    }
    videoWnd->HideModel(this);
    return 1;
}

// ---------------------------------------------------------------------------
// enable / paint / input
// ---------------------------------------------------------------------------

void RepositoryWnd::Enable(bool needEnable)
{
    if (needEnable)
    {
        m_style &= ~2u;
        m_curClr = 0;
        for (auto* slot : m_slots)
        {
            if (slot)
            {
                slot->SetColor(0);
            }
        }
        if (m_scrollVWnd)
        {
            m_scrollVWnd->EnableWindow(m_scrollVWnd->GetMaxPos() > 0.0f);
        }
    }
    else
    {
        m_style |= 2u;
        m_curClr = 3;
        if (DragDropItemsWnd::m_dragSlot && DragDropItemsWnd::m_dragSlot->GetItem().m_parentRepository == m_repository)
        {
            DragDropItemsWnd::RemoveDragSlot();
            OnDragRemove();
        }
        for (auto* slot : m_slots)
        {
            if (slot)
            {
                slot->SetColor(3u);
            }
        }
        if (m_scrollVWnd)
        {
            m_scrollVWnd->EnableWindow(false);
        }
    }
}

int RepositoryWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    if (!m_isInited || !m_repository)
    {
        return 0;
    }
    DrawGrid(di);
    DrawUnsuitableBg(di);
    DrawBunch(di);
    DrawFrame(di);
    return 1;
}

void RepositoryWnd::DrawGrid(m3d::ui::DrawInfo const& di)
{
    float sy = m_scrollVWnd ? m_scrollVWnd->GetCurPos() : 0.0f;
    if (m_scrollHWnd)
    {
        m_scrollHWnd->GetCurPos();  // read for parity with the original
    }

    float phaseY = sy / m_cellSize.y;
    float fracY = phaseY - std::floor(phaseY);
    float tilesY = ((m_bounds.height - m_clientEdges[1] - m_clientEdges[3]) / m_cellSize.y) + fracY;

    float scrollBarW = 0.0f;
    if (m_scrollVWnd)
    {
        scrollBarW = m_scrollVWnd->GetBounds().width + m_clientEdges[2];
    }

    BoundsBase<float> gridB;
    gridB.x0 = m_clientEdges[0];
    gridB.y0 = m_clientEdges[1];
    gridB.width = m_bounds.width - (m_clientEdges[0] * 2.0f + scrollBarW);
    gridB.height = m_bounds.height - (m_clientEdges[1] + m_clientEdges[3]);

    GetGfxServer()->AddImagedRectGeneral(
        di, gridB, m_curClr, m_cellTextures[0], 0.0f, fracY, static_cast<float>(m_geomSize.x), tilesY);
}

void RepositoryWnd::DrawBunch(m3d::ui::DrawInfo const& di)
{
    if (m_curBunch.m_bounds.width == 0 && m_curBunch.m_bounds.height == 0)
    {
        return;
    }

    BoundsBase<float> bunchB = GeomToWndBounds(m_curBunch.m_bounds);
    if (m_curBunch.m_state == SLOTSTATE_SELECTED)
    {
        GetGfxServer()->AddImagedRectGeneral(
            di,
            bunchB,
            0xFFFFFFFFu,
            m_cellTextures[1],
            0.0f,
            0.0f,
            static_cast<float>(m_curBunch.m_bounds.width),
            static_cast<float>(m_curBunch.m_bounds.height));
    }
    else
    {
        GetGfxServer()->AddImagedRect(di, bunchB, 0xFFFFFFFFu, m_cellTextures[m_curBunch.m_state]);
    }
}

void RepositoryWnd::DrawFrame(m3d::ui::DrawInfo const& di)
{
    m3d::ui::DrawInfo fakeDi(di);

    float used = 0.0f;
    if (auto* pane = GetGfxServer()->GetPane(m_paneName))
    {
        if (auto* frame = pane->m_frame[0])
        {
            used = static_cast<float>(frame->m_barUsedWidth);
        }
    }

    fakeDi.m_originalRect.x0 -= used;
    fakeDi.m_originalRect.y0 -= used;
    fakeDi.m_originalRect.width += used * 2.0f;
    fakeDi.m_originalRect.height += used * 2.0f;
    fakeDi.m_clippedRect.x0 -= used;
    fakeDi.m_clippedRect.y0 -= used;
    fakeDi.m_clippedRect.width += used * 2.0f;
    fakeDi.m_clippedRect.height += used * 2.0f;

    m3d::ui::Wnd::OnPaint(fakeDi);
}

void RepositoryWnd::DrawUnsuitableBg(m3d::ui::DrawInfo const& di)
{
    if (!DragDropItemsWnd::IsDragging())
    {
        return;
    }

    float const eps = 0.001f;
    for (auto* slot : m_slots)
    {
        if (!slot)
        {
            continue;
        }
        if (ai::operator==(slot->GetItem(), DragDropItemsWnd::GetDragItem()))
        {
            continue;
        }

        BoundsBase<float> slotB = slot->GetBounds();
        BoundsBase<float> bunchWndB = GeomToWndBounds(m_curBunch.m_bounds);

        BoundsBase<float> inter = IntersectRectsF(slotB, bunchWndB);
        if (inter.width <= eps || inter.height <= eps)
        {
            GetGfxServer()->AddImagedRect(di, slotB, 0xFFFFFFFFu, m_unsuitableBg);
            continue;
        }

        BoundsBase<int> slotGeomB = WndToGeomBounds(slotB);
        for (int gx = slotGeomB.x0; gx < slotGeomB.x0 + slotGeomB.width; ++gx)
        {
            for (int gy = slotGeomB.y0; gy < slotGeomB.y0 + slotGeomB.height; ++gy)
            {
                BoundsBase<float> cellWndB = GeomToWndBounds(BoundsBase<int>{gx, gy, 1, 1});
                BoundsBase<float> cellInter = IntersectRectsF(cellWndB, bunchWndB);
                if (cellInter.width <= eps || cellInter.height <= eps)
                {
                    GetGfxServer()->AddImagedRect(di, cellWndB, 0xFFFFFFFFu, m_unsuitableBg);
                }
            }
        }
    }
}

int RepositoryWnd::OnMouseButton1(unsigned int state, PointBase<float> const& at)
{
    if (!m3d::ui::Wnd::OnMouseButton1(state, at))
    {
        return 0;
    }
    if (state && !DragDropItemsWnd::IsDragging())
    {
        ai::GeomRepositoryItem item = GetItemFromOrigin(at);
        if (item.IsValid())
        {
            ItemAcceptInfo info(this, nullptr, item);
            M3D_APP->m_pInterfaceManager->LaunchEvent(99, GUI_EVENT_CUSTOM, &info);
        }
    }
    return 1;
}

int RepositoryWnd::OnMouseWheel(int ticks, PointBase<float> const& at)
{
    if (m_scrollVWnd && (m_scrollVWnd->GetStyle() & 2) == 0 && ticks != 0)
    {
        float rem = std::fmod(m_scrollVWnd->GetCurPos(), m_cellSize.y);
        float step = ticks > 0 ? rem : m_cellSize.y - rem;
        if (step <= 0.001f)
        {
            step = m_cellSize.y;
        }
        if (step > 0.001f)
        {
            m_scrollVWnd->SetCurPos(m_scrollVWnd->GetCurPos() - static_cast<float>(ticks) * step);
            ScrollSlots();
        }
    }

    // The style-0x20 "reflect wheel to parent" behaviour is identical to the base
    // implementation, so defer to it rather than re-poking a protected method
    // through a base-class pointer.
    m3d::ui::Wnd::OnMouseWheel(ticks, at);
    return 1;
}

int RepositoryWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
{
    int result = DragDropItemsWnd::OnWndNotify(from, idFrom, message, data);
    if (result)
    {
        return result;
    }
    if (message != 5)
    {
        return result;
    }
    ScrollSlots();
    return 1;
}

int RepositoryWnd::OnBeforeAddToWndStation()
{
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int RepositoryWnd::OnAfterRemoveFromWndStation()
{
    int r = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    M3D_APP->m_pInterfaceManager->RemoveWindow(36);
    HideVideo();
    return r;
}

void RepositoryWnd::OnSlotIn(int slotId)
{
    if (m_prevOverroledSlotId != -1)
    {
        M3D_APP->m_pInterfaceManager->LaunchEvent(95, GUI_EVENT_CUSTOM, nullptr);
        m_prevOverroledSlotId = -1;
    }
    if (slotId == -1)
    {
        return;
    }

    ai::GeomRepositoryItem item = m_repository->GetItem(slotId);
    ItemAcceptInfo info(this, nullptr, item);
    M3D_APP->m_pInterfaceManager->LaunchEvent(94, GUI_EVENT_CUSTOM, &info);
    m_prevOverroledSlotId = slotId;
}

void RepositoryWnd::OnSlotOut()
{
    if (m_prevOverroledSlotId != -1)
    {
        M3D_APP->m_pInterfaceManager->LaunchEvent(95, GUI_EVENT_CUSTOM, nullptr);
        m_prevOverroledSlotId = -1;
    }
}
