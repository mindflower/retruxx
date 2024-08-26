#include "repositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(RepositoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepositoryWnd);

RepositoryWnd::CellBunch::~CellBunch()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::CellBunch::Reset()
{
    throw std::logic_error("Not implemented");
}

RepositoryWnd::CellBunch::CellBunch()
{
    throw std::logic_error("Not implemented");
}

RepositoryWnd::TwinkleInfo::TwinkleInfo(int)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::TwinkleInfo::Invalidate()
{
    throw std::logic_error("Not implemented");
}

bool RepositoryWnd::TwinkleInfo::IsValid() const
{
    throw std::logic_error("Not implemented");
}

RepositoryWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

PointBase<int> RepositoryWnd::WndToGeomPt(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetSpecialized(std::vector<int> const&)
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> const& RepositoryWnd::GetMaxBounds() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepositoryWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::Enable(bool)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetCellSize(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> RepositoryWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::CreateScrollWnd()
{
    throw std::logic_error("Not implemented");
}

PointBase<int> RepositoryWnd::GetGeomSize() const
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetBounds(BoundsBase<float> const&, bool)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetMaxBounds(BoundsBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

PointBase<float> RepositoryWnd::GetCellSize() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepositoryWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepositoryWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::GetItemDrawStyleByResourceId(int) const
{
    throw std::logic_error("Not implemented");
}

RepositoryWnd::RepositoryType RepositoryWnd::GetRepositoryType() const
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::CanAddDragItem(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepositoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

BoundsBase<int> RepositoryWnd::WndToGeomBounds(BoundsBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

RepositoryWnd::~RepositoryWnd()
{
    throw std::logic_error("Not implemented");
}

bool RepositoryWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetItemDrawStyleByResourceId(int, int)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetItemToTwinkle(int)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::UpdateOnItemQuickDrop(ItemAcceptInfo const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::DrawBunch(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnSlotIn(int)
{
    throw std::logic_error("Not implemented");
}

GeomSlot* RepositoryWnd::GetSlotByItemObjId(int) const
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::ShowVideo(int)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::UpdateCellBunchWhileNoDrag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::UpdateScrollWnd()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetSlotCompatibleWithVehicle(int, bool)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::ScrollSlots()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnVehiclePartChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetGeomSize(PointBase<int> const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnDragOut()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::OnDragRemove()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::HideVideo()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnPlayerMoneyChanged()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::StartDrag()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::UpdateSlotsStylesCompatibleWithVehicle()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::UpdateCellBunchWhileDrag()
{
    throw std::logic_error("Not implemented");
}

float RepositoryWnd::GetMaxSlotY()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

bool RepositoryWnd::IsItemCompatibleByResourceType(ai::GeomRepositoryItem const&) const
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::AddInfoToEncyclopaedia()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::AdjustWndToCells()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::RemoveAllItems()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::UpdateSlotsStylesTooRich()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::AddItem(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::HideInfo()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnSlotOut()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::ShowDragSrc()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::TwinkleItem(TwinkleInfo&)
{
    throw std::logic_error("Not implemented");
}

bool RepositoryWnd::CanWindowQuickDropToMe(m3d::ui::Wnd const*, ai::GeomRepositoryItem const&) const
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnPlayerVehicleChanged()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnRepositoryChanged(void*)
{
    throw std::logic_error("Not implemented");
}

GeomSlot* RepositoryWnd::GetDraggingSlot() const
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::ShowInfo(int)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::HideDragSrc()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::SetSlotTooRich(int, bool)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::CreateItems()
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem RepositoryWnd::GetItemFromOrigin(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::AdjustCellsToWnd()
{
    throw std::logic_error("Not implemented");
}

RepositoryWnd::RepositoryWnd(RepositoryWnd const&)
{
    throw std::logic_error("Not implemented");
}

RepositoryWnd::RepositoryWnd()
{
    throw std::logic_error("Not implemented");
}

void RepositoryWnd::DrawUnsuitableBg(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int RepositoryWnd::OnMouseWheel(int, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}
