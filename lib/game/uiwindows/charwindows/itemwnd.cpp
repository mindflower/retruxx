#include "itemwnd.h"
#include "dragdropitemswnd.h"
#include "iteminfownd.h"
#include "videownd.h"
#include "warewnd.h"

#include "core/kernel.h"
#include <game/m3dgame.h>
#include <m3dapp.h>
#include <game/uimisc/guihelper.h>

#include <ui/frame.h>
#include <ui/ui_srv.h>
#include <ui/wnd.h>

#include <renderer/i_renderer.h>

#include <server/server.h>
#include <server/geomrepository.h>
#include <server/geomrepositoryitem.h>
#include <server/izvratrepository.h>
#include <server/resourcemanager.h>
#include <server/objects/vehicle.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypemanager.h>

RT_CLASS_EXPORTS_BEGIN(ItemWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ItemWnd);

namespace
{
    // The "disabled" style bits force colour index 3 (grey) for every draw call.
    unsigned int ItemDrawColor(unsigned int style, unsigned int curClr)
    {
        return ((style & 2) != 0 || (style & 0x80000) != 0) ? 3u : curClr;
    }
}  // namespace

// ---------------------------------------------------------------------------
//  AuxInfo
// ---------------------------------------------------------------------------

ItemWnd::AuxInfo::AuxInfo()
{
    m_cellTexNames[0] = "Cell";
    m_cellTexNames[1] = "CellSelected";
    m_cellTexNames[2] = "CellSuitable";
    m_cellTexNames[3] = "CellUnsuitable";
    m_cellTexNames[4] = "CellSuitable";
    // m_cellTexNames[5] deliberately left empty (matches the shipped ctor).
    m_suitableDistantPane = "PaneGun";
}

// ---------------------------------------------------------------------------
//  lifetime
// ---------------------------------------------------------------------------

ItemWnd::ItemWnd()
{
    m_vehicleId = -1;
    m_geomSize.x = 1;
    m_geomSize.y = 1;
    m_state = SLOTSTATE_UNSELECTED;
    m_isDragging = false;
    m_drawStyle = 0;
    m_itemId = -1;
    SetDefaultFont(1);
    // m_itemIco / m_cellTextures default-construct to the invalid handle.
}

ItemWnd::~ItemWnd()
{
    if (m_itemIco.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_itemIco);
        m_itemIco.SetInvalid();
    }
    for (auto& tex : m_cellTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex);
    }
}

m3d::Class* ItemWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

m3d::Class* ItemWnd::GetRtClass() const
{
    return &ItemWnd::m_classItemWnd;
}

// ---------------------------------------------------------------------------
//  trivial accessors
// ---------------------------------------------------------------------------

int ItemWnd::GetDrawStyle() const
{
    return m_drawStyle;
}

void ItemWnd::SetDrawStyle(int drawStyle)
{
    m_drawStyle = drawStyle;
}

int ItemWnd::GetItemObjId() const
{
    return m_itemId;
}

bool ItemWnd::SetItemObjId(int objId)
{
    m_itemId = objId;
    return true;
}

bool ItemWnd::IsEmpty() const
{
    return GetItemObjId() == -1;
}

bool ItemWnd::IsHidden() const
{
    return m_dragStyle == DRAGSTYLE_HIDDEN_SRC && m_isDragging;
}

bool ItemWnd::IsDisabled() const
{
    return (m_style & 2) != 0 || (m_style & 0x80000) != 0;
}

bool ItemWnd::CanChangeItem() const
{
    return true;
}

bool ItemWnd::CanContainItem() const
{
    return true;
}

bool ItemWnd::NeedUpdateInfoWnd(ItemInfoWnd* infoWnd) const
{
    return infoWnd != nullptr;
}

int ItemWnd::GetResourceId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Obj* ItemWnd::GetItem() const
{
    return m_itemId >= 0 ? ai::theObjects->GetEntityByObjId(m_itemId) : nullptr;
}

ai::Vehicle* ItemWnd::GetVehicle() const
{
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    auto* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    return (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle)) ? static_cast<ai::Vehicle*>(obj) : nullptr;
}

int ItemWnd::GetItemPrototypeId() const
{
    ai::Obj* item = GetItem();
    return item ? item->GetPrototypeId() : -1;
}

m3d::rend::TexHandle ItemWnd::GetItemIco(int prototypeId) const
{
    if (prototypeId == -1)
    {
        return m3d::rend::TexHandle{};
    }
    return M3D_APP->m_pInterfaceManager->GetIcoByName(ai::thePrototypeManager->GetPrototypeName(prototypeId), 0);
}

PointBase<int> ItemWnd::GetCorrectGeomSize() const
{
    if (ai::theResourceManager)
    {
        if (ai::Resource* resource = ai::theResourceManager->GetResource(GetResourceId()))
        {
            return resource->GetGeomSize();
        }
    }
    return PointBase<int>{1, 1};
}

ai::GeomRepositoryItem ItemWnd::GetAsRepositoryItem() const
{
    return ai::GeomRepositoryItem(GetItemObjId());
}

ai::GeomRepositoryItem ItemWnd::GetItemFromOrigin(PointBase<float> const&)
{
    return GetAsRepositoryItem();
}

BoundsBase<float> ItemWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    return BoundsBase<float>{0.0f, 0.0f, m_bounds.width, m_bounds.height};
}

bool ItemWnd::IsValid() const
{
    return GetVehicle() != nullptr && (IsEmpty() || GetItem() != nullptr);
}

// ---------------------------------------------------------------------------
//  empty hooks (present so subclasses can override; no-ops on ItemWnd)
// ---------------------------------------------------------------------------

void ItemWnd::HideDragSrc()
{
}
void ItemWnd::ShowDragSrc()
{
}
void ItemWnd::SetupInfoWnd(ItemInfoWnd*)
{
}
void ItemWnd::LaunchEventOnItemDeactivation() const
{
}
void ItemWnd::UpdateItemValue()
{
}

m3d::Object* ItemWnd::RepositoryClone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ItemWnd::RepositoryDeactivateItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

// ---------------------------------------------------------------------------
//  creation
// ---------------------------------------------------------------------------

int ItemWnd::Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id)
{
    if (!m3d::ui::Wnd::Create(caption, style, rc, id))
    {
        return 0;
    }
    InitTextures();
    return 1;
}

int ItemWnd::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    if (!patternWnd)
    {
        return 0;
    }
    if (!m3d::ui::Wnd::Create(CStr(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        return 0;
    }
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(const_cast<m3d::ui::Wnd*>(patternWnd));
        const_cast<m3d::ui::Wnd*>(patternWnd)->DecRef();
    }
    return 1;
}

int ItemWnd::CreateDragSlotFromWndPt(PointBase<float> const& wndPt)
{
    return CanChangeItem() ? DragDropItemsWnd::CreateDragSlotFromWndPt(wndPt) : 0;
}

void ItemWnd::InitTextures()
{
    for (int i = 0; i < 6; ++i)
    {
        m_cellTextures[i] = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_cellTexNames[i], 0);
        if (m_cellTextures[i].IsValid())
        {
            M3D_RENDERER->ReferenceTexture(m_cellTextures[i]);
        }
    }
}

void ItemWnd::Reset()
{
    if (m_itemIco.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_itemIco);
    }
    m_itemIco.SetInvalid();
    m_state = SLOTSTATE_UNSELECTED;
    m_isDragging = false;
}

// ---------------------------------------------------------------------------
//  item value / icon
// ---------------------------------------------------------------------------

void ItemWnd::SetVehicleId(int vehicleId)
{
    m_vehicleId = vehicleId;
    if (GetVehicle())
    {
        OnItemChanged();
    }
    else
    {
        Reset();
    }
}

void ItemWnd::OnItemChanged()
{
    UpdateItemValue();
    UpdateItemIco();
}

void ItemWnd::UpdateItemIco()
{
    m3d::rend::TexHandle const itemIco = GetItemIco(GetItemPrototypeId());

    if (m_itemIco.IsValid())
    {
        M3D_RENDERER->ReleaseTexture(m_itemIco);
        m_itemIco.SetInvalid();
    }
    if (itemIco.IsValid())
    {
        m_itemIco = itemIco;
        M3D_RENDERER->ReferenceTexture(m_itemIco);
    }
}

// ---------------------------------------------------------------------------
//  activate / deactivate
// ---------------------------------------------------------------------------

int ItemWnd::DeactivateItem()
{
    if (!IsEmpty())
    {
        if (!IsValid() || !CanChangeItem())
        {
            return 0;
        }
        if (!IsEmpty() && !m_isDragging)
        {
            LaunchEventOnItemDeactivation();
        }
        SetItemObjId(-1);
    }
    return 1;
}

int ItemWnd::ActivateItem(ai::GeomRepositoryItem const& item)
{
    if (!GetVehicle())
    {
        return 0;
    }
    ai::GeomRepositoryItem const objectItem = item.GetConvertedToObjectItem();
    return SetItemObjId(objectItem.m_objId);
}

int ItemWnd::AddItem(ai::GeomRepositoryItem const& item)
{
    return (DeactivateItem() & 1) & ActivateItem(item);
}

bool ItemWnd::CanAddDragItemDistant(ai::GeomRepositoryItem const& item) const
{
    if (!GetVehicle())
    {
        return false;
    }
    if (item.IsValid() && ai::theResourceManager->bResourceIsKindOf(item.m_resourceId, GetResourceId()) &&
        CanContainItem())
    {
        return CanChangeItem();
    }
    return false;
}

int ItemWnd::CanAddDragItem(bool bSilent)
{
    if (!CanAddDragItemDistant(DragDropItemsWnd::GetDragItem()))
    {
        return 0;
    }

    if (M3D_APP->m_pInterfaceManager->IsInSaleMode())
    {
        if (ai::Vehicle* vehicle = GetVehicle())
        {
            ai::IzvratRepository* repository = vehicle->GetRepository();
            if (!IsEmpty() && m_itemId != DragDropItemsWnd::m_dragSlot->GetItem().GetObjId())
            {
                if (!repository || !repository->CanAddThing(GetAsRepositoryItem()))
                {
                    if (!bSilent)
                    {
                        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NotEnoughSpace")), 1u, false);
                    }
                    return 0;
                }
            }
        }
    }
    return !m_isDragging;
}

// ---------------------------------------------------------------------------
//  drawing
// ---------------------------------------------------------------------------

int ItemWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    DrawFrame(di);
    DrawBg(di);
    DrawGrid(di);
    DrawSelection(di);
    if (m_dragStyle != DRAGSTYLE_HIDDEN_SRC || !m_isDragging)
    {
        DrawItemIco(di);
        DrawItemInfo(di);
    }
    return 1;
}

void ItemWnd::DrawBg(m3d::ui::DrawInfo const& di)
{
    if (!m_bgTexture.IsValid())
    {
        return;
    }
    BoundsBase<float> const rect{0.0f, 0.0f, m_bounds.width, m_bounds.height};
    m_gfx->AddImagedRect(di, rect, ItemDrawColor(m_style, m_curClr), m_bgTexture);
}

void ItemWnd::DrawItemIco(m3d::ui::DrawInfo const& di)
{
    if (IsEmpty())
    {
        return;
    }
    BoundsBase<float> const rect{0.0f, 0.0f, m_bounds.width, m_bounds.height};
    m_gfx->AddImagedRect(di, rect, ItemDrawColor(m_style, m_curClr), m_itemIco);
}

void ItemWnd::DrawSelection(m3d::ui::DrawInfo const& di)
{
    if (m_state > SLOTSTATE_UNSELECTED && m_state <= SLOTSTATE_OWN_PLACE_IN_HIDDEN_MODE)
    {
        BoundsBase<float> const rect{0.0f, 0.0f, m_bounds.width, m_bounds.height};
        m_gfx->AddImagedRect(di, rect, ItemDrawColor(m_style, m_curClr), m_cellTextures[m_state]);
    }
}

void ItemWnd::DrawGrid(m3d::ui::DrawInfo const& di)
{
    int const resourceId = GetResourceId();
    bool const isCabinOrBasket =
        ai::theResourceManager->bResourceIsKindOf(resourceId, ai::theResourceManager->GetResourceId(CStr("CABIN"))) ||
        ai::theResourceManager->bResourceIsKindOf(resourceId, ai::theResourceManager->GetResourceId(CStr("BASKET")));
    if (isCabinOrBasket)
    {
        return;
    }

    BoundsBase<float> const rect{0.0f, 0.0f, m_bounds.width, m_bounds.height};
    m_gfx->AddImagedRectGeneral(
        di,
        rect,
        ItemDrawColor(m_style, m_curClr),
        m_cellTextures[0],
        0.0f,
        0.0f,
        static_cast<float>(m_geomSize.x),
        static_cast<float>(m_geomSize.y));
}

void ItemWnd::DrawFrame(m3d::ui::DrawInfo const& di)
{
    if (m_paneFlags == 0)
    {
        return;
    }

    m3d::ui::DrawInfo fakeDi = di;
    m3d::ui::PaneFlagBg bgFlag = m3d::ui::PANE_FLAG_BG_OUT;
    CStr paneName;
    if (m_state == SLOTSTATE_SUITABLE || (m_state > SLOTSTATE_UNSUITABLE && m_state <= SLOTSTATE_SUITABLE_DISTANT))
    {
        paneName = m_aif.m_suitableDistantPane;
        bgFlag = m3d::ui::PANE_FLAG_BG_DOWN;
    }
    else
    {
        paneName = GetPaneName();
    }

    float barW = 0.0f;
    if (m3d::ui::Pane* pane = m_gfx->GetPane(paneName))
    {
        if (m3d::ui::Frame* frame = pane->m_frame[bgFlag])
        {
            barW = static_cast<float>(frame->m_barUsedWidth);
        }
    }

    float const grow = barW * 2.0f;
    fakeDi.m_originalRect.x0 -= barW;
    fakeDi.m_originalRect.y0 -= barW;
    fakeDi.m_originalRect.width += grow;
    fakeDi.m_originalRect.height += grow;
    fakeDi.m_clippedRect.x0 -= barW;
    fakeDi.m_clippedRect.y0 -= barW;
    fakeDi.m_clippedRect.width += grow;
    fakeDi.m_clippedRect.height += grow;

    BoundsBase<float> const b{0.0f, 0.0f, grow + m_bounds.width, grow + m_bounds.height};
    m_gfx->AddFlatAxialPane0(fakeDi, b, ItemDrawColor(m_style, m_curClr), m_paneFlags, paneName, bgFlag);
}

void ItemWnd::DrawItemInfo(m3d::ui::DrawInfo const& di)
{
    m3d::ui::GfxServer* gfx = m_gfx;
    float const lineHeight = gfx->MeasureText(CStr("A"), m_defFont, m3d::TW_NOWRAP, 1000.0f).y;

    if ((m_drawStyle & 4) != 0)
    {
        PointBase<float> const at{0.0f, di.m_clientRect.height - lineHeight};
        gfx->AddText(di, at, CStr(1), m_defFont, m3d::TW_NOWRAP, m3d::TF_LEFT);
    }

    if ((m_drawStyle & 2) != 0)
    {
        CStr text;
        int const prototypeId = GetItemPrototypeId();
        if (prototypeId != -1)
        {
            text = ai::pServer->GetPrototypeFullName(prototypeId);
        }
        PointBase<float> const sz = gfx->MeasureText(text, m_defFont, m3d::TW_WORD_WRAP, m_bounds.width);
        if (m_bounds.height >= sz.y)
        {
            PointBase<float> const at{(di.m_clientRect.width - sz.x) * 0.5f, (di.m_clientRect.height - sz.y) * 0.5f};
            gfx->AddText(di, at, text, m_defFont, m3d::TW_WORD_WRAP, m3d::TF_LEFT);
        }
    }

    if ((m_drawStyle & 1) != 0 && GetItemObjId() != -1)
    {
        // TODO(RVA 0x458330): draws help::GetPriceSmart(objId) top-right; that
        // helper routes through the unported ZnayuKakProdatWnd / town price tables.
    }
}

// ---------------------------------------------------------------------------
//  events
// ---------------------------------------------------------------------------

int ItemWnd::GameDataUpdate(void* data, int dataType)
{
    switch (dataType)
    {
    case 92:  // distant drag started somewhere
        OnDistantStartDrag();
        return 1;
    case 93:  // distant drag ended
        OnDistantEndDrag();
        return 1;
    case 94:  // pointer entered a distant slot
        if (data)
        {
            ItemAcceptInfo info(*static_cast<ItemAcceptInfo*>(data));
            if (info.m_eventSrcWnd != this)
            {
                OnDistantMouseIn(info.m_item);
            }
        }
        return 1;
    case 95:  // pointer left the distant slot
        OnDistantMouseOut();
        return 1;
    default:
        return DragDropItemsWnd::GameDataUpdate(data, dataType);
    }
}

int ItemWnd::OnBeforeAddToWndStation()
{
    OnItemChanged();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int ItemWnd::OnAfterRemoveFromWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    HideInfoWnd();
    HideVideoWnd();
    return res;
}

int ItemWnd::OnMouseIn()
{
    if (DragDropItemsWnd::m_dragSlot)
    {
        DragDropItemsWnd::m_dragSlot->SetStyle(DragDropItemsWnd::m_dragSlot->GetStyle() | 0x100u);
        M3D_APP->CaptureMouse(nullptr);
    }
    m3d::ui::Wnd::OnMouseIn();

    if (!IsEmpty() && (!DragDropItemsWnd::m_dragSlot || !DragDropItemsWnd::m_dragSlot->IsChildOf(M3D_APP)))
    {
        ItemAcceptInfo info(this, nullptr, GetAsRepositoryItem());
        M3D_APP->m_pInterfaceManager->LaunchEvent(94, GUI_EVENT_CUSTOM, &info);
    }
    return 1;
}

int ItemWnd::OnMouseOut()
{
    DragDropItemsWnd::OnMouseOut();
    if (!IsEmpty() && (!DragDropItemsWnd::m_dragSlot || !DragDropItemsWnd::m_dragSlot->IsChildOf(M3D_APP)))
    {
        M3D_APP->m_pInterfaceManager->LaunchEvent(95, GUI_EVENT_CUSTOM, nullptr);
    }
    return 1;
}

int ItemWnd::OnMouseButton1(unsigned int state, PointBase<float> const& at)
{
    if (!m3d::ui::Wnd::OnMouseButton1(state, at))
    {
        return 0;
    }
    if (state && !m_isDragging)
    {
        ai::GeomRepositoryItem const item = GetItemFromOrigin(at);
        if (item.IsValid())
        {
            m_isDragging = true;
            ItemAcceptInfo info(this, nullptr, item);
            M3D_APP->m_pInterfaceManager->LaunchEvent(99, GUI_EVENT_CUSTOM, &info);
            m_isDragging = false;
        }
    }
    return 1;
}

void ItemWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    if (!DragDropItemsWnd::m_dragSlot || !DragDropItemsWnd::m_dragSlot->IsChildOf(M3D_APP))
    {
        return;
    }
    if (CanAddDragItem(true))
    {
        m_state = SLOTSTATE_SUITABLE;
    }
    else if (m_dragStyle == DRAGSTYLE_HIDDEN_SRC && m_isDragging)
    {
        m_state = SLOTSTATE_OWN_PLACE_IN_HIDDEN_MODE;
    }
    else
    {
        m_state = SLOTSTATE_UNSUITABLE;
    }
}

void ItemWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    if (IsEmpty())
    {
        if (m_state == SLOTSTATE_SELECTED)
        {
            HideInfoWnd();
            HideVideoWnd();
        }
        m_state = SLOTSTATE_UNSELECTED;
    }
    else
    {
        if (m_state != SLOTSTATE_SELECTED)
        {
            ShowInfoWnd();
            ShowVideoWnd();
        }
        m_state = SLOTSTATE_SELECTED;
    }
}

void ItemWnd::OnDragOut()
{
    m_state =
        CanAddDragItemDistant(DragDropItemsWnd::GetDragItem()) ? SLOTSTATE_SUITABLE_DISTANT : SLOTSTATE_UNSELECTED;
    HideInfoWnd();
    HideVideoWnd();
}

int ItemWnd::OnDragRemove()
{
    if (!DragDropItemsWnd::OnDragRemove())
    {
        return 0;
    }
    m_state = SLOTSTATE_UNSELECTED;
    m_isDragging = false;
    return 1;
}

void ItemWnd::OnDistantStartDrag()
{
    if (DragDropItemsWnd::m_dragSlot && DragDropItemsWnd::m_dragSlot->IsChildOf(M3D_APP) &&
        DragDropItemsWnd::m_dragSlot->m_srcItemsWnd != this)
    {
        OnDistantMouseIn(DragDropItemsWnd::m_dragSlot->GetItem());
    }
}

void ItemWnd::OnDistantEndDrag()
{
    OnDistantMouseOut();
}

void ItemWnd::OnDistantMouseIn(ai::GeomRepositoryItem const& item)
{
    if (CanAddDragItemDistant(item))
    {
        m_state = SLOTSTATE_SUITABLE_DISTANT;
    }
}

void ItemWnd::OnDistantMouseOut()
{
    if (m_state == SLOTSTATE_SUITABLE_DISTANT)
    {
        m_state = SLOTSTATE_UNSELECTED;
    }
}

// ---------------------------------------------------------------------------
//  drag
// ---------------------------------------------------------------------------

int ItemWnd::StartDrag()
{
    HideInfoWnd();
    HideVideoWnd();
    m_isDragging = true;
    return DragDropItemsWnd::StartDrag();
}

int ItemWnd::GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd)
{
    if (!DeactivateItem())
    {
        return 0;
    }

    bool const sellsToShop = targetWnd &&
        ((targetWnd->IsKindOf(&RepositoryWnd::m_classRepositoryWnd) &&
          static_cast<RepositoryWnd*>(targetWnd)->GetRepositoryType() == RepositoryWnd::REPOSITORYTYPE_SHOP) ||
         targetWnd->IsKindOf(&WareWnd::m_classWareWnd));

    if (sellsToShop)
    {
        if (ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown())
        {
            int const unitPrice = help::GetSellPriceByObjId(item.GetObjId(), town->GetId());
            if (ai::thePlayer)
            {
                ai::thePlayer->AddMoney(unitPrice * static_cast<int>(item.GetAmount()));
            }
        }
    }

    HideInfoWnd();
    return 1;
}

// ---------------------------------------------------------------------------
//  info / video windows
// ---------------------------------------------------------------------------

int ItemWnd::ShowInfoWnd()
{
    if (!IsValid() || m_itemId < 0 || !GetItem())
    {
        return 0;
    }

    ref_ptr<m3d::ui::Wnd> infoWnd = M3D_APP->m_pInterfaceManager->GetWindow(36);
    if (!infoWnd)
    {
        infoWnd = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("ItemInfoWnd"));
        if (!infoWnd || !infoWnd->IsKindOf(&ItemInfoWnd::m_classItemInfoWnd))
        {
            return 0;
        }
        SetupInfoWnd(static_cast<ItemInfoWnd*>(infoWnd.get()));
        if (!M3D_APP->m_pInterfaceManager->AddWindowById(infoWnd.get(), 36, true, false))
        {
            return 0;
        }
        M3D_APP->MoveChildToFirstPosition(infoWnd.get());
        return 1;
    }

    if (!infoWnd->IsKindOf(&ItemInfoWnd::m_classItemInfoWnd))
    {
        return 0;
    }
    if (NeedUpdateInfoWnd(static_cast<ItemInfoWnd*>(infoWnd.get())))
    {
        SetupInfoWnd(static_cast<ItemInfoWnd*>(infoWnd.get()));
    }
    return 1;
}

int ItemWnd::HideInfoWnd()
{
    return M3D_APP->m_pInterfaceManager->RemoveWindow(36);
}

int ItemWnd::ShowVideoWnd()
{
    if (!IsValid() || m_itemId < 0 || !GetItem())
    {
        return 0;
    }
    ai::GeomRepositoryItem const item = GetAsRepositoryItem();
    if (!item.IsValid())
    {
        return 0;
    }

    ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(77);
    if (!w || !w->IsKindOf(&VideoWnd::m_classVideoWnd))
    {
        return 0;
    }
    auto* videoWnd = static_cast<VideoWnd*>(w.get());

    // The shipped build short-circuits when the video panel is already showing
    // this exact object for this slot; that check reads a private VideoWnd field
    // with no accessor in retruxx yet.
    IncRef();
    return videoWnd->ShowModelByObjId(item.GetObjId(), this);
}

int ItemWnd::HideVideoWnd()
{
    ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(77);
    if (!w || !w->IsKindOf(&VideoWnd::m_classVideoWnd))
    {
        return 0;
    }
    IncRef();
    return static_cast<VideoWnd*>(w.get())->HideModel(this);
}
