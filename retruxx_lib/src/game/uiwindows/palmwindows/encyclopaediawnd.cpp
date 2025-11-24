#include "encyclopaediawnd.h"

RT_CLASS_EXPORTS_BEGIN(EncyclopaediaWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(EncyclopaediaWnd);

EncyclopaediaWnd::AuxInfo::AuxInfo()
{
    m_subjectListName = "subjectList";
    m_wndDizName = "wndDiz";
    m_wndModelName = "wndModel";
    m_comboThemesName = "comboThemes";
}

m3d::Class* EncyclopaediaWnd::GetClass() const
{
    return RT_CLASS_LOCAL(EncyclopaediaWnd);
}

retruxx::vector<int> const* EncyclopaediaWnd::GetClans() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::AddClan(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool EncyclopaediaWnd::IsClanVisible(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::ShowAll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

retruxx::vector<int> const* EncyclopaediaWnd::GetPrototypesForTheme(Theme) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::AddPrototype(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

EncyclopaediaWnd::~EncyclopaediaWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* EncyclopaediaWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* EncyclopaediaWnd::CreateObject()
{
    return new EncyclopaediaWnd;
}

bool EncyclopaediaWnd::IsPrototypeVisible(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* EncyclopaediaWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle EncyclopaediaWnd::GetImageForClan(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::AddVehicle(ai::Vehicle const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::CheckAndAddTargetInfoVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnObjMet(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::GetRelatedVisiblePrototypeId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::AddPrototypeByThemeUnsafe(int, Theme)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::CacheRelatedPrototypes()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnNewFrameForce()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

EncyclopaediaWnd::EncyclopaediaWnd()
{
    m_subjectList = 0;
    m_wndDiz = 0;
    m_comboThemes = 0;
    m_wndModel = 0;
    m_bInitedOnce = 0;
    m_prevInfoObjId = -1;
    m_prevCapturingObjId = -1;
    m_prevCapturedObjId = -1;
}

EncyclopaediaWnd::EncyclopaediaWnd(EncyclopaediaWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnStartLevel(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::InitThemes()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::UpdateModel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnPrototypeMet(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::AddObj(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr EncyclopaediaWnd::Theme2Str(Theme) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnSubjectChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::InitOnce()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::GameDataUpdate(void*, int)
{
    // TODO: implement EncyclopaediaWnd::GameDataUpdate
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void EncyclopaediaWnd::OnPlayerVehicleDamaged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::CheckAndAddTargetCapturingVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::Str2Theme(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::CheckAndAddTargetCapturedVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::UpdateDiz()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::InitAlwaysVisiblePrototypes()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnQuestItemsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnBelongMet(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::GameDataSetup()
{
    // TODO: implement EncyclopaediaWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void EncyclopaediaWnd::OnGadgetChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle EncyclopaediaWnd::GetImageForAffix(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::GetPrototypeIdsKindOfResource(int, retruxx::vector<int>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetThemeByPrototypeId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetCurTheme() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr EncyclopaediaWnd::GetCurSubjectName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int EncyclopaediaWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetThemeByResourceId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnThemeChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void EncyclopaediaWnd::OnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(SubjectButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SubjectButton);

int SubjectButton::SetUp(CStr const&, PointBase<float> const&, float, EncyclopaediaWnd::Theme, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SubjectButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CheckButton);
}

m3d::Object* SubjectButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SubjectButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

SubjectButton::~SubjectButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SubjectButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectButton::SetUp(CStr const&, PointBase<float> const&, float, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SubjectButton::SubjectButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SubjectButton::SubjectButton(SubjectButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SubjectButton::CalcFullName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(SubjectList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SubjectList);

m3d::Object* SubjectList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::Update()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SubjectList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SubjectList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CheckList);
}

m3d::Object* SubjectList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr SubjectList::GetCurSubjectName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

SubjectList::~SubjectList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::SetUpForTheme(EncyclopaediaWnd::Theme)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::CreateItemsForClans()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::AddButtonByNameAndId(CStr const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::CreateItemsForPrototypes(EncyclopaediaWnd::Theme)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::CreateItemsForAffixes()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SubjectList::AddButtonByName(CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SubjectList::SubjectList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SubjectList::SubjectList(SubjectList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
