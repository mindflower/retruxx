#include "helpmanager.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, ShowHelp)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, EnableAutoHelp)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, IsAutoHelpEnabled)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, ResetAutoHelp)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(HelpManager)
	RT_CLASS_EXPORT(HelpManager, m3d::METHOD, ShowHelp, "", "", "")
    RT_CLASS_EXPORT(HelpManager, m3d::METHOD, EnableAutoHelp, "", "", "")
    RT_CLASS_EXPORT(HelpManager, m3d::METHOD, IsAutoHelpEnabled, "", "", "")
    RT_CLASS_EXPORT(HelpManager, m3d::METHOD, ResetAutoHelp, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HelpManager);

HelpManager::AuxInfo::AuxInfo()
{
}

CStr HelpManager::GetHelpMsgByHelpId(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

CStr HelpManager::Enumered2StrHelpId(HelpId)
{
    throw std::logic_error("Not implemented");
}

void HelpManager::ShowHelp(CStr const&, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* HelpManager::CreateObject()
{
    return new HelpManager;
}

HelpManager::HelpId HelpManager::Str2EnumeredHelpId(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void HelpManager::Clear()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HelpManager::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void HelpManager::ResetAutoHelp()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HelpManager::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void HelpManager::EnableAutoHelp(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* HelpManager::Clone()
{
    throw std::logic_error("Not implemented");
}

CStr HelpManager::GetHelpTitleByHelpId(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

HelpManager::~HelpManager()
{
    throw std::logic_error("Not implemented");
}

bool HelpManager::IsAutoHelpEnabled() const
{
    throw std::logic_error("Not implemented");
}

int HelpManager::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

HelpManager::HelpManager()
{
    m_predefinedHelpWindows.emplace(HELP_ID_MAIN_GAME_INTERFACE, 169);
}

HelpManager::HelpManager(HelpManager const&)
{
    throw std::logic_error("Not implemented");
}

void HelpManager::ShowHelpEnumered(HelpId, bool)
{
    throw std::logic_error("Not implemented");
}

void HelpManager::HideCurrentHelpWindow()
{
    throw std::logic_error("Not implemented");
}

void HelpManager::MarkHelpAsShown(HelpId)
{
    throw std::logic_error("Not implemented");
}

ProfileParam HelpManager::GetProfileParamIdByHelpId(HelpId) const
{
    throw std::logic_error("Not implemented");
}

int HelpManager::GetPredefinedWndGuiIdByHelpId(HelpId) const
{
    throw std::logic_error("Not implemented");
}

int HelpManager::GetCurHelpWndGuiId() const
{
    throw std::logic_error("Not implemented");
}

void HelpManager::GetAllHelpWndGuiIds(std::vector<int>&) const
{
    throw std::logic_error("Not implemented");
}

void HelpManager::OnHelpImpulse()
{
    throw std::logic_error("Not implemented");
}

HelpManager::HelpId HelpManager::CalcHelpIdForCurrentInterfaceState() const
{
    throw std::logic_error("Not implemented");
}

bool HelpManager::IsHelpIdValidForCurrentInterfaceState(HelpId) const
{
    throw std::logic_error("Not implemented");
}

bool HelpManager::WasHelpShown(HelpId) const
{
    throw std::logic_error("Not implemented");
}

void HelpManager::OnHelpEvent(void*)
{
    throw std::logic_error("Not implemented");
}
