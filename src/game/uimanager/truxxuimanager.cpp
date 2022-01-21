#include "truxxuimanager.h"
#include <config.h>
#include <stdexcept>
#include <renderer/i_renderer.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/console/console.h>
#include <game/uimisc/helpmanager.h>
#include <game/uimisc/levelinfo.h>
#include <game/uimisc/msgmanager.h>
#include <game/uimisc/navpoint.h>
#include <game/uimisc/questinfo.h>
#include <game/uimisc/reply.h>
#include <game/uimisc/savesmanager.h>
#include <game/uimisc/weapongroup.h>
#include <game/uiwindows/miscwindows/gamemenu.h>
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
    m_minDynamicId = 173;
    m_nextDynamicId = GUI_ValidateDynamicId(173);
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
    auto res = GUI_Init(true) & 1;
    m_questInfoManager = new QuestInfoManager;
    if (!m_questInfoManager)
    {
        res = 0;
    }
    m_repliesManager = dynamic_cast<RepliesManager*>(m3d::g_Kernel->New("RepliesManager"));
    if (!m_repliesManager)
    {
        res = 0;
    }
    m_levelInfoManager = dynamic_cast<LevelInfoManager*>(m3d::g_Kernel->New("LevelInfoManager"));
    if (m_levelInfoManager)
    {
        m_levelInfoManager->Init();
    }
    else
    {
        res = 0;
    }
    m_navPointManager = new NavPointManager;
    if (!m_navPointManager)
    {
        res = 0;
    }
    m_weaponGroupManager = dynamic_cast<WeaponGroupManager*>(m3d::g_Kernel->New("WeaponGroupManager"));
    if (!m_weaponGroupManager)
    {
        res = 0;
    }
    m_savesManager = dynamic_cast<SavesManager*>(m3d::g_Kernel->New("SavesManager"));
    if (!m_savesManager)
    {
        res = 0;
    }
    m_msgManager = dynamic_cast<MsgManager*>(m3d::g_Kernel->New("MsgManager"));
    if (!m_msgManager)
    {
        res = 0;
    }
    m_helpManager = dynamic_cast<HelpManager*>(m3d::g_Kernel->New("HelpManager"));
    if (!m_helpManager)
    {
        res = 0;
    }
    ref_ptr<m3d::ui::Wnd> gameMenuWnd = dynamic_cast<GameMenuWnd*>(m3d::g_Kernel->New("GameMenuWnd"));
    if (gameMenuWnd)
    {
        res = AddWindowById(gameMenuWnd, 154, true, false) & res;
        if (res)
        {
            M3D_LOG_ERR("TruxxUiManager: was inited with errors");
            return res;
        }
    }
    M3D_LOG_INFO("TruxxUiManager: was inited successfully");
    return res;
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
    GameUiManager::GUI_RegisterCVars();
    m_cvPathToQuestInfo.Init("pathToQuestInfo", "data\\if\\diz\\QuestInfoGlobal.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToDialogs.Init("pathToDialogs", "data\\if\\diz\\DialogsGlobal.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToDynamicDialogs.Init("pathToDynamicDialogs", "data\\if\\diz\\DynamicDialogsGlobal.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToLevelInfo.Init("pathToLevelInfo", "data\\if\\diz\\LevelInfo.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvPathToSplashes.Init("pathToSplashes", "data\\if\\ico\\splashes.xml", m3d::CVar::CVAR_STRING, m3d::CVar::eFlags::CVAR_ARCHIVE);
    m_cvDefaultFloatPrecision.Init("guiFloatPrecision", "2", m3d::CVar::CVAR_INT, m3d::CVar::eFlags::CVAR_ARCHIVE);

    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToQuestInfo, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToDialogs, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToDynamicDialogs, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToLevelInfo, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToSplashes, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvDefaultFloatPrecision, nullptr);
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
    //TODO: check this
    m_impulseToEvent[41] = 0;
    m_impulseToEvent[42] = 2;
    m_impulseToEvent[43] = 3;
    m_impulseToEvent[44] = 4;
    m_impulseToEvent[45] = 5;
    m_impulseToEvent[46] = 6;
    m_impulseToEvent[47] = 7;
    m_impulseToEvent[52] = 8;
    m_impulseToEvent[53] = 9;
    m_impulseToEvent[54] = 10;
    m_impulseToEvent[39] = 11;
    m_impulseToEvent[48] = 12;
    m_impulseToEvent[49] = 13;
    m_impulseToEvent[55] = 14;
    m_impulseToEvent[0] = 15;
    m_impulseToEvent[50] = 16;
    m_impulseToEvent[46] = 17;
    m_impulseToEvent[42] = 18;
    m_impulseToEvent[65653] = 19;
    m_impulseToEvent[65657] = 20;
    m_impulseToEvent[65658] = 21;
    m_impulseToEvent[65659] = 22;
    m_impulseToEvent[65660] = 23;
    m_impulseToEvent[65661] = 24;
    m_impulseToEvent[65663] = 25;
    m_impulseToEvent[65665] = 26;
    m_impulseToEvent[65664] = 27;
    m_impulseToEvent[65666] = 28;
    m_impulseToEvent[65667] = 29;
    m_impulseToEvent[65668] = 30;
    m_impulseToEvent[65669] = 31;
    m_impulseToEvent[65670] = 32;
    m_impulseToEvent[65671] = 33;
    m_impulseToEvent[65672] = 34;
    m_impulseToEvent[65673] = 35;
    m_impulseToEvent[65674] = 36;
    m_impulseToEvent[65675] = 37;
    m_impulseToEvent[65676] = 38;
    m_impulseToEvent[65677] = 39;
    m_impulseToEvent[65678] = 40;
    m_impulseToEvent[65679] = 41;
    m_impulseToEvent[65680] = 42;
    m_impulseToEvent[65644] = 43;
    m_impulseToEvent[65645] = 44;
    m_impulseToEvent[65684] = 45;
    m_impulseToEvent[65681] = 46;
    m_impulseToEvent[65682] = 47;
    m_impulseToEvent[65683] = 48;
    m_impulseToEvent[65656] = 49;
    m_impulseToEvent[65685] = 50;
    m_impulseToEvent[65686] = 51;
    m_impulseToEvent[65687] = 52;
    m_impulseToEvent[65688] = 53;
    m_impulseToEvent[65689] = 54;
    m_impulseToEvent[65690] = 55;
    m_impulseToEvent[65691] = 56;
    m_impulseToEvent[66540] = 57;
    m_impulseToEvent[66537] = 58;
    m_impulseToEvent[66538] = 59;
    m_impulseToEvent[66539] = 60;
    m_impulseToEvent[66541] = 61;
    m_impulseToEvent[66542] = 62;
    m_impulseToEvent[66543] = 63;
    m_impulseToEvent[66544] = 64;
    m_impulseToEvent[66545] = 65;
    m_impulseToEvent[66546] = 66;
    m_impulseToEvent[66547] = 67;
    m_impulseToEvent[66548] = 68;
    m_impulseToEvent[66551] = 69;
    m_impulseToEvent[66554] = 70;
    m_impulseToEvent[66556] = 71;
    m_impulseToEvent[66557] = 72;
    m_impulseToEvent[66558] = 73;
    m_impulseToEvent[66553] = 74;
    m_impulseToEvent[66559] = 75;
    m_impulseToEvent[66561] = 76;
    m_impulseToEvent[66562] = 77;
    m_impulseToEvent[66565] = 78;
    m_impulseToEvent[66563] = 79;
    m_impulseToEvent[66564] = 80;
    m_impulseToEvent[66566] = 81;
    m_impulseToEvent[66567] = 82;
    m_impulseToEvent[66568] = 83;
}

void TruxxUiManager::GUI_RegisterClasses()
{
    //m3d::g_Kernel->AddClass(&BindKeysWnd::m_classBindKeysWnd);
    //m3d::g_Kernel->AddClass(&RepositoryWnd::m_classRepositoryWnd);
    //m3d::g_Kernel->AddClass(&IzvratRepositoryWnd::m_classIzvratRepositoryWnd);
    //m3d::g_Kernel->AddClass(&InventoryWnd::m_classInventoryWnd);
    //m3d::g_Kernel->AddClass(&DragDropItemsWnd::m_classDragDropItemsWnd);
    //m3d::g_Kernel->AddClass(&VehiclePartWnd::m_classVehiclePartWnd);
    //m3d::g_Kernel->AddClass(&DevideDlg::m_classDevideDlg);
    //m3d::g_Kernel->AddClass(&BarDlg::m_classBarDlg);
    //m3d::g_Kernel->AddClass(&ConversationWnd::m_classConversationWnd);
    //m3d::g_Kernel->AddClass(&TownDlg::m_classTownDlg);
    //m3d::g_Kernel->AddClass(&RadarWnd::m_classRadarWnd);
    //m3d::g_Kernel->AddClass(&CinemaPanel::m_classCinemaPanel);
    //m3d::g_Kernel->AddClass(&CinemaFadePanel::m_classCinemaFadePanel);
    //m3d::g_Kernel->AddClass(&DamageInfoWnd::m_classDamageInfoWnd);
    //m3d::g_Kernel->AddClass(&HealthIndicatorWnd::m_classHealthIndicatorWnd);
    //m3d::g_Kernel->AddClass(&HealthIndicatorInMainInterfaceWnd::m_classHealthIndicatorInMainInterfaceWnd);
    //m3d::g_Kernel->AddClass(&DurabilityIndicatorWnd::m_classDurabilityIndicatorWnd);
    //m3d::g_Kernel->AddClass(&DurabilityIndicatorInMainInterfaceWnd::m_classDurabilityIndicatorInMainInterfaceWnd);
    //m3d::g_Kernel->AddClass(&JournalWnd::m_classJournalWnd);
    //m3d::g_Kernel->AddClass(&HistoryWnd::m_classHistoryWnd);
    //m3d::g_Kernel->AddClass(&BooksWnd::m_classBooksWnd);
    //m3d::g_Kernel->AddClass(&EncyclopaediaWnd::m_classEncyclopaediaWnd);
    //m3d::g_Kernel->AddClass(&SubjectButton::m_classSubjectButton);
    //m3d::g_Kernel->AddClass(&SubjectList::m_classSubjectList);
    //m3d::g_Kernel->AddClass(&CheckList::m_classCheckList);
    //m3d::g_Kernel->AddClass(&CheckButton::m_classCheckButton);
    //m3d::g_Kernel->AddClass(&InfoWnd::m_classInfoWnd);
    //m3d::g_Kernel->AddClass(&ItemInfoWnd::m_classItemInfoWnd);
    m3d::g_Kernel->AddClass(&RepliesManager::m_classRepliesManager);
    //m3d::g_Kernel->AddClass(&FuelIndicatorWnd::m_classFuelIndicatorWnd);
    //m3d::g_Kernel->AddClass(&FuelIndicatorInMainInterfaceWnd::m_classFuelIndicatorInMainInterfaceWnd);
    //m3d::g_Kernel->AddClass(&SpeedometerWnd::m_classSpeedometerWnd);
    //m3d::g_Kernel->AddClass(&WeaponInfoWnd::m_classWeaponInfoWnd);
    //m3d::g_Kernel->AddClass(&WeaponInfoList::m_classWeaponInfoList);
    //m3d::g_Kernel->AddClass(&ItemWnd::m_classItemWnd);
    //m3d::g_Kernel->AddClass(&GadgetWnd::m_classGadgetWnd);
    //m3d::g_Kernel->AddClass(&MotherPanel::m_classMotherPanel);
    //m3d::g_Kernel->AddClass(&ChildPanel::m_classChildPanel);
    //m3d::g_Kernel->AddClass(&GroundWnd::m_classGroundWnd);
    //m3d::g_Kernel->AddClass(&SaleWnd::m_classSaleWnd);
    //m3d::g_Kernel->AddClass(&ShopWnd::m_classShopWnd);
    //m3d::g_Kernel->AddClass(&PlayerMoneyWnd::m_classPlayerMoneyWnd);
    //m3d::g_Kernel->AddClass(&MainMenuUI::m_classMainMenuUI);
    //m3d::g_Kernel->AddClass(&RepositoryItemWnd::m_classRepositoryItemWnd);
    //m3d::g_Kernel->AddClass(&ComplexModelWnd::m_classComplexModelWnd);
    //m3d::g_Kernel->AddClass(&VideoWnd::m_classVideoWnd);
    //m3d::g_Kernel->AddClass(&ItemModelWnd::m_classItemModelWnd);
    //m3d::g_Kernel->AddClass(&ScreenWnd::m_classScreenWnd);
    //m3d::g_Kernel->AddClass(&TakSebeQuestLogWnd::m_classTakSebeQuestLogWnd);
    //m3d::g_Kernel->AddClass(&QuestItem::m_classQuestItem);
    //m3d::g_Kernel->AddClass(&QuestItemList::m_classQuestItemList);
    //m3d::g_Kernel->AddClass(&QuestStatusBtn::m_classQuestStatusBtn);
    //m3d::g_Kernel->AddClass(&QuestLabel::m_classQuestLabel);
    //m3d::g_Kernel->AddClass(&QuestDizWnd::m_classQuestDizWnd);
    //m3d::g_Kernel->AddClass(&MapComboBox::m_classMapComboBox);
    //m3d::g_Kernel->AddClass(&LocalMapWnd::m_classLocalMapWnd);
    //m3d::g_Kernel->AddClass(&LocalChartWnd::m_classLocalChartWnd);
    //m3d::g_Kernel->AddClass(&MapMarkWnd::m_classMapMarkWnd);
    //m3d::g_Kernel->AddClass(&MapMarkIcoWnd::m_classMapMarkIcoWnd);
    //m3d::g_Kernel->AddClass(&MapMarkNameWnd::m_classMapMarkNameWnd);
    //m3d::g_Kernel->AddClass(&PlayerMarkWnd::m_classPlayerMarkWnd);
    //m3d::g_Kernel->AddClass(&MapInfoPanel::m_classMapInfoPanel);
    //m3d::g_Kernel->AddClass(&MapSellItem::m_classMapSellItem);
    //m3d::g_Kernel->AddClass(&MapSellList::m_classMapSellList);
    //m3d::g_Kernel->AddClass(&SaveSellList::m_classSaveSellList);
    //m3d::g_Kernel->AddClass(&SaveSellButton::m_classSaveSellButton);
    //m3d::g_Kernel->AddClass(&TalkWithNpcDlg::m_classTalkWithNpcDlg);
    //m3d::g_Kernel->AddClass(&NpcModelWnd::m_classNpcModelWnd);
    //m3d::g_Kernel->AddClass(&NavPointMarkWnd::m_classNavPointMarkWnd);
    //m3d::g_Kernel->AddClass(&WeaponGroupChoiceDlg::m_classWeaponGroupChoiceDlg);
    //m3d::g_Kernel->AddClass(&CBWnd::m_classCBWnd);
    //m3d::g_Kernel->AddClass(&CabinWnd::m_classCabinWnd);
    //m3d::g_Kernel->AddClass(&BasketWnd::m_classBasketWnd);
    //m3d::g_Kernel->AddClass(&ZnayuKakProdatWnd::m_classZnayuKakProdatWnd);
    //m3d::g_Kernel->AddClass(&BuildingButton::m_classBuildingButton);
    //m3d::g_Kernel->AddClass(&BuildingList::m_classBuildingList);
    //m3d::g_Kernel->AddClass(&NpcButton::m_classNpcButton);
    //m3d::g_Kernel->AddClass(&NpcList::m_classNpcList);
    //m3d::g_Kernel->AddClass(&ContextModelWnd::m_classContextModelWnd);
    m3d::g_Kernel->AddClass(&LevelInfoManager::m_classLevelInfoManager);
    //m3d::g_Kernel->AddClass(&GlobalMapWnd::m_classGlobalMapWnd);
    //m3d::g_Kernel->AddClass(&LevelIco::m_classLevelIco);
    //m3d::g_Kernel->AddClass(&StatsWnd::m_classStatsWnd);
    //m3d::g_Kernel->AddClass(&StatsButton::m_classStatsButton);
    //m3d::g_Kernel->AddClass(&StatsList::m_classStatsList);
    //m3d::g_Kernel->AddClass(&ReputationWnd::m_classReputationWnd);
    //m3d::g_Kernel->AddClass(&ReputationButton::m_classReputationButton);
    //m3d::g_Kernel->AddClass(&ReputationList::m_classReputationList);
    //m3d::g_Kernel->AddClass(&SplashWnd::m_classSplashWnd);
    //m3d::g_Kernel->AddClass(&NewProfileWnd::m_classNewProfileWnd);
    //m3d::g_Kernel->AddClass(&ChangeProfileWnd::m_classChangeProfileWnd);
    //m3d::g_Kernel->AddClass(&LSWnd::m_classLSWnd);
    //m3d::g_Kernel->AddClass(&SaveButton::m_classSaveButton);
    //m3d::g_Kernel->AddClass(&SaveList::m_classSaveList);
    //m3d::g_Kernel->AddClass(&SaveWnd::m_classSaveWnd);
    //m3d::g_Kernel->AddClass(&LoadWnd::m_classLoadWnd);
    m3d::g_Kernel->AddClass(&SavesManager::m_classSavesManager);
    //m3d::g_Kernel->AddClass(&TargetInfoWnd::m_classTargetInfoWnd);
    //m3d::g_Kernel->AddClass(&SmartCursorWnd::m_classSmartCursorWnd);
    //m3d::g_Kernel->AddClass(&VehicleInfoPanel::m_classVehicleInfoPanel);
    //m3d::g_Kernel->AddClass(&FadingMsgList::m_classFadingMsgList);
    //m3d::g_Kernel->AddClass(&FadingMsgItem::m_classFadingMsgItem);
    m3d::g_Kernel->AddClass(&MsgManager::m_classMsgManager);
    //m3d::g_Kernel->AddClass(&GarageWnd::m_classGarageWnd);
    //m3d::g_Kernel->AddClass(&AdvancedButton::m_classAdvancedButton);
    //m3d::g_Kernel->AddClass(&AdvancedList::m_classAdvancedList);
    //m3d::g_Kernel->AddClass(&RefuelButton::m_classRefuelButton);
    //m3d::g_Kernel->AddClass(&RefuelList::m_classRefuelList);
    //m3d::g_Kernel->AddClass(&RepairButton::m_classRepairButton);
    //m3d::g_Kernel->AddClass(&RepairList::m_classRepairList);
    //m3d::g_Kernel->AddClass(&RechargeButton::m_classRechargeButton);
    //m3d::g_Kernel->AddClass(&RechargeList::m_classRechargeList);
    //m3d::g_Kernel->AddClass(&BuyServiceSlider::m_classBuyServiceSlider);
    //m3d::g_Kernel->AddClass(&CBButton::m_classCBButton);
    //m3d::g_Kernel->AddClass(&CBList::m_classCBList);
    //m3d::g_Kernel->AddClass(&CabinButton::m_classCabinButton);
    //m3d::g_Kernel->AddClass(&CabinList::m_classCabinList);
    //m3d::g_Kernel->AddClass(&BasketButton::m_classBasketButton);
    //m3d::g_Kernel->AddClass(&BasketList::m_classBasketList);
    //m3d::g_Kernel->AddClass(&VehicleWnd::m_classVehicleWnd);
    //m3d::g_Kernel->AddClass(&PlayerVehicleWnd::m_classPlayerVehicleWnd);
    //m3d::g_Kernel->AddClass(&WorkshopVehicleWnd::m_classWorkshopVehicleWnd);
    //m3d::g_Kernel->AddClass(&SkinsWnd::m_classSkinsWnd);
    //m3d::g_Kernel->AddClass(&IgrokaMochatWnd::m_classIgrokaMochatWnd);
    //m3d::g_Kernel->AddClass(&QuestItemsWnd::m_classQuestItemsWnd);
    //m3d::g_Kernel->AddClass(&WareWnd::m_classWareWnd);
    //m3d::g_Kernel->AddClass(&WareList::m_classWareList);
    //m3d::g_Kernel->AddClass(&WareItem::m_classWareItem);
    //m3d::g_Kernel->AddClass(&MotherPanelTabButton::m_classMotherPanelTabButton);
    //m3d::g_Kernel->AddClass(&MainCursorWnd::m_classMainCursorWnd);
    //m3d::g_Kernel->AddClass(&TargetCursorWnd::m_classTargetCursorWnd);
    //m3d::g_Kernel->AddClass(&TargetCapturingCursorWnd::m_classTargetCapturingCursorWnd);
    //m3d::g_Kernel->AddClass(&TargetCapturedCursorWnd::m_classTargetCapturedCursorWnd);
    //m3d::g_Kernel->AddClass(&CounterWnd::m_classCounterWnd);
    //m3d::g_Kernel->AddClass(&OptionsWnd::m_classOptionsWnd);
    //m3d::g_Kernel->AddClass(&OptionTabButton::m_classOptionTabButton);
    //m3d::g_Kernel->AddClass(&VideoOptionsWnd::m_classVideoOptionsWnd);
    //m3d::g_Kernel->AddClass(&SoundOptionsWnd::m_classSoundOptionsWnd);
    //m3d::g_Kernel->AddClass(&GameOptionsWnd::m_classGameOptionsWnd);
    //m3d::g_Kernel->AddClass(&ControlOptionsWnd::m_classControlOptionsWnd);
    //m3d::g_Kernel->AddClass(&NavPointButton::m_classNavPointButton);
    m3d::g_Kernel->AddClass(&WeaponGroupManager::m_classWeaponGroupManager);
    //m3d::g_Kernel->AddClass(&WeaponGroup::m_classWeaponGroup);
    //m3d::g_Kernel->AddClass(&GameMenuWnd::m_classGameMenuWnd);
    //m3d::g_Kernel->AddClass(&MainGameInterfaceWnd::m_classMainGameInterfaceWnd);
    //m3d::g_Kernel->AddClass(&ElectronicDigitalWnd::m_classElectronicDigitalWnd);
    //m3d::g_Kernel->AddClass(&TwinklingLampWnd::m_classTwinklingLampWnd);
    //m3d::g_Kernel->AddClass(&CharacteristicsWnd::m_classCharacteristicsWnd);
    //m3d::g_Kernel->AddClass(&CBCharacteristicsWnd::m_classCBCharacteristicsWnd);
    //m3d::g_Kernel->AddClass(&CabinCharacteristicsWnd::m_classCabinCharacteristicsWnd);
    //m3d::g_Kernel->AddClass(&BasketCharacteristicsWnd::m_classBasketCharacteristicsWnd);
    //m3d::g_Kernel->AddClass(&VehicleCharacteristicsWnd::m_classVehicleCharacteristicsWnd);
    //m3d::g_Kernel->AddClass(&WeaponSlotWnd::m_classWeaponSlotWnd);
    //m3d::g_Kernel->AddClass(&WeaponSlotList::m_classWeaponSlotList);
    //m3d::g_Kernel->AddClass(&WeaponGroupButton::m_classWeaponGroupButton);
    //m3d::g_Kernel->AddClass(&WeaponGroupButtonList::m_classWeaponGroupButtonList);
    //m3d::g_Kernel->AddClass(&DemoSplashWnd::m_classDemoSplashWnd);
    //m3d::g_Kernel->AddClass(&CreditsWnd::m_classCreditsWnd);
    //m3d::g_Kernel->AddClass(&FadePanelBeforeNextMap::m_classFadePanelBeforeNextMap);
    //m3d::g_Kernel->AddClass(&LevelConnectionsWnd::m_classLevelConnectionsWnd);
    m3d::g_Kernel->AddClass(&HelpManager::m_classHelpManager);
    //m3d::g_Kernel->AddClass(&HelpWnd::m_classHelpWnd);
    //m3d::g_Kernel->AddClass(&HelpSimpleMsgWnd::m_classHelpSimpleMsgWnd);
    //m3d::g_Kernel->AddClass(&ResourceInfo::m_classResourceInfo);
    //m3d::g_Kernel->AddClass(&WindowResourceInfo::m_classWindowResourceInfo);
    //m3d::g_Kernel->AddClass(&IcoResourceInfo::m_classIcoResourceInfo);
    //m3d::g_Kernel->AddClass(&AutoScrollTextWnd::m_classAutoScrollTextWnd);
    //m3d::g_Kernel->AddClass(&HelpInMainGameInterfaceWnd::m_classHelpInMainGameInterfaceWnd);
    //m3d::g_Kernel->AddClass(&MsgBox::m_classMsgBox);
    //m3d::g_Kernel->AddClass(&BossIndicatorWnd::m_classBossIndicatorWnd);
    //m3d::g_Kernel->AddClass(&RequestDifficultyWnd::m_classRequestDifficultyWnd);
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
