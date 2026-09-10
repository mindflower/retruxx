#include "repositoryitemwnd.h"

#include "iteminfownd.h"

#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/geomrepository.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/player.h>
#include <server/objects/town.h>

RT_CLASS_EXPORTS_BEGIN(RepositoryItemWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepositoryItemWnd);

m3d::Class* RepositoryItemWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemWnd);
}

m3d::Class* RepositoryItemWnd::GetClass() const
{
    return RT_CLASS_LOCAL(RepositoryItemWnd);
}

m3d::Object* RepositoryItemWnd::CreateObject()
{
    return new RepositoryItemWnd;
}

m3d::Object* RepositoryItemWnd::Clone()
{
    // RVA 0x469F00 - a fresh default-constructed window, not a copy of *this.
    return new RepositoryItemWnd;
}

RepositoryItemWnd::RepositoryItemWnd() : m_repository(nullptr)
{
    // RVA 0x469F90
}

RepositoryItemWnd::RepositoryItemWnd(RepositoryItemWnd const&) :
    // NOTE: the shipped copy ctor (RVA 0x469FE0) chains to the ItemWnd default
    // ctor and stops, leaving m_repository uninitialised rather than copying or
    // nulling it. Nulled here so nothing can read garbage.
    m_repository(nullptr)
{
}

RepositoryItemWnd::~RepositoryItemWnd() = default;

void RepositoryItemWnd::SetUpForRepositoryItem(ai::GeomRepositoryItem const& repositoryItem)
{
    // RVA 0x46A260 - the window borrows the repository the item came from, then
    // binds itself to that item's object.
    m_repository = repositoryItem.m_parentRepository;
    SetItemObjId(repositoryItem.GetObjId());
}

void RepositoryItemWnd::SetVehicleId(int)
{
    // RVA 0x46A010 - a repository item is not attached to any vehicle, so the
    // base class' notion of a vehicle id is deliberately ignored.
}

int RepositoryItemWnd::CanAddDragItem(bool)
{
    // RVA 0x46A020 - this window never accepts a drop.
    return 0;
}

int RepositoryItemWnd::AddItem(ai::GeomRepositoryItem const&)
{
    // RVA 0x46A230
    return 0;
}

bool RepositoryItemWnd::CanChangeItem() const
{
    // RVA 0x46A380 - the slot is read-only, which is what stops GiveUpItem from
    // ever running in the shipped build.
    return false;
}

int RepositoryItemWnd::DeactivateItem()
{
    // RVA 0x46A240
    return 1;
}

int RepositoryItemWnd::ActivateItem(ai::GeomRepositoryItem const&)
{
    // RVA 0x46A250
    return 1;
}

bool RepositoryItemWnd::NeedUpdateInfoWnd(ItemInfoWnd*) const
{
    // RVA 0x46A340 - the contents never change, so the info window never needs
    // refreshing once it has been set up.
    return false;
}

void RepositoryItemWnd::LaunchEventOnItemDeactivation() const
{
    // RVA 0x46A350 - deliberately silent.
}

void RepositoryItemWnd::UpdateItemValue()
{
    // RVA 0x46A360 - deliberately empty.
}

int RepositoryItemWnd::OnMouseButton1(unsigned int state, PointBase<float> const& at)
{
    // RVA 0x46A370 - note this reaches past ItemWnd straight to the plain Wnd
    // handler, so none of ItemWnd's click / drag-start behaviour runs here.
    return m3d::ui::Wnd::OnMouseButton1(state, at);
}

bool RepositoryItemWnd::IsValid() const
{
    // RVA 0x46A280
    if (m_itemId == -1 || !ai::theObjects->GetEntityByObjId(m_itemId))
    {
        return false;
    }
    return m_repository != nullptr;
}

ai::GeomRepositoryItem RepositoryItemWnd::GetAsRepositoryItem() const
{
    // RVA 0x46A030
    if (!m_repository || m_itemId == -1 || !ai::theObjects->GetEntityByObjId(m_itemId))
    {
        return {};
    }
    return m_repository->GetItem(m_repository->GetSlotByObjId(m_itemId));
}

bool RepositoryItemWnd::SetItemObjId(int objId)
{
    // RVA 0x46A1D0 - the id is round-tripped through the repository so that an
    // object which is not actually in it lands as -1.
    m_itemId = objId;
    if (m_repository)
    {
        m_itemId = m_repository->GetItem(m_repository->GetSlotByObjId(objId)).GetObjId();
    }
    else
    {
        m_itemId = -1;
    }
    return true;
}

int RepositoryItemWnd::GetResourceId() const
{
    // RVA 0x46A2D0
    return GetAsRepositoryItem().GetResourceId();
}

void RepositoryItemWnd::SetupInfoWnd(ItemInfoWnd* infoWnd)
{
    // RVA 0x46A2F0
    if (!infoWnd || !IsValid() || !GetItem())
    {
        return;
    }
    infoWnd->SetUpForRepositoryItem(GetAsRepositoryItem(), this);
}

int RepositoryItemWnd::GiveUpItem(ai::GeomRepositoryItem const& item, m3d::ui::Wnd*)
{
    // RVA 0x46A0B0 - buying the item out of the repository. NOTE: CanChangeItem
    // is hard-coded false above, so in the shipped build this always bails at
    // the second guard and only the HideInfoWnd tail runs.
    if (!IsValid() || !CanChangeItem() || !m_repository)
    {
        return 0;
    }
    if (item.m_parentRepository != m_repository || !item.IsValid())
    {
        return 0;
    }

    unsigned int givenUp = 0;
    if (ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown())
    {
        int const price =
            static_cast<int>(help::GetBuyPriceByObjId(item.GetObjId(), town->GetId()) * item.GetAmount());
        if (ai::thePlayer->GetMoney() >= price)
        {
            ai::thePlayer->AddMoney(-price);

            BoundsBase<int> const bounds = item.GetBounds();
            PointBase<int> const pt(bounds.x0, bounds.y0);
            givenUp = m_repository->GiveUpThingFromSlot(m_repository->GetSlotByPlace(pt), item.GetAmount());
            if (givenUp)
            {
                SetItemObjId(-1);
            }
        }
    }

    HideInfoWnd();
    return givenUp;
}
