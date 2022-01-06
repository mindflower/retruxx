#include "truxxuimanager.h"
#include <stdexcept>
#include <renderer/i_renderer.h>
#include <core/ini.h>
#include <ui/wnd.h>

CStr TruxxUiManager::GetPathToDialogsFileGlobal() const
{
    throw std::logic_error("Not implemented");
}

QuestInfoManager* TruxxUiManager::GetQuestInfoManager() const
{
    throw std::logic_error("Not implemented");
}

ref_ptr<m3d::ui::Wnd> TruxxUiManager::GetWindow(int) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GetDefaultFloatPrecision() const
{
    throw std::logic_error("Not implemented");
}

MsgManager* TruxxUiManager::GetMsgManager() const
{
    throw std::logic_error("Not implemented");
}

ObjectCollection const& TruxxUiManager::GetObjectCollection() const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsHidden() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::AddWindow(ref_ptr<m3d::ui::Wnd>, int&, bool, bool)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::AddWindowById(ref_ptr<m3d::ui::Wnd>, int, bool, bool)
{
    throw std::logic_error("Not implemented");
}

CStr TruxxUiManager::GetPathToQuestInfoFileGlobal() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::HandleImpulse(m3d::AuxImpulseInfo const&, m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

TruxxUiManager::TruxxUiManager()
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsHiddenByUser() const
{
    throw std::logic_error("Not implemented");
}

CStr TruxxUiManager::GetPathToDynamicDialogsFileGlobal() const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsWindowVisibleAndNotAnimating(int) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Load(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Init()
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsModalEqualWndRunning() const
{
    throw std::logic_error("Not implemented");
}

LevelInfoManager* TruxxUiManager::GetLevelInfoManager() const
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnEnterTown(int)
{
    throw std::logic_error("Not implemented");
}

CStr TruxxUiManager::GetPathToLevelInfoFile() const
{
    throw std::logic_error("Not implemented");
}

m3d::rend::TexHandle TruxxUiManager::GetIcoByName(CStr const&, int)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::ShowWindow(int, bool, bool, bool, bool, int*)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::ShowSplash(int, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Show(bool, bool)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::CreateAndAddWindow(int)
{
    throw std::logic_error("Not implemented");
}

StringParser const& TruxxUiManager::GetStringParser() const
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* TruxxUiManager::GetCurrentShop() const
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::AddFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

NavPointManager* TruxxUiManager::GetNavPointManager() const
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* TruxxUiManager::GetCurrentWorkshop() const
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::AddFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsGameModeValidForSmartCursor(GameState) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Str2WndGuiId(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* TruxxUiManager::GetVehicleSellingInWorkshop() const
{
    throw std::logic_error("Not implemented");
}

std::vector<int, std::allocator<int>> const& TruxxUiManager::GetTakenQuestIds() const
{
    throw std::logic_error("Not implemented");
}

RepliesManager* TruxxUiManager::GetRepliesManager() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Reset(bool)
{
    throw std::logic_error("Not implemented");
}

WeaponGroupManager* TruxxUiManager::GetWeaponGroupManager() const
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::AddImportantFadingMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::ui::MbRetCodes TruxxUiManager::RunMsgBoxDlg(CStr const&, CStr const&, unsigned, bool)
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsInSaleMode() const
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::IsWindowVisible(int) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::SetEventsForWindow(int, std::vector<int, std::allocator<int>> const&)
{
    throw std::logic_error("Not implemented");
}

TruxxUiManager::~TruxxUiManager()
{
    throw std::logic_error("Not implemented");
}

CStr TruxxUiManager::GetPathToSplashes() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Update()
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::LaunchEvent(int, GuiEventType, void*)
{
    throw std::logic_error("Not implemented");
}

SavesManager* TruxxUiManager::GetSavesManager() const
{
    throw std::logic_error("Not implemented");
}

ai::Town* TruxxUiManager::GetCurrentTown() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::Save(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::StartSplashing(int) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::HandleAppEvent(m3d::Event const&)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnLeaveTown(bool)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::AddImportantFadingMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&) const
{
    throw std::logic_error("Not implemented");
}

HelpManager* TruxxUiManager::GetHelpManager() const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::RemoveWindow(int)
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::GUI_IsWndModalEqual(m3d::ui::Wnd*) const
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GUI_ReadFromXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::IncRef()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_UnRegisterScriptGlobals()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::ShowGameMenu(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnGameModeChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnBeforeStartLevel()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_EndModalDlg()
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GUI_BindWindowsToEvents()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_RegisterCVars()
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GUI_WriteToXml(ref_ptr<m3d::cmn::XmlFile>, ref_ptr<m3d::cmn::XmlNode>)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnStartLevel(void*)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnEndLevel(bool)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::LoadCommonDiz()
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::CanLaunchModalEqualWindow()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::PrepareMenuForShow(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::DecRef()
{
    throw std::logic_error("Not implemented");
}

bool TruxxUiManager::GUI_NeedUpdateWndOnEvent(ref_ptr<m3d::ui::Wnd>, int, void*)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GUI_HandleEvent(int, m3d::ui::Wnd*, void*)
{
    throw std::logic_error("Not implemented");
}

void* TruxxUiManager::QueryIface(char const*)
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_RegisterEvents()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_RegisterClasses()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::OnChangeGameMenuMode()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::GUI_UnregisterCVars()
{
    throw std::logic_error("Not implemented");
}

void TruxxUiManager::SetGameMenuMode(bool)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GUI_BeginModalDlg(bool, bool)
{
    throw std::logic_error("Not implemented");
}

int TruxxUiManager::GUI_RegisterScriptGlobals()
{
    throw std::logic_error("Not implemented");
}
