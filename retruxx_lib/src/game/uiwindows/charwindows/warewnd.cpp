#include "warewnd.h"

RT_CLASS_EXPORTS_BEGIN(WareItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WareItem);

RT_CLASS_EXPORTS_BEGIN(WareList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WareList);

RT_CLASS_EXPORTS_BEGIN(WareWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WareWnd);

m3d::Object* WareItem::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WareItem::IsMarkedByPrototype() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WareItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Object* WareItem::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareItem::~WareItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareItem::SetUp(int, int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WareItem::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::MarkByPrototype(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareItem::GetWarePrototypeId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::UpdateBuyPrice(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareItem::GetTownId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareItem::WareItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareItem::WareItem(WareItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareItem::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::UpdateSellPrice(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareItem::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::GetBtnImages(bool, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::ClearPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::UpdateActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::OnPaintOverChildren(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::UpdateName()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareItem::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::UpdateIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::UpdateTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* WareItem::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::SetActive(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr WareItem::GetFullNameForWare(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WareItem::GetIcoForWare(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareItem::UpdateAmount(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::TwinkleInfo::Invalidate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareList::TwinkleInfo::TwinkleInfo(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WareList::TwinkleInfo::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareList::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::SetupForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareList::~WareList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::ReactOnMouseClick(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WareList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::MarkItemByPrototypeId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WareList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void WareList::SetItemToTwinkle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WareList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::BuyWare(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareList::Mode WareList::GetCurMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WareList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::SetCurMode(Mode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::ClearItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::AddItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::OnSelectItem(WareItem*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareList::WareList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareList::WareList(WareList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::SellWare(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::TwinkleItem(TwinkleInfo&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::ShowVideo(WareItem*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::HideVideo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* WareList::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareList::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareItem* WareList::GetItemByWarePrototypeId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareList::EnsureVisibleWare(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::CanAddDragItem(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::UpdateOnItemQuickDrop(ItemAcceptInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WareWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> WareWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::BuyWare(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WareWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::SetupForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WareWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WareWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

int WareWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareWnd::~WareWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WareWnd::CanAddDragItemDistant(ai::GeomRepositoryItem const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::SetCurMode(WareList::Mode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::UpdateModeButtonState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnDistantMouseOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnCurProfileParamChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WareWnd::CanWindowQuickDropToMe(m3d::ui::Wnd const*, ai::GeomRepositoryItem const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::OnDragRemove()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnDistantMouseIn(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareList::Mode WareWnd::GetCurMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnCurProfileChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::AddItem(ai::GeomRepositoryItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepositoryItem WareWnd::GetItemFromOrigin(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnDragOut()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnModeBtnClicked()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::HideDragSrc()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::ShowDragSrc()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::UpdateModeTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnChangeCurMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WareWnd::IsItemCompatibleByResourceType(ai::GeomRepositoryItem const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WareWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* WareWnd::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WareWnd::OnPaintOverChildren(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareWnd::WareWnd(WareWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WareWnd::WareWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}
