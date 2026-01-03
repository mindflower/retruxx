#include "repositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(RepositoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepositoryWnd);

RepositoryWnd::CellBunch::~CellBunch()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::CellBunch::Reset()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryWnd::CellBunch::CellBunch()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryWnd::TwinkleInfo::TwinkleInfo(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::TwinkleInfo::Invalidate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryWnd::TwinkleInfo::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<int> RepositoryWnd::WndToGeomPt(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetSpecialized(std::vector<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> const& RepositoryWnd::GetMaxBounds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepositoryWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::Enable(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetCellSize(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> RepositoryWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::CreateScrollWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<int> RepositoryWnd::GetGeomSize() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetBounds(BoundsBase<float> const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetMaxBounds(BoundsBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepository* RepositoryWnd::GetRepository() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::SetRepository(ai::GeomRepository* repository, RepositoryWnd::RepositoryType type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<float> RepositoryWnd::GetCellSize() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepositoryWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RepositoryWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::GetItemDrawStyleByResourceId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryWnd::RepositoryType RepositoryWnd::GetRepositoryType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::CanAddDragItem(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RepositoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

BoundsBase<int> RepositoryWnd::WndToGeomBounds(BoundsBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryWnd::~RepositoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetItemDrawStyleByResourceId(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetItemToTwinkle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::UpdateOnItemQuickDrop(ItemAcceptInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::DrawBunch(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnSlotIn(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

GeomSlot* RepositoryWnd::GetSlotByItemObjId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::ShowVideo(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::UpdateCellBunchWhileNoDrag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::UpdateScrollWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetSlotCompatibleWithVehicle(int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::ScrollSlots()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetGeomSize(PointBase<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnDragOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::OnDragRemove()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::HideVideo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnPlayerMoneyChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::StartDrag()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::UpdateSlotsStylesCompatibleWithVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void RepositoryWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::UpdateCellBunchWhileDrag()
{
    RETRUXX_NOT_IMPLEMENTED;
}

float RepositoryWnd::GetMaxSlotY()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryWnd::IsItemCompatibleByResourceType(ai::GeomRepositoryItem const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::AddInfoToEncyclopaedia()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::AdjustWndToCells()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::RemoveAllItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::UpdateSlotsStylesTooRich()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::OnMouseButton1(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::AddItem(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::HideInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnSlotOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::ShowDragSrc()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::TwinkleItem(TwinkleInfo&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool RepositoryWnd::CanWindowQuickDropToMe(m3d::ui::Wnd const*, ai::GeomRepositoryItem const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnRepositoryChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

GeomSlot* RepositoryWnd::GetDraggingSlot() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::ShowInfo(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::HideDragSrc()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::SetSlotTooRich(int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem RepositoryWnd::GetItemFromOrigin(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::AdjustCellsToWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryWnd::RepositoryWnd(RepositoryWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepositoryWnd::RepositoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepositoryWnd::DrawUnsuitableBg(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepositoryWnd::OnMouseWheel(int, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
