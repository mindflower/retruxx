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
