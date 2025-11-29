#include "helpmanager.h"
#include <stdexcept>

#include "game/m3dgame.h"

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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(HelpManager, ResetAutoHelp)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

CStr HelpManager::Enumered2StrHelpId(HelpId)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpManager::ShowHelp(CStr const&, bool)
{
    // TODO: implement HelpManager::ShowHelp
    // RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* HelpManager::CreateObject()
{
    return new HelpManager;
}

HelpManager::HelpId HelpManager::Str2EnumeredHelpId(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpManager::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

CStr HelpManager::GetHelpTitleByHelpId(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpManager::~HelpManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool HelpManager::IsAutoHelpEnabled() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpManager::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

HelpManager::HelpManager()
{
    m_predefinedHelpWindows.emplace(HELP_ID_MAIN_GAME_INTERFACE, 169);
}

HelpManager::HelpManager(HelpManager const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpManager::ShowHelpEnumered(HelpId, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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

void HelpManager::MarkHelpAsShown(HelpId)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ProfileParam HelpManager::GetProfileParamIdByHelpId(HelpId) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpManager::GetPredefinedWndGuiIdByHelpId(HelpId) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HelpManager::GetCurHelpWndGuiId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpManager::GetAllHelpWndGuiIds(std::vector<int>& guidIds) const
{
    guidIds.clear();
    // TODO: implement HelpManager::GetAllHelpWndGuiIds
}

void HelpManager::OnHelpImpulse()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HelpManager::HelpId HelpManager::CalcHelpIdForCurrentInterfaceState() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool HelpManager::IsHelpIdValidForCurrentInterfaceState(HelpId) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool HelpManager::WasHelpShown(HelpId) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HelpManager::OnHelpEvent(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
