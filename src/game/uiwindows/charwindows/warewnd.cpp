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
    throw std::logic_error("Not implemented");
}

bool WareItem::IsMarkedByPrototype() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WareItem::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WareItem::Clone()
{
    throw std::logic_error("Not implemented");
}

WareItem::~WareItem()
{
    throw std::logic_error("Not implemented");
}

int WareItem::SetUp(int, int, int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WareItem::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void WareItem::MarkByPrototype(bool)
{
    throw std::logic_error("Not implemented");
}

void WareItem::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int WareItem::GetWarePrototypeId() const
{
    throw std::logic_error("Not implemented");
}

void WareItem::UpdateBuyPrice(bool)
{
    throw std::logic_error("Not implemented");
}

int WareItem::GetTownId() const
{
    throw std::logic_error("Not implemented");
}

WareItem::WareItem()
{
    throw std::logic_error("Not implemented");
}

WareItem::WareItem(WareItem const&)
{
    throw std::logic_error("Not implemented");
}

int WareItem::CreateFromPattern()
{
    throw std::logic_error("Not implemented");
}

void WareItem::UpdateSellPrice(bool)
{
    throw std::logic_error("Not implemented");
}

int WareItem::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

void WareItem::GetBtnImages(bool, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&) const
{
    throw std::logic_error("Not implemented");
}

void WareItem::ClearPattern()
{
    throw std::logic_error("Not implemented");
}

void WareItem::UpdateActiveState()
{
    throw std::logic_error("Not implemented");
}

void WareItem::OnPaintOverChildren(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void WareItem::UpdateName()
{
    throw std::logic_error("Not implemented");
}

int WareItem::LoadPattern(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

void WareItem::UpdateIco()
{
    throw std::logic_error("Not implemented");
}

void WareItem::UpdateTooltip()
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* WareItem::GetWorkshop() const
{
    throw std::logic_error("Not implemented");
}

void WareItem::SetActive(bool)
{
    throw std::logic_error("Not implemented");
}

CStr WareItem::GetFullNameForWare(int) const
{
    throw std::logic_error("Not implemented");
}

void WareItem::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle WareItem::GetIcoForWare(int) const
{
    throw std::logic_error("Not implemented");
}

void WareItem::UpdateAmount(bool)
{
    throw std::logic_error("Not implemented");
}

void WareList::TwinkleInfo::Invalidate()
{
    throw std::logic_error("Not implemented");
}

WareList::TwinkleInfo::TwinkleInfo(int)
{
    throw std::logic_error("Not implemented");
}

bool WareList::TwinkleInfo::IsValid() const
{
    throw std::logic_error("Not implemented");
}

WareList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int WareList::SetupForWorkshop(int)
{
    throw std::logic_error("Not implemented");
}

WareList::~WareList()
{
    throw std::logic_error("Not implemented");
}

void WareList::ReactOnMouseClick(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WareList::Clone()
{
    throw std::logic_error("Not implemented");
}

void WareList::MarkItemByPrototypeId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WareList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void WareList::SetItemToTwinkle(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WareList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int WareList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

int WareList::BuyWare(int)
{
    throw std::logic_error("Not implemented");
}

void WareList::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

WareList::Mode WareList::GetCurMode() const
{
    throw std::logic_error("Not implemented");
}

int WareList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WareList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void WareList::SetCurMode(Mode)
{
    throw std::logic_error("Not implemented");
}

void WareList::ClearItems()
{
    throw std::logic_error("Not implemented");
}

int WareList::AddItem(int, int)
{
    throw std::logic_error("Not implemented");
}

void WareList::OnSelectItem(WareItem*)
{
    throw std::logic_error("Not implemented");
}

WareList::WareList()
{
    throw std::logic_error("Not implemented");
}

WareList::WareList(WareList const&)
{
    throw std::logic_error("Not implemented");
}

int WareList::SellWare(int)
{
    throw std::logic_error("Not implemented");
}

int WareList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void WareList::TwinkleItem(TwinkleInfo&)
{
    throw std::logic_error("Not implemented");
}

void WareList::RecalcLayot()
{
    throw std::logic_error("Not implemented");
}

void WareList::ShowVideo(WareItem*)
{
    throw std::logic_error("Not implemented");
}

void WareList::HideVideo()
{
    throw std::logic_error("Not implemented");
}

int WareList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* WareList::GetWorkshop() const
{
    throw std::logic_error("Not implemented");
}

void WareList::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

WareItem* WareList::GetItemByWarePrototypeId(int) const
{
    throw std::logic_error("Not implemented");
}

int WareList::EnsureVisibleWare(int)
{
    throw std::logic_error("Not implemented");
}

WareWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int WareWnd::CanAddDragItem(bool)
{
    throw std::logic_error("Not implemented");
}

int WareWnd::UpdateOnItemQuickDrop(ItemAcceptInfo const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WareWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> WareWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    throw std::logic_error("Not implemented");
}

int WareWnd::BuyWare(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WareWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int WareWnd::SetupForWorkshop(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WareWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WareWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int WareWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

WareWnd::~WareWnd()
{
    throw std::logic_error("Not implemented");
}

bool WareWnd::CanAddDragItemDistant(ai::GeomRepositoryItem const&) const
{
    throw std::logic_error("Not implemented");
}

void WareWnd::SetCurMode(WareList::Mode)
{
    throw std::logic_error("Not implemented");
}

void WareWnd::UpdateModeButtonState()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnDistantMouseOut()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnCurProfileParamChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int WareWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

int WareWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

bool WareWnd::CanWindowQuickDropToMe(m3d::ui::Wnd const*, ai::GeomRepositoryItem const&) const
{
    throw std::logic_error("Not implemented");
}

int WareWnd::OnDragRemove()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnDistantMouseIn(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

WareList::Mode WareWnd::GetCurMode() const
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnCurProfileChanged()
{
    throw std::logic_error("Not implemented");
}

int WareWnd::AddItem(ai::GeomRepositoryItem const&)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepositoryItem WareWnd::GetItemFromOrigin(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnDragOut()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnModeBtnClicked()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::HideDragSrc()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::ShowDragSrc()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::UpdateModeTooltip()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnChangeCurMode()
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int WareWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

bool WareWnd::IsItemCompatibleByResourceType(ai::GeomRepositoryItem const&) const
{
    throw std::logic_error("Not implemented");
}

int WareWnd::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* WareWnd::GetWorkshop() const
{
    throw std::logic_error("Not implemented");
}

void WareWnd::OnPaintOverChildren(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

WareWnd::WareWnd(WareWnd const&)
{
    throw std::logic_error("Not implemented");
}

WareWnd::WareWnd()
{
    throw std::logic_error("Not implemented");
}
