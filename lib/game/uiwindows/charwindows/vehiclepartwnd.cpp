#include "vehiclepartwnd.h"

#include <cmath>

#include "config.h"
#include "core/kernel.h"
#include "game/m3dgame.h"
#include "game/uimisc/guihelper.h"
#include "iteminfownd.h"
#include "ui/ui_srv.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypeinfo.h"
#include "server/objects/basket.h"
#include "server/objects/cabin.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/vehicle.h"
#include "server/resourcemanager.h"

RT_CLASS_EXPORTS_BEGIN(VehiclePartWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehiclePartWnd);

VehiclePartWnd::VPAuxInfo VehiclePartWnd::m_vpAif;
m3d::rend::TexHandle VehiclePartWnd::m_gunAngleTex;

VehiclePartWnd::VPAuxInfo::VPAuxInfo()
{
    m_gunAngleTexName = "GunAngle";
}

VehiclePartWnd::VehiclePartWnd()
{
    // RVA 0x75FF0 - the pattern cell textures ("Cell" plus four
    // gun-suitability variants) mirror ItemWnd::AuxInfo's own cell names.
    m_aif.m_cellTexNames[0] = "Cell";
    m_aif.m_cellTexNames[1] = "GunCellSelected";
    m_aif.m_cellTexNames[2] = "GunCellSuitable";
    m_aif.m_cellTexNames[3] = "GunCellUnsuitable";
    m_aif.m_cellTexNames[4] = "GunCellSuitable";

    if (!m_gunAngleTex.IsValid())
    {
        m_gunAngleTex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_vpAif.m_gunAngleTexName, 0);
    }
    M3D_RENDERER->ReferenceTexture(m_gunAngleTex);
}

VehiclePartWnd::VehiclePartWnd(VehiclePartWnd const&) : VehiclePartWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x76230) only default-constructs the
    // base and m_partName/m_gunAngleEdges, skipping the cell-texture-name and
    // gun-angle-texture setup that the default ctor performs; delegating here
    // is a superset of that (harmless - it's the same aux data and a ref that
    // gets released again in the destructor either way) rather than
    // duplicating a second, divergent ctor body.
}

VehiclePartWnd::~VehiclePartWnd()
{
    // RVA 0x76290
    M3D_RENDERER->ReleaseTexture(m_gunAngleTex);
}

m3d::Object* VehiclePartWnd::Clone()
{
    return new VehiclePartWnd(*this);
}

m3d::Object* VehiclePartWnd::CreateObject()
{
    return new VehiclePartWnd;
}

m3d::Class* VehiclePartWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

m3d::Class* VehiclePartWnd::GetClass() const
{
    return RT_CLASS_LOCAL(VehiclePartWnd);
}

void VehiclePartWnd::SetPartName(CStr const& partName)
{
    // RVA 0x76330
    m_partName = partName;
    m_geomSize = GetCorrectGeomSize();
    UpdateItemIco();
    UpdateBg();
    GetGunAngleEdges(m_gunAngleEdges);
}

void VehiclePartWnd::SetVehicleId(int vehicleId)
{
    // RVA 0x76E90
    ItemWnd::SetVehicleId(vehicleId);
    if (GetVehicle())
    {
        OnItemChanged();
    }
    else
    {
        Reset();
    }
    GetGunAngleEdges(m_gunAngleEdges);
}

void VehiclePartWnd::UpdateItemValue()
{
    // RVA 0x763F0
    ai::Vehicle* vehicle = GetVehicle();
    ai::VehiclePart* part = vehicle ? vehicle->GetPartByName(m_partName) : nullptr;
    m_itemId = part ? part->GetId() : -1;
}

bool VehiclePartWnd::IsValid() const
{
    // RVA 0x76540
    return GetVehicle() && (IsEmpty() || GetItem()) && !m_partName.empty();
}

bool VehiclePartWnd::CanContainItem() const
{
    // RVA 0x765A0
    ai::Vehicle* vehicle = GetVehicle();
    return !vehicle || vehicle->CanPartBeAttached(m_partName);
}

bool VehiclePartWnd::CanChangeItem() const
{
    // RVA 0x76710 - the cabin and basket slots are managed by their own
    // CBWnd-family windows, not by drag/drop on the vehicle-part grid.
    int const resourceId = GetResourceId();
    int const cabinResourceId = ai::theResourceManager->GetResourceId("CABIN");
    if (ai::theResourceManager->bResourceIsKindOf(resourceId, cabinResourceId))
    {
        return false;
    }
    int const basketResourceId = ai::theResourceManager->GetResourceId("BASKET");
    return !ai::theResourceManager->bResourceIsKindOf(resourceId, basketResourceId);
}

bool VehiclePartWnd::NeedUpdateInfoWnd(ItemInfoWnd* infoWnd) const
{
    // RVA 0x76610
    return infoWnd && IsValid() && infoWnd->GetObjectA() != GetItem();
}

void VehiclePartWnd::SetupInfoWnd(ItemInfoWnd* infoWnd)
{
    // RVA 0x765D0
    if (infoWnd && IsValid())
    {
        IncRef();
        infoWnd->SetUpForObject(m_itemId, this);
    }
}

void VehiclePartWnd::LaunchEventOnItemDeactivation() const
{
    // RVA 0x76650
    M3D_APP->ImmediateMessage(65670, m_vehicleId, 0, 0, 0, m_partName, m3d::AIParam());
}

int VehiclePartWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x76390
    if (dataType == 65)
    {
        if (data)
        {
            auto const* evt = static_cast<m3d::Event const*>(data);
            if (m_vehicleId == evt->m_intEv[0] && CStr::my_strcmp(m_partName.c_str(), evt->m_strEv.c_str()) == 0)
            {
                OnItemChanged();
            }
        }
        return 1;
    }
    return ItemWnd::GameDataUpdate(data, dataType);
}

bool VehiclePartWnd::SetItemObjId(int objId)
{
    // RVA 0x76490
    m_itemId = objId;
    bool ok = true;
    if (objId >= 0)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
        if (obj && !obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
        {
            m_itemId = -1;
            ok = false;
        }
    }

    ai::Vehicle* vehicle = GetVehicle();
    if (vehicle && (m_itemId != -1 || m_partName != "BASKET"))
    {
        vehicle->SetPartByName(m_partName, static_cast<ai::VehiclePart*>(GetItem()), false);
    }
    return ok;
}

int VehiclePartWnd::OnMouseButton1(unsigned state, PointBase<float> const& at)
{
    // RVA 0x766B0 - the cabin/basket slots forward straight to the base Wnd
    // (no drag/drop) since CanChangeItem() already excludes them.
    if (m_partName == "CABIN" || m_partName == "BASKET")
    {
        return m3d::ui::Wnd::OnMouseButton1(state, at);
    }
    return ItemWnd::OnMouseButton1(state, at);
}

int VehiclePartWnd::GetResourceId() const
{
    // RVA 0x76430
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return -1;
    }
    auto const* partDesc = vehicle->GetPrototypeInfo()->GetPartDescriptionByName(m_partName);
    return partDesc ? partDesc->GetPartResourceId() : -1;
}

void VehiclePartWnd::UpdateBg()
{
    // RVA 0x76810
    CStr const resourceName = ai::theResourceManager->GetResourceName(GetResourceId());
    SetBackground(M3D_APP->m_pInterfaceManager->GetIcoByName(resourceName, 0));
}

void VehiclePartWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    // RVA 0x76800 - empty in the shipped game.
}

int VehiclePartWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x76870
    DrawFrame(di);
    DrawBg(di);
    DrawGrid(di);
    DrawSelection(di);
    DrawGunAngleEdges(di);
    if (m_dragStyle != DragDropItemsWnd::DRAGSTYLE_HIDDEN_SRC || !m_isDragging)
    {
        DrawItemIco(di);
        DrawItemInfo(di);
    }
    return 1;
}

void VehiclePartWnd::UpdateGunAngleEdges()
{
    // RVA 0x76A10
    GetGunAngleEdges(m_gunAngleEdges);
}

void VehiclePartWnd::GetGunAngleEdges(retruxx::set<m3d::ui::Edges>& edges) const
{
    // RVA 0x76A20
    edges.clear();
    if (m_partName.empty())
    {
        return;
    }
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }
    auto const* partDesc = vehicle->GetPrototypeInfo()->GetPartDescriptionByName(m_partName);
    if (!partDesc)
    {
        return;
    }

    unsigned const numLps = partDesc->GetNumLps();
    if (numLps > 0)
    {
        // Maps a normalised [0, 2*pi) angle to the compass quadrant it falls
        // in, with the quadrant boundaries offset by 45 degrees so each
        // quadrant is centred on its cardinal angle.
        auto quadrantOf = [](float angle)
        {
            int const q = static_cast<int>((angle + 2.3561945f) * 0.63661975f);
            return ((q % 4) + 4) % 4;
        };

        for (unsigned index = 0; index < numLps; ++index)
        {
            float leftStopAngle = 0.0f;
            float rightStopAngle = 0.0f;
            vehicle->GetGunHorizontalStopAngles(m_partName, index, leftStopAngle, rightStopAngle);

            if (rightStopAngle - leftStopAngle > 6.2831855f)
            {
                // Unlimited traverse - every edge is relevant, and no other
                // lp's range can narrow that down.
                edges.insert(m3d::ui::LEFT);
                edges.insert(m3d::ui::TOP);
                edges.insert(m3d::ui::RIGHT);
                edges.insert(m3d::ui::BOTTOM);
                return;
            }

            int const leftEdge = quadrantOf(help::Angle0To2Pi(leftStopAngle));
            int const rightEdge = quadrantOf(help::Angle0To2Pi(rightStopAngle));
            if (leftEdge == rightEdge)
            {
                if (help::AngleMinusPiToPi(leftStopAngle) > help::AngleMinusPiToPi(rightStopAngle))
                {
                    // The arc actually wraps almost all the way around despite
                    // landing in the same quadrant bucket.
                    edges.insert(m3d::ui::LEFT);
                    edges.insert(m3d::ui::TOP);
                    edges.insert(m3d::ui::RIGHT);
                    edges.insert(m3d::ui::BOTTOM);
                }
                else
                {
                    edges.insert(static_cast<m3d::ui::Edges>(leftEdge));
                }
            }
            else
            {
                for (int edge = leftEdge;; edge = (edge + 1) & 3)
                {
                    edges.insert(static_cast<m3d::ui::Edges>(edge));
                    if (edge == rightEdge)
                    {
                        break;
                    }
                }
            }
        }
    }

    // The edges were computed in the gun's own coordinate frame; mirror
    // LEFT/RIGHT to match the screen-facing orientation the UI shows.
    retruxx::set<m3d::ui::Edges> mirrored;
    for (m3d::ui::Edges edge : edges)
    {
        if (edge == m3d::ui::LEFT)
        {
            mirrored.insert(m3d::ui::RIGHT);
        }
        else if (edge == m3d::ui::RIGHT)
        {
            mirrored.insert(m3d::ui::LEFT);
        }
        else
        {
            mirrored.insert(edge);
        }
    }
    edges = std::move(mirrored);
}

void VehiclePartWnd::DrawGunAngleEdges(m3d::ui::DrawInfo const& di)
{
    // RVA 0x768E0
    if (m_gunAngleEdges.empty())
    {
        return;
    }

    M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    // NOTE: the shipped build sets stage 1's BM_COLOR state twice here (the
    // second call should plausibly have been BM_ALPHA, mirroring the stage-0
    // pair above); preserved as shipped.
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    M3D_RENDERER->PushZbState(m3d::rend::ZB_NOWRITE);
    M3D_RENDERER->PushBlend(m3d::rend::BM_NONE);

    for (m3d::ui::Edges edge : m_gunAngleEdges)
    {
        DrawGunAngleEdge(edge, di);
    }

    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->PopBlend();
    M3D_RENDERER->PopZbState();
}

void VehiclePartWnd::DrawGunAngleEdge(m3d::ui::Edges edge, m3d::ui::DrawInfo const& di)
{
    // RVA 0x76D50
    if (m_gunAngleTex.IsValid())
    {
        M3D_RENDERER->SetTexture(0, m_gunAngleTex, -1.0);
    }
    else
    {
        M3D_RENDERER->SetWhiteTexture(0);
    }

    float const x0 = di.m_clientRect.x0;
    float const y0 = di.m_clientRect.y0;
    float const width = di.m_clientRect.width;
    float const height = di.m_clientRect.height;

    float halfW = 0.0f;
    float halfH = 0.0f;
    switch (edge)
    {
    case m3d::ui::LEFT:
    case m3d::ui::RIGHT:
        halfW = height * 0.5f;
        halfH = width * 0.25f;
        break;
    case m3d::ui::TOP:
    case m3d::ui::BOTTOM:
        halfW = width * 0.5f;
        halfH = height * 0.25f;
        break;
    default:
        break;
    }

    // One quarter-turn per edge, going LEFT -> TOP -> RIGHT -> BOTTOM.
    float const angleByEdge[4] = {-1.5707964f, 0.0f, 1.5707964f, 3.1415927f};

    M3D_APP->PutSpriteRelRot(
        (x0 * 2.0f + width) * 0.5f,
        (y0 * 2.0f + height) * 0.5f - halfH,
        halfW,
        halfH,
        0xFFFFFFFF,
        angleByEdge[edge],
        0.0f,
        halfH,
        0.0f,
        0.0f,
        0.0f);
}
