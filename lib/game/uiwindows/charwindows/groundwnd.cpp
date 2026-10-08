#include "groundwnd.h"
#include "repositorywnd.h"
#include <core/kernel.h>
#include <core/log.h>
#include <i_event.h>
#include <game/m3dgame.h>
#include "server/geomrepository.h"
#include "server/objects/vehicle.h"
#include <server/objects/player.h>
#include <ui/button.h>
#include <ui/image.h>

RT_CLASS_EXPORTS_BEGIN(GroundWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GroundWnd);

GroundWnd::AuxInfo::AuxInfo()
{
    m_wndPictureName = "wndPicture";
    m_wndRepositoryName = "wndRepository";
    m_btnPickUpAllName = "btnPickUpAll";
    m_wndGroundPictureName = "wndGroundPicture";
    m_groundPictureTexName = "InventoryGround";
}

m3d::Class* GroundWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* GroundWnd::GetClass() const
{
    return RT_CLASS_LOCAL(GroundWnd);
}

m3d::Object* GroundWnd::CreateObject()
{
    return new GroundWnd;
}

m3d::Object* GroundWnd::Clone()
{
    return new GroundWnd(*this);
}

GroundWnd::GroundWnd()
{
    m_btnPickUpAll = nullptr;
    m_wndGroundPicture = nullptr;
}

GroundWnd::GroundWnd(GroundWnd const&) : GroundWnd()
{
}

GroundWnd::~GroundWnd() = default;

// ---------------------------------------------------------------------------

ai::GeomRepository* GroundWnd::GetGroundRepository() const
{
    auto* vehicle = GetVehicle();
    return vehicle ? vehicle->GetGroundRepository() : nullptr;
}

void GroundWnd::SetVehicleId(int vehicleId)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ChildPanel::SetVehicleId(vehicleId);
    if (IsChildOf(M3D_APP))
    {
        SetupRepository();
    }
}

void GroundWnd::SetupRepository()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (auto* vehicle = GetVehicle())
    {
        vehicle->CollectNearbyObjectsToGroundRepository();
    }
    m_wndRepository->SetRepository(GetGroundRepository(), RepositoryWnd::REPOSITORYTYPE_GROUND);
    m_wndRepository->SetItemDrawStyleByResourceId(4, -1);
}

void GroundWnd::UpdatePickupButtonState()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::GeomRepository* groundRepository = GetGroundRepository();
    ai::IzvratRepository* playerRepository = ai::thePlayer ? ai::thePlayer->GetRepository() : nullptr;
    const bool hasItems = groundRepository && groundRepository->GetNumItems() != 0 && playerRepository != nullptr;
    m_btnPickUpAll->EnableWindow(hasItems);
}

void GroundWnd::UpdateGroundPicture()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndGroundPicture->SetImage(M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_groundPictureTexName, 0));
}

void GroundWnd::OnStartLevel()
{
    UpdateGroundPicture();
}

void GroundWnd::OnRepositoryChanged(void* data)
{
    if (!data)
    {
        return;
    }
    ai::GeomRepository* groundRepository = GetGroundRepository();
    if (groundRepository &&
        groundRepository == reinterpret_cast<ai::GeomRepository*>(static_cast<const m3d::Event*>(data)->m_intEv[0]))
    {
        UpdatePickupButtonState();
    }
}

void GroundWnd::PickUpAll()
{
    if ((m_gameDataFlags & 1) == 0 || !m_btnPickUpAll->IsEnabled())
    {
        return;
    }
    if (!ai::thePlayer)
    {
        return;
    }
    auto* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }

    unsigned int originalNumItems = 0;
    retruxx::vector<int> addedObjIds;
    vehicle->PickUpNearbyObjects(false, originalNumItems, addedObjIds);
    if (originalNumItems != addedObjIds.size())
    {
        M3D_APP->RunMsgBoxDlg("", M3D_APP->GetStringByStringId0("NotEnoughSpace"), 1u, false);
    }
}

int GroundWnd::GameDataClear(bool)
{
    SetVehicleId(-1);
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndRepository->SetRepository(nullptr, RepositoryWnd::REPOSITORYTYPE_PLAYER);
    }
    return 1;
}

int GroundWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    if (dataType == 58)
    {
        if (IsChildOf(M3D_APP) && data)
        {
            OnRepositoryChanged(data);
        }
        return 1;
    }
    if (dataType == 64)
    {
        auto* playerVehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
        SetVehicleId(playerVehicle ? playerVehicle->GetId() : -1);
        return 1;
    }
    if (dataType == 85)
    {
        OnStartLevel();
    }
    return 1;
}

int GroundWnd::GameDataSetup()
{
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        auto* child = GetChildByName(m_aif.m_btnPickUpAllName);
        if (child && child->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnPickUpAll = static_cast<m3d::ui::ButtonWnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnPickUpAllName + " is not found or incorrect type");
            res = 0;
        }

        child = GetChildByName(m_aif.m_wndGroundPictureName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
        {
            m_wndGroundPicture = static_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndGroundPictureName +
                         " is not found or incorrect type");
            res = 0;
        }

        auto* repositoryPattern = GetChildByName(m_aif.m_wndRepositoryName);
        if (repositoryPattern && repositoryPattern->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndRepository = static_cast<RepositoryWnd*>(M3D_KERNEL->New("RepositoryWnd"));
            if (!m_wndRepository)
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndRepositoryName +
                             " - cannot find rtti class RepositoryWnd");
                res = 0;
            }
            else if (!m_wndRepository->CreateFromPattern(static_cast<m3d::ui::Wnd*>(repositoryPattern), true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndRepositoryName + " from pattern class");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndRepositoryName + " is not found or incorrect type");
            res = 0;
        }

        if (m_wndRepository)
        {
            res &= M3D_APP->m_pInterfaceManager->AddWindowById(m_wndRepository.get(), 10, true, false);
        }
        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("GroundWnd: error - fail to init because of a bad resource");
    return 0;
}

int GroundWnd::OnAfterAddToWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

int GroundWnd::OnAfterRemoveFromWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    if ((m_gameDataFlags & 1) != 0)
    {
        ai::GeomRepository* groundRepository = GetGroundRepository();

        CVector pos = ZeroVector;
        if (ai::thePlayer)
        {
            auto* playerVehicle = ai::thePlayer->GetVehicle();
            if (playerVehicle)
            {
                pos = playerVehicle->GetPosition();
            }
        }
        if (groundRepository)
        {
            groundRepository->FlushInReferenceChests(pos);
        }
    }
    return res;
}

int GroundWnd::OnBeforeAddToWndStation()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        SetupRepository();
    }
    return Wnd::OnBeforeAddToWndStation();
}

int GroundWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, idFrom, message, data))
    {
        return 1;
    }
    if (idFrom == 200 && message == 1)
    {
        PickUpAll();
        return 1;
    }
    return 0;
}
