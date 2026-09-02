#include "dragdropitemswnd.h"

#include <game/m3dgame.h>
#include <m3dapp.h>

#include <ui/ui_srv.h>
#include <ui/wnd.h>
#include <ui/wndstation.h>
#include <renderer/i_renderer.h>

#include <server/geomrepositoryitem.h>

#include <cmath>

RT_CLASS_EXPORTS_BEGIN(DragDropItemsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DragDropItemsWnd);

DragSlot* DragDropItemsWnd::m_dragSlot = nullptr;
GeomSlot::AuxInfo GeomSlot::m_aif;
m3d::rend::TexHandle GeomSlot::m_unsuitableTex;
m3d::rend::TexHandle GeomSlot::m_tooRichTex;

// ===========================================================================
//  ItemAcceptInfo
// ===========================================================================

ItemAcceptInfo::ItemAcceptInfo(
    m3d::ui::Wnd* eventSrcWnd,
    m3d::ui::Wnd* eventDstWnd,
    ai::GeomRepositoryItem const& item) :
    m_eventSrcWnd(eventSrcWnd),
    m_eventDstWnd(eventDstWnd),
    m_item(item)
{
}

ItemAcceptInfo::ItemAcceptInfo(ItemAcceptInfo const&) = default;

// ===========================================================================
//  GeomSlot
// ===========================================================================

GeomSlot::AuxInfo::AuxInfo()
{
    m_unsuitableTexName = "ItemUncompatible";
    m_tooRichTexName = "ItemTooRich";
    m_icoSz.x = 15.0f;
    m_icoSz.y = 15.0f;
    m_space = 0.0f;
}

GeomSlot::GeomSlot()
{
    m3d::ui::ImageWnd::CreateImageWnd(BoundsBase<float>{}, m3d::rend::TexHandle{});
    SetDefaultFont(1);
    m_style = 768;  // WS_TEXT_CENTERED
    m_gsStyle = 0;

    if (!m_unsuitableTex.IsValid())
    {
        m_unsuitableTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_unsuitableTexName, 0);
    }
    if (!m_tooRichTex.IsValid())
    {
        m_tooRichTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_tooRichTexName, 0);
    }
    M3D_RENDERER->ReferenceTexture(m_unsuitableTex);
    M3D_RENDERER->ReferenceTexture(m_tooRichTex);
}

GeomSlot::~GeomSlot()
{
    M3D_RENDERER->ReleaseTexture(m_unsuitableTex);
    M3D_RENDERER->ReleaseTexture(m_tooRichTex);
}

void GeomSlot::SetDrawStyle(int drawStyle)
{
    m_gsStyle = drawStyle;
}

int GeomSlot::GetDrawStyle() const
{
    return m_gsStyle;
}

ai::GeomRepositoryItem& GeomSlot::GetItem()
{
    return m_item;
}

void GeomSlot::SetItem(ai::GeomRepositoryItem const& item)
{
    m_item = item;

    if (m3d::Object* parent = GetParent())
    {
        if (parent->IsKindOf(&DragDropItemsWnd::m_classDragDropItemsWnd))
        {
            auto* ddi = static_cast<DragDropItemsWnd*>(parent);
            SetBounds(ddi->GeomToWndBounds(item.GetBounds()), true);
            unsigned int const style = ddi->GetStyle();
            m_curClr = ((style & 2) != 0 || (style & 0x80000) != 0) ? 3u : 0u;
        }
    }

    SetImage(M3D_APP->m_pInterfaceManager->GetIcoByName(item.GetIcoName(), 0));
}

int GeomSlot::OnPaint(m3d::ui::DrawInfo const& di)
{
    m3d::ui::ImageWnd::OnPaint(di);

    m3d::ui::GfxServer* gfx = m_gfx;
    float const lineHeight = gfx->MeasureText(CStr("A"), m_defFont, m3d::TW_NOWRAP, 1000.0f).y;

    // (m_gsStyle & 4): stacked-resource amount, bottom-left.
    if ((m_gsStyle & 4) != 0 && m_item.IsValid() &&
        m_item.m_repositoryItemType == ai::GeomRepositoryItem::ITEMTYPE_RESOURCE && m_item.m_amount > 1)
    {
        unsigned int const amount =
            m_item.m_repositoryItemType != ai::GeomRepositoryItem::ITEMTYPE_RESOURCE ? 1u : m_item.m_amount;
        PointBase<float> const at{0.0f, di.m_clientRect.height - lineHeight};
        gfx->AddText(di, at, m_strTextColor + CStr(static_cast<int>(amount)), m_defFont, m3d::TW_NOWRAP, m3d::TF_LEFT);
    }

    // (m_gsStyle & 2): item name, centred.
    if ((m_gsStyle & 2) != 0)
    {
        CStr const name = m_item.GetIcoName();
        PointBase<float> const sz = gfx->MeasureText(name, m_defFont, m3d::TW_WORD_WRAP, m_bounds.width);
        if (m_bounds.height >= sz.y)
        {
            PointBase<float> const at{(di.m_clientRect.width - sz.x) * 0.5f, (di.m_clientRect.height - sz.y) * 0.5f};
            gfx->AddText(di, at, m_strTextColor + name, m_defFont, m3d::TW_WORD_WRAP, m3d::TF_LEFT);
        }
    }

    // (m_gsStyle & 1): "smart" price, top-right.
    if ((m_gsStyle & 1) != 0 && m_item.GetObjId() != -1)
    {
        // TODO(RVA 0x444950): draws help::GetPriceSmart(objId) here; that helper
        // routes through the (unported) ZnayuKakProdatWnd / town price tables.
    }

    // (m_gsStyle & 0x10 / 0x20): "unsuitable" / "too rich" corner badges.
    if ((m_gsStyle & 0x10) != 0)
    {
        BoundsBase<float> rc;
        rc.x0 = m_bounds.width - m_aif.m_space - m_aif.m_icoSz.x;
        rc.y0 = m_bounds.height - m_aif.m_space - m_aif.m_icoSz.y;
        rc.width = m_aif.m_icoSz.x;
        rc.height = m_aif.m_icoSz.y;
        gfx->AddImagedRect(di, rc, 0xFFFFFFFFu, m_unsuitableTex);
    }
    if ((m_gsStyle & 0x20) != 0)
    {
        BoundsBase<float> rc;
        rc.x0 = m_bounds.width - m_aif.m_space * 2.0f - m_aif.m_icoSz.x * 2.0f;
        rc.y0 = m_bounds.height - m_aif.m_space - m_aif.m_icoSz.y;
        rc.width = m_aif.m_icoSz.x;
        rc.height = m_aif.m_icoSz.y;
        gfx->AddImagedRect(di, rc, 0xFFFFFFFFu, m_tooRichTex);
    }

    return 1;
}

// ===========================================================================
//  DragSlot
// ===========================================================================

DragSlot::DragSlot(DragDropItemsWnd* srcItemsWnd) : m_srcItemsWnd(srcItemsWnd)
{
}

DragSlot::~DragSlot() = default;

int DragSlot::OnMouseButton0(unsigned int state, PointBase<float> const& at)
{
    if (state && !m_srcItemsWnd)
    {
        return m3d::ui::Wnd::OnMouseButton0(state, at);
    }
    DragDropItemsWnd::RemoveDragSlot();
    return 1;
}

int DragSlot::OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas)
{
    m3d::ui::Wnd::OnMouseMove(pt, deltas);

    PointBase<float> const parentPt = ToParent(pt);
    PointBase<float> screenPt = ToScreen(pt);

    m_bounds.x0 = parentPt.x - m_bounds.width * 0.5f;
    m_bounds.y0 = parentPt.y - m_bounds.height * 0.5f;

    M3D_APP->m_pInterfaceManager->LaunchEvent(91, GUI_EVENT_CUSTOM, &screenPt);
    return 1;
}

// ===========================================================================
//  DragDropItemsWnd
// ===========================================================================

DragDropItemsWnd::DragDropItemsWnd()
{
    m_dragStyle = DRAGSTYLE_HIDDEN_SRC;
}

DragDropItemsWnd::DragDropItemsWnd(DragDropItemsWnd const&) : DragDropItemsWnd()
{
}

DragDropItemsWnd::~DragDropItemsWnd() = default;

m3d::Class* DragDropItemsWnd::GetBaseClass()
{
    return &m3d::ui::Wnd::m_classWnd;
}

m3d::Class* DragDropItemsWnd::GetRtClass() const
{
    return &DragDropItemsWnd::m_classDragDropItemsWnd;
}

// --- statics -------------------------------------------------------------------

bool DragDropItemsWnd::IsDragging()
{
    return m_dragSlot != nullptr && m_dragSlot->IsChildOf(M3D_APP);
}

ai::GeomRepositoryItem DragDropItemsWnd::GetDragItem()
{
    if (m_dragSlot)
    {
        return m_dragSlot->GetItem();
    }
    return ai::GeomRepositoryItem();
}

void DragDropItemsWnd::RemoveDragSlot()
{
    if (m_dragSlot)
    {
        if (m_dragSlot->m_srcItemsWnd)
        {
            m_dragSlot->m_srcItemsWnd->OnDragRemove();
        }
        M3D_APP->m_pInterfaceManager->LaunchEvent(93, GUI_EVENT_CUSTOM, nullptr);
        m_dragSlot->DecRef();
    }
    m_dragSlot = nullptr;
}

// --- drag slot plumbing ------------------------------------------------------

int DragDropItemsWnd::CreateDragSlotFromWndPt(PointBase<float> const& wndPt)
{
    RemoveDragSlot();

    ai::GeomRepositoryItem item = GetItemFromOrigin(wndPt);
    if (!item.IsValid())
    {
        return 0;
    }

    m_dragSlot = new DragSlot(this);
    m_dragSlot->SetItem(item);
    UpdateDragSlotSize();
    UpdateDragSlotPosition(wndPt);
    return 1;
}

void DragDropItemsWnd::UpdateDragSlotSize()
{
    if (m_dragSlot)
    {
        m_dragSlot->SetBounds(GeomToWndBounds(m_dragSlot->GetItem().GetBounds()), true);
    }
}

void DragDropItemsWnd::UpdateDragSlotPosition(PointBase<float> const& mousePt)
{
    if (!m_dragSlot)
    {
        return;
    }
    PointBase<float> const pt = (m_dragSlot->GetParent() == GetParent()) ? ToParent(mousePt) : ToScreen(mousePt);
    BoundsBase<float> dragB = m_dragSlot->GetBounds();
    dragB.x0 = pt.x - dragB.width * 0.5f;
    dragB.y0 = pt.y - dragB.height * 0.5f;
    m_dragSlot->SetBounds(dragB, true);
}

int DragDropItemsWnd::StartDrag()
{
    if (!m_dragSlot || m_dragSlot->IsChildOf(M3D_APP))
    {
        return 0;
    }
    m3d::Object* parent = GetParent();
    if (!parent)
    {
        return 1;
    }

    // Re-parent the drag slot to the nearest modal ancestor, or the station.
    m3d::Object* host = parent;
    while (host && !host->IsKindOf(&m3d::ui::ModalWnd::m_classModalWnd))
    {
        host = host->GetParent();
    }
    if (!host)
    {
        host = static_cast<m3d::Object*>(M3D_APP);
    }
    host->AddChild(m_dragSlot);
    host->MoveChildToFirstPosition(m_dragSlot);

    if (host == parent)
    {
        m_dragSlot->SetBounds(m_dragSlot->ToParent(m_dragSlot->GetBounds()), true);
    }
    else
    {
        BoundsBase<float> const b = m_dragSlot->GetBounds();
        PointBase<float> const screenOrigin = ToScreen(PointBase<float>{b.x0, b.y0});
        m_dragSlot->SetBounds(BoundsBase<float>{screenOrigin.x, screenOrigin.y, b.width, b.height}, true);
    }

    if (m_dragStyle == DRAGSTYLE_HIDDEN_SRC)
    {
        HideDragSrc();
    }
    PlayStartDragSound();
    M3D_APP->m_pInterfaceManager->LaunchEvent(92, GUI_EVENT_CUSTOM, nullptr);
    return 1;
}

void DragDropItemsWnd::Drag(PointBase<float> const& mousePt)
{
    if (!m_dragSlot)
    {
        return;
    }
    if (!m_dragSlot->IsChildOf(M3D_APP))
    {
        StartDrag();
    }
    UpdateDragSlotPosition(mousePt);
    OnUpdateWhileDrag(mousePt);
}

int DragDropItemsWnd::Drop(PointBase<float> const& mousePt)
{
    if (!m_dragSlot)
    {
        return 0;
    }

    int result = 1;
    PlayDropSound();

    ItemAcceptInfo info(m_dragSlot->m_srcItemsWnd, this, m_dragSlot->GetItem());
    M3D_APP->m_pInterfaceManager->LaunchEvent(100, GUI_EVENT_CUSTOM, &info);
    if (CanAddDragItem(false))
    {
        M3D_APP->m_pInterfaceManager->LaunchEvent(97, GUI_EVENT_CUSTOM, &info);
    }
    else
    {
        result = 0;
    }

    RemoveDragSlot();
    OnUpdateWhileNoDrag(mousePt);
    return result;
}

// --- input -----------------------------------------------------------------

int DragDropItemsWnd::OnMouseButton0(unsigned int state, PointBase<float> const& at)
{
    if (state)
    {
        if (!m_dragSlot)
        {
            CreateDragSlotFromWndPt(at);
            return m3d::ui::Wnd::OnMouseButton0(state, at);
        }
    }
    else if (m_dragSlot)
    {
        if (!m_dragSlot->IsChildOf(M3D_APP))
        {
            Drag(at);
            return m3d::ui::Wnd::OnMouseButton0(0, at);
        }
        Drop(at);
    }
    return m3d::ui::Wnd::OnMouseButton0(state, at);
}

int DragDropItemsWnd::OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas)
{
    if (m_dragSlot)
    {
        if (m_dragSlot->IsChildOf(M3D_APP) || std::fabs(deltas.x) > 10.0f || std::fabs(deltas.y) > 10.0f)
        {
            Drag(pt);
        }
    }
    else
    {
        OnUpdateWhileNoDrag(pt);
    }
    return m3d::ui::Wnd::OnMouseMove(pt, deltas);
}

int DragDropItemsWnd::OnMouseIn()
{
    if (m_dragSlot)
    {
        m_dragSlot->SetStyle(m_dragSlot->GetStyle() | 0x100u);  // WS_TRANSPARENT: click through the slot
        M3D_APP->CaptureMouse(nullptr);
    }
    return m3d::ui::Wnd::OnMouseIn();
}

int DragDropItemsWnd::OnMouseOut()
{
    if (m_dragSlot)
    {
        PointBase<float> screenMouse{
            static_cast<float>(M3D_APP->GetMouseX()), static_cast<float>(M3D_APP->GetMouseY())};
        M3D_RENDERER->AbsToRel(screenMouse.x, screenMouse.y);

        PointBase<float> const wndOrigin = ToScreen(PointBase<float>{0.0f, 0.0f});
        PointBase<float> const local{screenMouse.x - wndOrigin.x, screenMouse.y - wndOrigin.y};
        PointBase<float> const parentPt = ToParent(local);

        if (parentPt.x < m_bounds.x0 || m_bounds.width + m_bounds.x0 <= parentPt.x || parentPt.y < m_bounds.y0 ||
            m_bounds.height + m_bounds.y0 <= parentPt.y)
        {
            if (!m_dragSlot->IsChildOf(M3D_APP))
            {
                StartDrag();
            }
            m_dragSlot->SetStyle(m_dragSlot->GetStyle() & ~0x100u);
            M3D_APP->CaptureMouse(m_dragSlot);
        }
    }
    OnDragOut();
    return m3d::ui::Wnd::OnMouseOut();
}

int DragDropItemsWnd::OnBeforeRemoveFromWndStation()
{
    int const res = m3d::ui::Wnd::OnBeforeRemoveFromWndStation();
    if (m_dragSlot && m_dragSlot->m_srcItemsWnd == this)
    {
        RemoveDragSlot();
    }
    return res;
}

int DragDropItemsWnd::OnDragRemove()
{
    if (!m_dragSlot)
    {
        return 0;
    }
    if (!m_dragSlot->IsChildOf(M3D_APP) || m_dragSlot->m_srcItemsWnd != this)
    {
        return 0;
    }
    if (m_dragStyle == DRAGSTYLE_HIDDEN_SRC)
    {
        ShowDragSrc();
    }
    return 1;
}

int DragDropItemsWnd::OnWndNotify(
    m3d::ui::Wnd* from,
    unsigned int idFrom,
    unsigned int message,
    m3d::AIParam const& data)
{
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (message != 8)
    {
        return 0;
    }

    PointBase<float> screenMouse{static_cast<float>(M3D_APP->GetMouseX()), static_cast<float>(M3D_APP->GetMouseY())};
    M3D_RENDERER->AbsToRel(screenMouse.x, screenMouse.y);

    PointBase<float> const wndOrigin = ToScreen(PointBase<float>{0.0f, 0.0f});
    PointBase<float> const local{screenMouse.x - wndOrigin.x, screenMouse.y - wndOrigin.y};
    PointBase<float> const parentPt = ToParent(local);

    if (parentPt.x < m_bounds.x0 || m_bounds.width + m_bounds.x0 <= parentPt.x || parentPt.y < m_bounds.y0 ||
        m_bounds.height + m_bounds.y0 <= parentPt.y)
    {
        OnMouseOut();
    }
    return 1;
}

int DragDropItemsWnd::GameDataUpdate(void* data, int dataType)
{
    switch (dataType)
    {
    case 91:  // mouse moved while dragging - clamp / capture inside bounds
    {
        if (!IsChildOf(M3D_APP) || !data || !m_dragSlot)
        {
            break;
        }
        m3d::ui::ModalWnd* topModal = GetStation()->GetTopModal();
        if (topModal && !IsChildOf(topModal))
        {
            break;
        }
        float const* xy = static_cast<float const*>(data);
        PointBase<float> const parentPt = ToParent(ToWindow(PointBase<float>{xy[0], xy[1]}));
        if (m_bounds.IsPtInBounds(parentPt))
        {
            m_dragSlot->SetStyle(m_dragSlot->GetStyle() | 0x100u);
            M3D_APP->CaptureMouse(nullptr);
        }
        break;
    }
    case 97:  // an item was accepted by a drop target
        if (data)
        {
            ItemAcceptInfo info(*static_cast<ItemAcceptInfo*>(data));
            if (info.m_eventDstWnd == this && GiveUpItem(info.m_item, info.m_eventSrcWnd))
            {
                ItemAcceptInfo out(this, info.m_eventSrcWnd, info.m_item);
                M3D_APP->m_pInterfaceManager->LaunchEvent(98, GUI_EVENT_CUSTOM, &out);
                return 1;
            }
        }
        break;
    case 98:  // finalise the accepted item on the destination
        if (data)
        {
            ItemAcceptInfo info(*static_cast<ItemAcceptInfo*>(data));
            if (info.m_eventDstWnd == this)
            {
                AddItem(info.m_item);
                return 1;
            }
        }
        break;
    default:
        return 0;
    }
    return 1;
}

// --- sounds -------------------------------------------------------------------

void DragDropItemsWnd::PlayStartDragSound()
{
    bool bLooped = false;
    m_gfx->PlayControlSound(CStr("SOUND_START_DRAG_ITEM"), &bLooped);
}

void DragDropItemsWnd::PlayDropSound()
{
    bool bLooped = false;
    m_gfx->PlayControlSound(CStr("SOUND_DROP_ITEM"), &bLooped);
}

void DragDropItemsWnd::Enable(bool needEnable)
{
    if (needEnable)
    {
        m_style &= ~2u;
        m_curClr = 0;
    }
    else
    {
        m_style |= 2u;
        m_curClr = 3;
    }
}
