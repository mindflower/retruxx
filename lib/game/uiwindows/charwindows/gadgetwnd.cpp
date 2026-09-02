#include "gadgetwnd.h"
#include "iteminfownd.h"

#include <core/aiparam.h>
#include <i_event.h>
#include <game/m3dgame.h>
#include <game/uimisc/guihelper.h>

#include <ui/frame.h>
#include <ui/image.h>
#include <ui/ui_srv.h>

#include <server/resourcemanager.h>
#include <server/geomrepositoryitem.h>
#include <server/objects/vehicle.h>
#include <server/objects/gadget.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/base/objcontainer.h>

#include <cstring>

RT_CLASS_EXPORTS_BEGIN(GadgetWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GadgetWnd);

namespace
{
    // The "dim / disabled" style bits pick colour index 3 (grey) for every draw.
    unsigned int GadgetDrawColor(unsigned int style, unsigned int curClr)
    {
        return ((style & 2) != 0 || (style & 0x80000) != 0) ? 3u : curClr;
    }
}  // namespace

GadgetWnd::GadgetWnd()
{
    m_resourceName = "GADGET";
    m_slotId = -1;
    m_itemIcoBounds.x0 = 0.0f;
    m_itemIcoBounds.y0 = 0.0f;
    m_itemIcoBounds.width = 0.0f;
    m_itemIcoBounds.height = 0.0f;
    m_aif.m_suitableDistantPane = "PaneSlotSuitableThin";
}

GadgetWnd::GadgetWnd(GadgetWnd const&) : GadgetWnd()
{
}

GadgetWnd::~GadgetWnd() = default;

m3d::Object* GadgetWnd::Clone()
{
    return new GadgetWnd(*this);
}

m3d::Object* GadgetWnd::CreateObject()
{
    return new GadgetWnd;
}

m3d::Class* GadgetWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

m3d::Class* GadgetWnd::GetClass() const
{
    return RT_CLASS_LOCAL(GadgetWnd);
}

// --- slot / resource --------------------------------------------------------

int GadgetWnd::GetSlotId() const
{
    return m_slotId;
}

void GadgetWnd::SetSlotId(int slotId)
{
    m_slotId = slotId;
    OnItemChanged();
}

CStr const& GadgetWnd::GetResourceName() const
{
    return m_resourceName;
}

int GadgetWnd::GetResourceId() const
{
    return ai::theResourceManager ? ai::theResourceManager->GetResourceId(m_resourceName) : -1;
}

void GadgetWnd::SetResourceName(CStr const& resourceName)
{
    m_resourceName = resourceName;
    m_geomSize = GetCorrectGeomSize();
    UpdateItemIco();
}

BoundsBase<float> GadgetWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        return m_itemIcoBounds;
    }
    return BoundsBase<float>{};
}

// --- validity / info ------------------------------------------------------

bool GadgetWnd::IsValid() const
{
    if (!GetVehicle())
    {
        return false;
    }
    if (!IsEmpty() && !GetItem())
    {
        return false;
    }
    return m_resourceName.c_str() != nullptr && m_resourceName.length() != 0 && m_slotId != -1;
}

bool GadgetWnd::CanContainItem() const
{
    ai::Vehicle* vehicle = GetVehicle();
    return vehicle != nullptr && vehicle->GetMaxGadgets(m_resourceName) > 0;
}

bool GadgetWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    return false;
}

void GadgetWnd::SetupInfoWnd(ItemInfoWnd* infoWnd)
{
    if (infoWnd && IsValid())
    {
        IncRef();
        infoWnd->SetUpForObject(m_itemId, this);
    }
}

// --- item value / events ------------------------------------------------------

void GadgetWnd::UpdateItemValue()
{
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        m_itemId = -1;
        return;
    }
    auto const& gadgets = vehicle->GetGadgets();
    auto it = gadgets.find(m_slotId);
    m_itemId = (it != gadgets.end() && it->second) ? it->second->GetId() : -1;
}

bool GadgetWnd::SetItemObjId(int objId)
{
    m_itemId = objId;
    if (objId >= 0)
    {
        ai::Obj* item = ai::theObjects->GetEntityByObjId(objId);
        if (item)
        {
            if (!item->IsKindOf(&ai::Gadget::m_classGadget))
            {
                m_itemId = -1;
                return false;
            }
            // TODO(RVA 0x4457F0): the shipped build also stamps the gadget's own
            // slot field with m_slotId here; ai::Gadget exposes no accessor for
            // it in retruxx yet.
            if (ai::Vehicle* vehicle = GetVehicle())
            {
                vehicle->AddGadget(static_cast<ai::Gadget*>(item));
            }
        }
    }
    return true;
}

int GadgetWnd::GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd* targetWnd)
{
    if (m_itemId < 0)
    {
        return 0;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_itemId);
    if (!obj || !obj->IsKindOf(&ai::Gadget::m_classGadget))
    {
        return 0;
    }

    DeactivateItem();
    if (ai::Vehicle* vehicle = GetVehicle())
    {
        vehicle->RemoveChild(obj);
    }

    if (targetWnd && targetWnd->IsKindOf(&RepositoryWnd::m_classRepositoryWnd) &&
        static_cast<RepositoryWnd*>(targetWnd)->GetRepositoryType() == RepositoryWnd::REPOSITORYTYPE_SHOP)
    {
        ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
        if (town)
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

// --- drawing -------------------------------------------------------------------

void GadgetWnd::DrawBackground(m3d::ui::DrawInfo const& di)
{
    if (!m_itemIco.IsValid())
    {
        return;
    }
    BoundsBase<float> const rect{0.0f, 0.0f, m_bounds.width, m_bounds.height};
    m_gfx->AddImagedRect(di, rect, GadgetDrawColor(m_style, m_curClr), m_itemIco);
}

void GadgetWnd::DrawItemIco(m3d::ui::DrawInfo const& di)
{
    if (IsEmpty())
    {
        return;
    }
    m_gfx->AddImagedRect(di, m_itemIcoBounds, GadgetDrawColor(m_style, m_curClr), m_itemIco);
}

void GadgetWnd::DrawSelection(m3d::ui::DrawInfo const& di)
{
    if (m_state > SLOTSTATE_UNSELECTED && m_state <= SLOTSTATE_OWN_PLACE_IN_HIDDEN_MODE)
    {
        m_gfx->AddImagedRect(di, m_itemIcoBounds, GadgetDrawColor(m_style, m_curClr), m_cellTextures[m_state]);
    }
}

void GadgetWnd::DrawFrame(m3d::ui::DrawInfo const& di)
{
    float barW = 0.0f;
    if (m3d::ui::Pane* pane = m_gfx->GetPane(m_aif.m_suitableDistantPane))
    {
        if (m3d::ui::Frame* frame = pane->m_frame[0])
        {
            barW = static_cast<float>(frame->m_barUsedWidth);
        }
    }

    BoundsBase<float> frameB;
    frameB.x0 = m_itemIcoBounds.x0 - barW;
    frameB.y0 = m_itemIcoBounds.y0 - barW;
    frameB.width = barW * 2.0f + m_itemIcoBounds.width;
    frameB.height = barW * 2.0f + m_itemIcoBounds.height;

    m_gfx->AddFlatAxialPane0(
        di, frameB, GadgetDrawColor(m_style, m_curClr), 6, m_aif.m_suitableDistantPane, m3d::ui::PANE_FLAG_BG_OUT);
}

int GadgetWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    DrawBackground(di);
    DrawSelection(di);
    if (m_dragStyle != DragDropItemsWnd::DRAGSTYLE_HIDDEN_SRC || !m_isDragging)
    {
        DrawItemIco(di);
        DrawItemInfo(di);
    }
    if (m_state == SLOTSTATE_SUITABLE_DISTANT || m_state == SLOTSTATE_SUITABLE ||
        m_state == SLOTSTATE_OWN_PLACE_IN_HIDDEN_MODE)
    {
        DrawFrame(di);
    }
    return 1;
}

// --- misc ------------------------------------------------------------------

void GadgetWnd::LaunchEventOnItemDeactivation() const
{
    M3D_APP->ImmediateMessage(65669, m_vehicleId, m_slotId, 0, 0, CStr(), m3d::AIParam());
}

int GadgetWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 69)
    {
        if (data && IsChildOf(M3D_APP))
        {
            auto const* evt = static_cast<m3d::Event const*>(data);
            if (m_vehicleId == evt->m_intEv[0] && evt->m_intEv[1] == m_slotId)
            {
                OnItemChanged();
            }
        }
        return 1;
    }
    ItemWnd::GameDataUpdate(data, dataType);
    return 1;
}

int GadgetWnd::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    if (!patternWnd)
    {
        return 0;
    }

    if (!Create(CStr(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        return 0;
    }

    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetBackground(patternWnd->GetBackground());

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        return 0;
    }

    // The icon pattern shares the background pattern's name with "Bg" swapped
    // for "Ico" ("wndGadgetCommonBg03" -> "wndGadgetCommonIco03").
    CStr const patternName = patternWnd->GetName();
    char const* bgPos = patternName.c_str() ? std::strstr(patternName.c_str(), "Bg") : nullptr;
    int const prefixLen = bgPos ? static_cast<int>(bgPos - patternName.c_str()) : -1;
    int const nameLen = patternName.c_str() ? static_cast<int>(std::strlen(patternName.c_str())) : 0;
    CStr const wndIcoName = patternName.substr(0, prefixLen) + CStr("Ico") + patternName.substr(nameLen - 2, 2);

    m3d::Object* icoChild = parent->GetChildByName(wndIcoName);
    if (!icoChild || !icoChild->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
    {
        return 0;
    }

    m_itemIcoBounds = static_cast<m3d::ui::Wnd*>(icoChild)->GetBounds();
    BoundsBase<float> const screenB = static_cast<m3d::ui::Wnd*>(parent)->ToScreen(m_itemIcoBounds);
    m_itemIcoBounds = patternWnd->ToWindow(screenB);

    if (deleteSrc)
    {
        const_cast<m3d::ui::Wnd*>(patternWnd)->DecRef();
        icoChild->DecRef();
    }

    m_gameDataFlags |= 1u;
    return 1;
}
