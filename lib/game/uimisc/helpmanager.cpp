#include "helpmanager.h"
#include <stdexcept>

#include "game/m3dgame.h"
#include "game/uiwindows/charwindows/garagewnd.h"
#include "game/uiwindows/charwindows/motherpanel.h"
#include "game/uiwindows/charwindows/znayukakprodatwnd.h"
#include "game/uiwindows/miscwindows/helpsimplemsgwnd.h"
#include "game/uiwindows/miscwindows/helpwnd.h"
#include "server/event.h"

namespace
{
    struct _HelpId2ProfileParamId
    {
        HelpManager::HelpId m_helpId;
        ProfileParam m_profileParamId;
    };

    _HelpId2ProfileParamId l_helpId2ProfileParamId[] = {
        {HelpManager::HELP_ID_MAIN_GAME_INTERFACE, PP_HELP_ID_MAIN_GAME_INTERFACE },
        {HelpManager::HELP_ID_INVENTORY, PP_HELP_ID_INVENTORY },
        {HelpManager::HELP_ID_VEHICLE_CHARACTERISTICS, PP_HELP_ID_VEHICLE_CHARACTERISTICS },
        {HelpManager::HELP_ID_QUESTLOG_QUESTS, PP_HELP_ID_QUESTLOG_QUESTS },
        {HelpManager::HELP_ID_QUESTLOG_QUEST_ITEMS, PP_HELP_ID_QUESTLOG_QUEST_ITEMS },
        {HelpManager::HELP_ID_LOCAL_MAP, PP_HELP_ID_LOCAL_MAP},
        {HelpManager::HELP_ID_GLOBAL_MAP, PP_HELP_ID_GLOBAL_MAP},
        {HelpManager::HELP_ID_HISTORY, PP_HELP_ID_HISTORY},
        {HelpManager::HELP_ID_BOOKS, PP_HELP_ID_BOOKS},
        {HelpManager::HELP_ID_RELATIONS, PP_HELP_ID_RELATIONS},
        {HelpManager::HELP_ID_STATS, PP_HELP_ID_STATS},
        {HelpManager::HELP_ID_ENCYCLOPAEDIA, PP_HELP_ID_ENCYCLOPAEDIA},
        {HelpManager::HELP_ID_SHOP_GUNS, PP_HELP_ID_SHOP_GUNS},
        {HelpManager::HELP_ID_SHOP_GOODS, PP_HELP_ID_SHOP_GOODS},
        {HelpManager::HELP_ID_WORKSHOP, PP_HELP_ID_WORKSHOP},
        {HelpManager::HELP_ID_WORKSHOP_CABINS, PP_HELP_ID_WORKSHOP_CABINS},
        {HelpManager::HELP_ID_WORKSHOP_BASKETS, PP_HELP_ID_WORKSHOP_BASKETS},
        {HelpManager::HELP_ID_WORKSHOP_VEHICLE, PP_HELP_ID_WORKSHOP_VEHICLE},
        {HelpManager::HELP_ID_WORKSHOP_SKIN, PP_HELP_ID_WORKSHOP_SKIN},
        {HelpManager::HELP_ID_WORKSHOP_REFUEL, PP_HELP_ID_WORKSHOP_REFUEL},
        {HelpManager::HELP_ID_WORKSHOP_REPAIR, PP_HELP_ID_WORKSHOP_REPAIR},
        {HelpManager::HELP_ID_WORKSHOP_RECHARGE, PP_HELP_ID_WORKSHOP_RECHARGE},
        {HelpManager::HELP_ID_BUY_CABIN, PP_HELP_ID_BUY_CABIN},
        {HelpManager::HELP_ID_BUY_BASKET, PP_HELP_ID_BUY_BASKET},
        {HelpManager::HELP_ID_BUY_VEHICLE, PP_HELP_ID_BUY_VEHICLE},
        {HelpManager::HELP_ID_TOWN, PP_HELP_ID_TOWN},
        {HelpManager::HELP_ID_BAR, PP_HELP_ID_BAR},
        {HelpManager::HELP_ID_TALK_WITH_NPC, PP_HELP_ID_TALK_WITH_NPC},
    };

    // Help id names as scripts and window definitions spell them; the order and
    // spelling follow the binary's static initializer.
    struct _Str2EnumeredHelpId
    {
        CStr m_strHelpId;
        HelpManager::HelpId m_helpId;
    };

    _Str2EnumeredHelpId l_str2EnumeredHelpId[] = {
        {"HELP_ID_MAIN_GAME_INTERFACE", HelpManager::HELP_ID_MAIN_GAME_INTERFACE},
        {"HELP_ID_INVENTORY", HelpManager::HELP_ID_INVENTORY},
        {"HELP_ID_VEHICLE_CHARACTERISTICS", HelpManager::HELP_ID_VEHICLE_CHARACTERISTICS},
        {"HELP_ID_QUESTLOG_QUESTS", HelpManager::HELP_ID_QUESTLOG_QUESTS},
        {"HELP_ID_QUESTLOG_QUEST_ITEMS", HelpManager::HELP_ID_QUESTLOG_QUEST_ITEMS},
        {"HELP_ID_LOCAL_MAP", HelpManager::HELP_ID_LOCAL_MAP},
        {"HELP_ID_GLOBAL_MAP", HelpManager::HELP_ID_GLOBAL_MAP},
        {"HELP_ID_HISTORY", HelpManager::HELP_ID_HISTORY},
        {"HELP_ID_BOOKS", HelpManager::HELP_ID_BOOKS},
        {"HELP_ID_RELATIONS", HelpManager::HELP_ID_RELATIONS},
        {"HELP_ID_STATS", HelpManager::HELP_ID_STATS},
        {"HELP_ID_ENCYCLOPAEDIA", HelpManager::HELP_ID_ENCYCLOPAEDIA},
        {"HELP_ID_SHOP_GUNS", HelpManager::HELP_ID_SHOP_GUNS},
        {"HELP_ID_SHOP_GOODS", HelpManager::HELP_ID_SHOP_GOODS},
        {"HELP_ID_WORKSHOP", HelpManager::HELP_ID_WORKSHOP},
        {"HELP_ID_WORKSHOP_CABINS", HelpManager::HELP_ID_WORKSHOP_CABINS},
        {"HELP_ID_WORKSHOP_BASKETS", HelpManager::HELP_ID_WORKSHOP_BASKETS},
        {"HELP_ID_WORKSHOP_VEHICLE", HelpManager::HELP_ID_WORKSHOP_VEHICLE},
        {"HELP_ID_WORKSHOP_SKIN", HelpManager::HELP_ID_WORKSHOP_SKIN},
        {"HELP_ID_WORKSHOP_REFUEL", HelpManager::HELP_ID_WORKSHOP_REFUEL},
        {"HELP_ID_WORKSHOP_REPAIR", HelpManager::HELP_ID_WORKSHOP_REPAIR},
        {"HELP_ID_WORKSHOP_RECHARGE", HelpManager::HELP_ID_WORKSHOP_RECHARGE},
        {"HELP_ID_BUY_CABIN", HelpManager::HELP_ID_BUY_CABIN},
        {"HELP_ID_BUY_BASKET", HelpManager::HELP_ID_BUY_BASKET},
        {"HELP_ID_BUY_VEHICLE", HelpManager::HELP_ID_BUY_VEHICLE},
        {"HELP_ID_TOWN", HelpManager::HELP_ID_TOWN},
        {"HELP_ID_BAR", HelpManager::HELP_ID_BAR},
        {"HELP_ID_TALK_WITH_NPC", HelpManager::HELP_ID_TALK_WITH_NPC},
        {"HELP_ID_RADAR", HelpManager::HELP_ID_RADAR},
        {"HELP_ID_SPEEDOMETER", HelpManager::HELP_ID_SPEEDOMETER},
        {"HELP_ID_HP", HelpManager::HELP_ID_HP},
        {"HELP_ID_FUEL", HelpManager::HELP_ID_FUEL},
        {"HELP_ID_DURABILITY", HelpManager::HELP_ID_DURABILITY},
        {"HELP_ID_TIMER", HelpManager::HELP_ID_TIMER},
        {"HELP_ID_WEAPON", HelpManager::HELP_ID_WEAPON},
        {"HELP_ID_FADING_MESSAGES", HelpManager::HELP_ID_FADING_MESSAGES},
        {"HELP_ID_IMPORTANT_FADING_MESSAGES", HelpManager::HELP_ID_IMPORTANT_FADING_MESSAGES},
        {"HELP_ID_TARGET_INFO", HelpManager::HELP_ID_TARGET_INFO},
        {"HELP_ID_TARGET_CAPTURE", HelpManager::HELP_ID_TARGET_CAPTURE},
        {"HELP_ID_ACTION_ICONS", HelpManager::HELP_ID_ACTION_ICONS},
        {"HELP_ID_HOW_TO_USE_MAIN_GAME_INTERFACE_HELP", HelpManager::HELP_ID_HOW_TO_USE_MAIN_GAME_INTERFACE_HELP},
    };

    constexpr int HELP_WND_GUI_ID_DEFAULT = 168;
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, ShowHelp)
{
    auto helpManager = (HelpManager*)context->asObject(0, "HelpManager");
    auto helpId = context->asString(1);
    auto allowDisable = context->asBool(2);
    helpManager->ShowHelp(helpId, allowDisable);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, EnableAutoHelp)
{
    auto helpManager = (HelpManager*)context->asObject(0, "HelpManager");
    auto enable = context->asBool(1);
    helpManager->EnableAutoHelp(enable);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, IsAutoHelpEnabled)
{
    // RVA 0x558C70
    auto helpManager = (HelpManager*)context->asObject(0, "HelpManager");
    context->pushBool(helpManager->IsAutoHelpEnabled());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, ResetAutoHelp)
{
    // RVA 0x558CA0
    auto helpManager = (HelpManager*)context->asObject(0, "HelpManager");
    helpManager->ResetAutoHelp();
    return 1;
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
    // RVA 0x558D00 - help titles are looked up as "<help id>_title".
    m_strTitleSuffix = "_title";
}

CStr HelpManager::GetHelpMsgByHelpId(CStr const& strHelpId) const
{
    // RVA 0x559C70 - the help id doubles as the string id of its body text.
    if (strHelpId.empty())
    {
        return CStr();
    }
    return M3D_APP->GetStringByStringId0(strHelpId);
}

CStr HelpManager::Enumered2StrHelpId(HelpId helpId)
{
    // RVA 0x559350
    for (auto const& entry : l_str2EnumeredHelpId)
    {
        if (entry.m_helpId == helpId)
        {
            return entry.m_strHelpId;
        }
    }
    return CStr();
}

void HelpManager::ShowHelp(CStr const& strHelpId, bool bAllowDisableAutoHelp)
{
    // RVA 0x559060
    int wndGuiId = HELP_WND_GUI_ID_DEFAULT;
    auto const it = m_predefinedHelpWindows.find(Str2EnumeredHelpId(strHelpId));
    if (it != m_predefinedHelpWindows.end() && it->second != -1)
    {
        wndGuiId = it->second;
    }

    auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(wndGuiId);
    if (!wnd)
    {
        return;
    }

    if (wnd->IsKindOf(&HelpSimpleMsgWnd::m_classHelpSimpleMsgWnd))
    {
        static_cast<HelpSimpleMsgWnd*>(static_cast<m3d::ui::Wnd*>(wnd))->SetupForHelp(strHelpId, bAllowDisableAutoHelp);
    }
    else if (wnd->IsKindOf(&HelpWnd::m_classHelpWnd))
    {
        static_cast<HelpWnd*>(static_cast<m3d::ui::Wnd*>(wnd))->AllowDisableAutoHelp(bAllowDisableAutoHelp);
    }

    if (GetCurHelpWndGuiId() != wndGuiId)
    {
        HideCurrentHelpWindow();
        M3D_APP->m_pInterfaceManager->ShowWindow(wndGuiId, true, false, true, true, nullptr);
        MarkHelpAsShown(Str2EnumeredHelpId(strHelpId));
    }
}

m3d::Object* HelpManager::CreateObject()
{
    return new HelpManager;
}

HelpManager::HelpId HelpManager::Str2EnumeredHelpId(CStr const& strHelpId)
{
    // RVA 0x5592C0
    for (auto const& entry : l_str2EnumeredHelpId)
    {
        if (entry.m_strHelpId == strHelpId)
        {
            return entry.m_helpId;
        }
    }
    return HELP_ID_INVALID;
}

void HelpManager::Clear()
{
    // RVA 0x558F30
    HideCurrentHelpWindow();
}

m3d::Class* HelpManager::GetClass() const
{
    return RT_CLASS_LOCAL(HelpManager);
}

void HelpManager::ResetAutoHelp()
{
    if (auto const profile = M3D_APP->GetProfileManager()->GetCurProfile())
    {
        m3d::AIParam const param{0};
        for (auto const& helpId : l_helpId2ProfileParamId )
        {
            profile->SetParam(helpId.m_profileParamId, param);
        }
    }
}

m3d::Class* HelpManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

void HelpManager::EnableAutoHelp(bool enable)
{
    if (auto const profile = M3D_APP->GetProfileManager()->GetCurProfile())
    {
        m3d::AIParam const param{static_cast<int>(enable)};
        profile->SetParam(PP_AUTOHELP_ENABLED, param);
    }
}

m3d::Object* HelpManager::Clone()
{
    // RVA 0x558B80
    return new HelpManager(*this);
}

CStr HelpManager::GetHelpTitleByHelpId(CStr const& strHelpId) const
{
    // RVA 0x559CC0 - the title lives under the help id plus a fixed suffix.
    if (strHelpId.empty())
    {
        return CStr();
    }
    return M3D_APP->GetStringByStringId0(strHelpId + m_aif.m_strTitleSuffix);
}

HelpManager::~HelpManager()
{
    // RVA 0x558EA0
    HideCurrentHelpWindow();
}

bool HelpManager::IsAutoHelpEnabled() const
{
    // RVA 0x559A60
    auto const* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return false;
    }
    m3d::AIParam paramVal;
    profile->GetParam(PP_AUTOHELP_ENABLED, paramVal);
    return paramVal.GetAsID() != 0;
}

int HelpManager::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x558F40
    if (dataType == IE_IMP_IM_UI_HELP)
    {
        OnHelpImpulse();
    }
    else if (dataType == IE_EV_UM_HELP)
    {
        OnHelpEvent(data);
    }
    return 1;
}

HelpManager::HelpManager()
{
    m_predefinedHelpWindows.emplace(HELP_ID_MAIN_GAME_INTERFACE, 169);
}

HelpManager::HelpManager(HelpManager const&) :
    m3d::Object()
{
    // RVA 0x558E60 - nothing is copied, and unlike the default constructor the
    // predefined main-interface help window is not registered.
}

void HelpManager::ShowHelpEnumered(HelpId helpId, bool bAllowDisableAutoHelp)
{
    // RVA 0x559020
    if (helpId != HELP_ID_INVALID)
    {
        ShowHelp(Enumered2StrHelpId(helpId), bAllowDisableAutoHelp);
    }
}

void HelpManager::HideCurrentHelpWindow()
{
    std::vector<int> helpWndGuiIds;
    GetAllHelpWndGuiIds(helpWndGuiIds);
    for (auto& wnd : helpWndGuiIds)
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(wnd, false, false, false, false, nullptr);
    }
}

void HelpManager::MarkHelpAsShown(HelpId helpId)
{
    // RVA 0x559EA0
    auto const paramId = GetProfileParamIdByHelpId(helpId);
    if (paramId == PP_NUM_PROFILE_PARAMS)
    {
        return;
    }
    if (auto* profile = M3D_APP->GetProfileManager()->GetCurProfile())
    {
        profile->SetParam(paramId, m3d::AIParam(1));
    }
}

ProfileParam HelpManager::GetProfileParamIdByHelpId(HelpId helpId) const
{
    // RVA 0x559DF0
    for (auto const& entry : l_helpId2ProfileParamId)
    {
        if (entry.m_helpId == helpId)
        {
            return entry.m_profileParamId;
        }
    }
    return PP_NUM_PROFILE_PARAMS;
}

int HelpManager::GetPredefinedWndGuiIdByHelpId(HelpId helpId) const
{
    // RVA 0x559260
    auto const it = m_predefinedHelpWindows.find(helpId);
    return it != m_predefinedHelpWindows.end() ? it->second : -1;
}

int HelpManager::GetCurHelpWndGuiId() const
{
    // RVA 0x559AE0 - the first help window currently shown, if any.
    std::vector<int> allHelpWndGuiIds;
    GetAllHelpWndGuiIds(allHelpWndGuiIds);
    for (auto const guiId : allHelpWndGuiIds)
    {
        if (M3D_APP->m_pInterfaceManager->IsWindowVisible(guiId))
        {
            return guiId;
        }
    }
    return -1;
}

void HelpManager::GetAllHelpWndGuiIds(std::vector<int>& guiIds) const
{
    // RVA 0x559B80 - every predefined help window, then the default one.
    guiIds.clear();
    for (auto const& [helpId, guiId] : m_predefinedHelpWindows)
    {
        guiIds.push_back(guiId);
    }
    guiIds.push_back(HELP_WND_GUI_ID_DEFAULT);
}

void HelpManager::OnHelpImpulse()
{
    // RVA 0x558F90 - the help key toggles help for whatever is on screen.
    if (GetCurHelpWndGuiId() != -1)
    {
        HideCurrentHelpWindow();
        return;
    }
    auto const helpId = CalcHelpIdForCurrentInterfaceState();
    if (helpId != HELP_ID_INVALID)
    {
        ShowHelp(Enumered2StrHelpId(helpId), false);
    }
}

HelpManager::HelpId HelpManager::CalcHelpIdForCurrentInterfaceState() const
{
    // RVA 0x5591C0 - the first help id that applies to the current screen.
    for (int helpId = HELP_ID_MAIN_GAME_INTERFACE; helpId < NUM_HELP_IDS; ++helpId)
    {
        if (IsHelpIdValidForCurrentInterfaceState(static_cast<HelpId>(helpId)))
        {
            return static_cast<HelpId>(helpId);
        }
    }
    return HELP_ID_INVALID;
}

bool HelpManager::IsHelpIdValidForCurrentInterfaceState(HelpId helpId) const
{
    // RVA 0x559390
    auto* ui = M3D_APP->m_pInterfaceManager;
    switch (helpId)
    {
    case HELP_ID_MAIN_GAME_INTERFACE:
        return ui->IsWindowVisibleAndNotAnimating(155) && !ui->IsModalEqualWndRunning();
    case HELP_ID_INVENTORY:
        return ui->IsWindowVisibleAndNotAnimating(63);
    case HELP_ID_VEHICLE_CHARACTERISTICS:
        return ui->IsWindowVisibleAndNotAnimating(69);
    case HELP_ID_QUESTLOG_QUESTS:
        return ui->IsWindowVisibleAndNotAnimating(78);
    case HELP_ID_QUESTLOG_QUEST_ITEMS:
        return ui->IsWindowVisibleAndNotAnimating(146);
    case HELP_ID_LOCAL_MAP:
        return ui->IsWindowVisibleAndNotAnimating(82);
    case HELP_ID_GLOBAL_MAP:
        return ui->IsWindowVisibleAndNotAnimating(84);
    case HELP_ID_HISTORY:
        return ui->IsWindowVisibleAndNotAnimating(95);
    case HELP_ID_BOOKS:
        return ui->IsWindowVisibleAndNotAnimating(96);
    case HELP_ID_RELATIONS:
        return ui->IsWindowVisibleAndNotAnimating(106);
    case HELP_ID_STATS:
        return ui->IsWindowVisibleAndNotAnimating(105);
    case HELP_ID_ENCYCLOPAEDIA:
        return ui->IsWindowVisibleAndNotAnimating(97);
    case HELP_ID_SHOP_GUNS:
        return ui->IsWindowVisibleAndNotAnimating(32);
    case HELP_ID_SHOP_GOODS:
        return ui->IsWindowVisibleAndNotAnimating(35);
    case HELP_ID_WORKSHOP:
    {
        // The workshop root, with no sub-list open and no trade dialog up.
        auto wnd = ui->GetWindow(67);
        return wnd && wnd->IsKindOf(&GarageWnd::m_classGarageWnd) && wnd->IsChildOf(M3D_APP) &&
            !static_cast<GarageWnd*>(static_cast<m3d::ui::Wnd*>(wnd))->m_curList &&
            !ui->IsWindowVisibleAndNotAnimating(94);
    }
    case HELP_ID_WORKSHOP_CABINS:
        return ui->IsWindowVisibleAndNotAnimating(133) && !ui->IsWindowVisibleAndNotAnimating(94);
    case HELP_ID_WORKSHOP_BASKETS:
        return ui->IsWindowVisibleAndNotAnimating(134) && !ui->IsWindowVisibleAndNotAnimating(94);
    case HELP_ID_WORKSHOP_VEHICLE:
        return ui->IsWindowVisibleAndNotAnimating(73) && !ui->IsWindowVisibleAndNotAnimating(94);
    case HELP_ID_WORKSHOP_SKIN:
        return ui->IsWindowVisibleAndNotAnimating(144);
    case HELP_ID_WORKSHOP_REFUEL:
        return ui->IsWindowVisibleAndNotAnimating(128);
    case HELP_ID_WORKSHOP_REPAIR:
        return ui->IsWindowVisibleAndNotAnimating(129);
    case HELP_ID_WORKSHOP_RECHARGE:
        return ui->IsWindowVisibleAndNotAnimating(130);
    case HELP_ID_BUY_CABIN:
    case HELP_ID_BUY_BASKET:
    case HELP_ID_BUY_VEHICLE:
    {
        auto const tradeType = helpId == HELP_ID_BUY_CABIN
            ? ZnayuKakProdatWnd::TRADETYPE_CABIN
            : (helpId == HELP_ID_BUY_BASKET ? ZnayuKakProdatWnd::TRADETYPE_BASKET : ZnayuKakProdatWnd::TRADETYPE_VEHICLE);
        auto wnd = ui->GetWindow(94);
        return wnd && wnd->IsKindOf(&ZnayuKakProdatWnd::m_classZnayuKakProdatWnd) && wnd->IsChildOf(M3D_APP) &&
            static_cast<ZnayuKakProdatWnd*>(static_cast<m3d::ui::Wnd*>(wnd))->m_tradeType == tradeType;
    }
    case HELP_ID_TOWN:
    {
        auto wnd = ui->GetWindow(7);
        return wnd && wnd->IsKindOf(&MotherPanel::m_classMotherPanel) && wnd->IsChildOf(M3D_APP) &&
            static_cast<MotherPanel*>(static_cast<m3d::ui::Wnd*>(wnd))->IsInTownRoot();
    }
    case HELP_ID_BAR:
        return ui->IsWindowVisibleAndNotAnimating(2) || ui->IsWindowVisibleAndNotAnimating(3);
    case HELP_ID_TALK_WITH_NPC:
        return ui->IsWindowVisibleAndNotAnimating(88);
    default:
        return false;
    }
}

bool HelpManager::WasHelpShown(HelpId helpId) const
{
    // RVA 0x559D40
    auto const paramId = GetProfileParamIdByHelpId(helpId);
    if (paramId == PP_NUM_PROFILE_PARAMS)
    {
        return false;
    }
    auto const* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return false;
    }
    m3d::AIParam paramVal;
    profile->GetParam(paramId, paramVal);
    return paramVal.GetAsID() != 0;
}

void HelpManager::OnHelpEvent(void*)
{
    // RVA 0x558FF0 - automatic help, shown once per screen.
    if (IsAutoHelpEnabled())
    {
        auto const helpId = CalcHelpIdForCurrentInterfaceState();
        if (!WasHelpShown(helpId))
        {
            ShowHelpEnumered(helpId, true);
        }
    }
}
