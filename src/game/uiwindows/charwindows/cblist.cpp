#include "cblist.h"
#include "repositoryitemwnd.h"

RT_CLASS_EXPORTS_BEGIN(CBButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBButton);

RT_CLASS_EXPORTS_BEGIN(CBList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBList);

m3d::Class* CBButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int CBButton::GetCBId() const
{
    throw std::logic_error("Not implemented");
}

int CBButton::SetupForCB(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CBButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

CBButton::~CBButton()
{
    throw std::logic_error("Not implemented");
}

int CBButton::GetPrice() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CBButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CBButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void CBButton::Select(bool)
{
    throw std::logic_error("Not implemented");
}

int CBButton::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

int CBButton::CreateFromPattern()
{
    throw std::logic_error("Not implemented");
}

CBButton::CBButton()
{
    throw std::logic_error("Not implemented");
}

CBButton::CBButton(CBButton const&)
{
    throw std::logic_error("Not implemented");
}

void CBButton::ClearPattern(Type)
{
    throw std::logic_error("Not implemented");
}

int CBButton::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

int CBButton::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int CBButton::LoadPattern(ref_ptr<m3d::ui::Wnd>, Type)
{
    throw std::logic_error("Not implemented");
}

CBList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

CBList::~CBList()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CBList::GetRtClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CBList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int CBList::SetupForWorkshop(int)
{
    throw std::logic_error("Not implemented");
}

void CBList::ScrollPrev()
{
    throw std::logic_error("Not implemented");
}

int CBList::OnMouseWheel(int, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

int CBList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

CBList::CBList()
{
    throw std::logic_error("Not implemented");
}

int CBList::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void CBList::HackedRestore()
{
    throw std::logic_error("Not implemented");
}

void CBList::ClearItems()
{
    throw std::logic_error("Not implemented");
}

void CBList::OnBuyCB(int)
{
    throw std::logic_error("Not implemented");
}

int CBList::AddItem(int)
{
    throw std::logic_error("Not implemented");
}

int CBList::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void CBList::ScrollNext()
{
    throw std::logic_error("Not implemented");
}

bool CBList::CanScrollNext()
{
    throw std::logic_error("Not implemented");
}

int CBList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepository* CBList::GetWorkshopRepository() const
{
    throw std::logic_error("Not implemented");
}

bool CBList::CanScrollPrev()
{
    throw std::logic_error("Not implemented");
}

void CBList::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

void CBList::UpdateNextPrevButtonState()
{
    throw std::logic_error("Not implemented");
}

void CBList::SelectItem(CBButton*)
{
    throw std::logic_error("Not implemented");
}

int CBList::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void CBList::GetCBIds(std::vector<int, std::allocator<int>>&) const
{
    throw std::logic_error("Not implemented");
}

int CBList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

int CBList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* CBList::GetWorkshop() const
{
    throw std::logic_error("Not implemented");
}

void CBList::RecalcLayot()
{
    throw std::logic_error("Not implemented");
}

void CBList::AddInfoToEncyclopaedia()
{
    throw std::logic_error("Not implemented");
}

void CBList::OnRepositoryChanged()
{
    throw std::logic_error("Not implemented");
}

int CBList::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}
