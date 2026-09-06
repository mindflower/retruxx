#include "navpointbutton.h"

#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimisc/questinfo.h>
#include <m3dapp.h>
#include <server/quest.h>
#include <ui/ui_srv.h>

RT_CLASS_EXPORTS_BEGIN(NavPointButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NavPointButton);

NavPointButton::AuxInfo NavPointButton::m_aif;

NavPointButton::AuxInfo::AuxInfo()
{
    // RVA 0x4F5490
    m_strNpButton = "QuestLogNavPointBtn_";
    m_strTypeMainQuest = "mainQuest";
    m_strTypeUserQuest = "userQuest";
    m_strTypeUserLocation = "userLocation";
    m_strIdAddNavPoint = "AddNavPoint";
    m_strIdDeleteNavPoint = "DeleteNavPoint";
    m_texIdOut = "NavPointBtnBg_out";
    m_texIdIn = "NavPointBtnBg_in";
    m_texIdDown = "NavPointBtnBg_down";
    m_npTexSz.x = 16.0f;
    m_npTexSz.y = 16.0f;
}

NavPointButton::NavPointButton() : m_npId(-1), m_npType(NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
{
}

NavPointButton::NavPointButton(NavPointButton const&) : NavPointButton()
{
}

NavPointButton::~NavPointButton()
{
    M3D_RENDERER->ReleaseTexture(m_npTex);
}

m3d::Object* NavPointButton::Clone()
{
    return new NavPointButton(*this);
}

m3d::Object* NavPointButton::CreateObject()
{
    return new NavPointButton;
}

m3d::Class* NavPointButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* NavPointButton::GetClass() const
{
    return RT_CLASS_LOCAL(NavPointButton);
}

int NavPointButton::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
{
    // RVA 0x4F57C0 - the button's three background states come from the
    // interface manager rather than from XML.
    int const result = ButtonWnd::Create(caption, style, rc, id);
    if (!result)
    {
        return result;
    }
    auto* im = M3D_APP->m_pInterfaceManager;
    m3d::rend::TexHandle const texOut = im->GetIcoByName(m_aif.m_texIdOut, 0);
    m3d::rend::TexHandle const texIn = im->GetIcoByName(m_aif.m_texIdIn, 0);
    m3d::rend::TexHandle const texDown = im->GetIcoByName(m_aif.m_texIdDown, 0);
    SetImaged(texOut, texDown, texIn, m3d::rend::TexHandle{});
    return 1;
}

int NavPointButton::GameDataClear(bool)
{
    // RVA 0x4F5880
    m_npType = NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES;
    m_npId = -1;
    return 1;
}

int NavPointButton::SetupForQuest(help::QuestType questType, int questId)
{
    // RVA 0x4F58A0
    if (!Valid())
    {
        return 0;
    }
    GameDataClear(false);
    if (questType == help::QUESTTYPE_NUM_QUEST_TYPES || questId == -1)
    {
        return 0;
    }

    m_npType = GetNavPointTypeByQuest(questType, questId);
    if (m_npType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return 0;
    }

    // The button shows "add" or "delete" depending on whether this quest already
    // has a navigation point on the current level.
    NavPointManager* npm = M3D_APP->m_pInterfaceManager->GetNavPointManager();
    NavPoint const* np =
        npm->GetNavPointByObjectId(help::GetCurrentLevelName(), questId, help::GetNpObjectTypeByQuestType(questType));
    if (np)
    {
        m_npId = np->GetId();
    }

    UpdateNpTexture();
    UpdateTooltip();
    return 1;
}

void NavPointButton::FullUpdate()
{
    // RVA 0x4F5960
    UpdateNpTexture();
    UpdateTooltip();
}

NavPoint const* NavPointButton::GetNavPoint() const
{
    // RVA 0x4F5970
    if (m_npId == -1)
    {
        return nullptr;
    }
    return M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointById(m_npId);
}

int NavPointButton::GetNavPointId() const
{
    return m_npId;
}

NavPoint::NavPointType NavPointButton::GetNavPointType() const
{
    return m_npType;
}

void NavPointButton::UpdateNpTexture()
{
    // RVA 0x4F59D0 - the small overlay marking which kind of nav point this is.
    M3D_RENDERER->ReleaseTexture(m_npTex);
    m_npTex.SetInvalid();

    // A "user location" button carries no overlay, and neither does one whose
    // quest has no nav point set yet.
    if (m_npType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES || !GetNavPoint())
    {
        return;
    }

    CStr const npTexId = m_aif.m_strNpButton + NavPointType2Str(m_npType);
    m_npTex = M3D_APP->m_pInterfaceManager->GetIcoByName(npTexId, 0);
    M3D_RENDERER->ReferenceTexture(m_npTex);
}

NavPoint::NavPointType NavPointButton::GetNavPointTypeByQuest(help::QuestType questType, int questId) const
{
    // RVA 0x4F5AB0
    if (questType != help::QUESTTYPE_STATIC)
    {
        return questType == help::QUESTTYPE_DYNAMIC ? NavPoint::NAVPOINT_TYPE_USER_QUEST
                                                    : NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES;
    }

    // A static quest is a main-story one or a side one depending on what the
    // quest-info table says about it.
    QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
    ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
    if (!quest)
    {
        return NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES;
    }
    QuestInfo const* qi = qim->GetQuestInfoByQuestInfoName(quest->GetName());
    if (!qi)
    {
        return NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES;
    }
    return qi->IsMainQuest() ? NavPoint::NAVPOINT_TYPE_MAIN_QUEST : NavPoint::NAVPOINT_TYPE_USER_QUEST;
}

CStr NavPointButton::NavPointType2Str(NavPoint::NavPointType npType)
{
    // RVA 0x4F5B20
    switch (npType)
    {
    case NavPoint::NAVPOINT_TYPE_MAIN_QUEST:
        return m_aif.m_strTypeMainQuest;
    case NavPoint::NAVPOINT_TYPE_USER_QUEST:
        return m_aif.m_strTypeUserQuest;
    case NavPoint::NAVPOINT_TYPE_USER_LOCATION:
        return m_aif.m_strTypeUserLocation;
    default:
        return {};
    }
}

void NavPointButton::UpdateTooltip()
{
    // RVA 0x4F5B90 - clicking the button toggles the nav point, so the tooltip
    // says which way it will go.
    CStr const& stringId = GetNavPoint() ? m_aif.m_strIdDeleteNavPoint : m_aif.m_strIdAddNavPoint;
    CStr const tooltip = M3D_APP->GetStringByStringId0(stringId);
    SetProperty(PROP_WND_TOOLTIP, const_cast<char*>(tooltip.c_str()));
}

void NavPointButton::OnNcPaint(m3d::ui::DrawInfo const& di, unsigned clr)
{
    // RVA 0x4F5BF0 - the nav-point overlay is centred over the button face.
    ButtonWnd::OnNcPaint(di, clr);
    if (m_npId == -1)
    {
        return;
    }

    BoundsBase<float> npB;
    npB.x0 = (m_bounds.width - m_aif.m_npTexSz.x) * 0.5f;
    npB.y0 = (m_bounds.height - m_aif.m_npTexSz.y) * 0.5f;
    npB.width = ((m_bounds.width + m_aif.m_npTexSz.x) * 0.5f) - npB.x0;
    npB.height = ((m_bounds.height + m_aif.m_npTexSz.y) * 0.5f) - npB.y0;
    GetGfxServer()->AddImagedRect(di, npB, clr, m_npTex);
}
