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
    RETRUXX_NOT_IMPLEMENTED;
}

int CBButton::SetupForCB(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CBButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBButton::~CBButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBButton::GetPrice() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CBButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CBButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBButton::Select(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBButton::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBButton::CBButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBButton::CBButton(CBButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBButton::ClearPattern(Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBButton::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBButton::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBButton::LoadPattern(ref_ptr<m3d::ui::Wnd>, Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBList::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBList::~CBList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CBList::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CBList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int CBList::SetupForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::ScrollPrev()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::OnMouseWheel(int, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CBList::CBList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::HackedRestore()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::ClearItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::OnBuyCB(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::AddItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::ScrollNext()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool CBList::CanScrollNext()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepository* CBList::GetWorkshopRepository() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool CBList::CanScrollPrev()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::UpdateNextPrevButtonState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::SelectItem(CBButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::GetCBIds(std::vector<int, std::allocator<int>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* CBList::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::AddInfoToEncyclopaedia()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CBList::OnRepositoryChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CBList::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
