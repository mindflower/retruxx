#include "encyclopaediawnd.h"

RT_CLASS_EXPORTS_BEGIN(EncyclopaediaWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(EncyclopaediaWnd);

EncyclopaediaWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* EncyclopaediaWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

std::vector<int> const* EncyclopaediaWnd::GetClans() const
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::AddClan(int)
{
    throw std::logic_error("Not implemented");
}

bool EncyclopaediaWnd::IsClanVisible(int) const
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::ShowAll()
{
    throw std::logic_error("Not implemented");
}

std::vector<int> const* EncyclopaediaWnd::GetPrototypesForTheme(Theme) const
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::AddPrototype(int)
{
    throw std::logic_error("Not implemented");
}

EncyclopaediaWnd::~EncyclopaediaWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* EncyclopaediaWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* EncyclopaediaWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

bool EncyclopaediaWnd::IsPrototypeVisible(int) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* EncyclopaediaWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle EncyclopaediaWnd::GetImageForClan(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::AddVehicle(ai::Vehicle const*)
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::CheckAndAddTargetInfoVehicle()
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnObjMet(void*)
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::GetRelatedVisiblePrototypeId(int) const
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::AddPrototypeByThemeUnsafe(int, Theme)
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::CacheRelatedPrototypes()
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnNewFrameForce()
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

EncyclopaediaWnd::EncyclopaediaWnd()
{
    throw std::logic_error("Not implemented");
}

EncyclopaediaWnd::EncyclopaediaWnd(EncyclopaediaWnd const&)
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnStartLevel(void*)
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::InitThemes()
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::UpdateModel()
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnPrototypeMet(void*)
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::AddObj(int)
{
    throw std::logic_error("Not implemented");
}

CStr EncyclopaediaWnd::Theme2Str(Theme) const
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnSubjectChanged()
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::InitOnce()
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnPlayerVehicleDamaged(void*)
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::CheckAndAddTargetCapturingVehicle()
{
    throw std::logic_error("Not implemented");
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::Str2Theme(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::CheckAndAddTargetCapturedVehicle()
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::UpdateDiz()
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::InitAlwaysVisiblePrototypes()
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnQuestItemsChanged()
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnBelongMet(void*)
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnGadgetChanged(void*)
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle EncyclopaediaWnd::GetImageForAffix(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::GetPrototypeIdsKindOfResource(int, std::vector<int>&) const
{
    throw std::logic_error("Not implemented");
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetThemeByPrototypeId(int) const
{
    throw std::logic_error("Not implemented");
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetCurTheme() const
{
    throw std::logic_error("Not implemented");
}

CStr EncyclopaediaWnd::GetCurSubjectName() const
{
    throw std::logic_error("Not implemented");
}

int EncyclopaediaWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetThemeByResourceId(int) const
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnThemeChanged()
{
    throw std::logic_error("Not implemented");
}

void EncyclopaediaWnd::OnPlayerVehicleChanged()
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(SubjectButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SubjectButton);

int SubjectButton::SetUp(CStr const&, PointBase<float> const&, float, EncyclopaediaWnd::Theme, int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SubjectButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CheckButton);
}

m3d::Object* SubjectButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SubjectButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

SubjectButton::~SubjectButton()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SubjectButton::Clone()
{
    throw std::logic_error("Not implemented");
}

int SubjectButton::SetUp(CStr const&, PointBase<float> const&, float, CStr const&)
{
    throw std::logic_error("Not implemented");
}

SubjectButton::SubjectButton()
{
    throw std::logic_error("Not implemented");
}

SubjectButton::SubjectButton(SubjectButton const&)
{
    throw std::logic_error("Not implemented");
}

CStr SubjectButton::CalcFullName() const
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(SubjectList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SubjectList);

m3d::Object* SubjectList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int SubjectList::Update()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SubjectList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SubjectList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CheckList);
}

m3d::Object* SubjectList::Clone()
{
    throw std::logic_error("Not implemented");
}

CStr SubjectList::GetCurSubjectName() const
{
    throw std::logic_error("Not implemented");
}

SubjectList::~SubjectList()
{
    throw std::logic_error("Not implemented");
}

int SubjectList::SetUpForTheme(EncyclopaediaWnd::Theme)
{
    throw std::logic_error("Not implemented");
}

int SubjectList::CreateItemsForClans()
{
    throw std::logic_error("Not implemented");
}

int SubjectList::AddButtonByNameAndId(CStr const&, int)
{
    throw std::logic_error("Not implemented");
}

int SubjectList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int SubjectList::CreateItemsForPrototypes(EncyclopaediaWnd::Theme)
{
    throw std::logic_error("Not implemented");
}

int SubjectList::CreateItemsForAffixes()
{
    throw std::logic_error("Not implemented");
}

int SubjectList::AddButtonByName(CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

SubjectList::SubjectList()
{
    throw std::logic_error("Not implemented");
}

SubjectList::SubjectList(SubjectList const&)
{
    throw std::logic_error("Not implemented");
}
