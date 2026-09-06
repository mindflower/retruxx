#include "questdizwnd.h"

#include "navpointbutton.h"
#include "questitemlist.h"

#include <vector>

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
#include <ui/button.h>
#include <ui/ui.h>
#include <ui/ui_srv.h>

#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimanager/uidefs.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/navpoint.h"
#include "game/uimisc/questinfo.h"
#include "server/event.h"
#include "server/objects/base/objcontainer.h"
#include "server/quest.h"

RT_CLASS_EXPORTS_BEGIN(QuestDizWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestDizWnd);

QuestDizWnd::AuxInfo::AuxInfo() : m_spaceX(8.0f), m_spaceY(10.0f), m_btnNavPointSz(32.0f, 32.0f)
{
}

QuestDizWnd::QuestDizWnd() :
    m_btnNavPoint(nullptr),
    m_wndQuestDiz(nullptr),
    m_wndDataPlace(nullptr),
    m_questType(help::QUESTTYPE_NUM_QUEST_TYPES),
    m_questId(-1)
{
}

QuestDizWnd::QuestDizWnd(QuestDizWnd const&) :
    m_btnNavPoint(nullptr),
    m_wndQuestDiz(nullptr),
    m_wndDataPlace(nullptr),
    m_questType(help::QUESTTYPE_NUM_QUEST_TYPES),
    m_questId(-1)
{
}

QuestDizWnd::~QuestDizWnd()
{
    // m_lstChildQuests (ref_ptr) releases itself; m_btnNavPoint / m_wndQuestDiz / m_wndDataPlace are
    // released through GameDataClear, matching the original.
}

m3d::Class* QuestDizWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* QuestDizWnd::GetClass() const
{
    return RT_CLASS_LOCAL(QuestDizWnd);
}

m3d::Object* QuestDizWnd::CreateObject()
{
    return new QuestDizWnd;
}

m3d::Object* QuestDizWnd::Clone()
{
    return new QuestDizWnd(*this);
}

int QuestDizWnd::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    if (!patternWnd)
    {
        return 0;
    }

    if (!m3d::ui::Wnd::Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        return 0;
    }

    // Copy the remaining state / appearance from the pattern control.
    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetTextColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    // Take the pattern's place in the window tree, then optionally destroy it.
    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        delete const_cast<m3d::ui::Wnd*>(patternWnd);
    }

    m_gameDataFlags |= 1u;
    return 1;
}

// --- setup -------------------------------------------------------------------------------------------

int QuestDizWnd::SetupForQuest(help::QuestType questType, int questId)
{
    if (questId == -1 || questType == help::QUESTTYPE_NUM_QUEST_TYPES)
    {
        return GameDataClear(false);
    }

    m_questType = questType;
    m_questId = questId;

    QuestInfoManager* qim = M3D_APP->m_pInterfaceManager->GetQuestInfoManager();
    QuestInfo const* questInfo = nullptr;
    if (questType == help::QUESTTYPE_STATIC)
    {
        if (ai::Quest* quest = ai::theQuestManager->GetQuestById(questId))
        {
            questInfo = qim->GetQuestInfoByQuestInfoName(quest->GetName());
        }
    }
    else if (questType == help::QUESTTYPE_DYNAMIC)
    {
        questInfo = qim->GetQuestInfoForDynamicQuest(questId);
    }

    int ok = SetupNavPointBtn(questType, questId);
    ok &= SetupDizWnd(questInfo);
    ok &= SetupDataPlaceWnd(questInfo);
    ok &= SetupChildQuestList(questType, questId);
    return ok;
}

int QuestDizWnd::SetupNavPointBtn(help::QuestType questType, int questId)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    if (m_btnNavPoint)
    {
        m_btnNavPoint->DecRef();
    }
    m_btnNavPoint = nullptr;

    if (questType == help::QUESTTYPE_NUM_QUEST_TYPES || questId == -1)
    {
        return 0;
    }
    if (!NeedNavPointBtnBeAdded(questType, questId))
    {
        return 1;
    }

    m_btnNavPoint = static_cast<NavPointButton*>(M3D_KERNEL->New("NavPointButton"));
    if (!m_btnNavPoint)
    {
        return 0;
    }

    BoundsBase<float> const clientB = GetClientBounds();
    BoundsBase<float> b;
    b.x0 = clientB.x0;
    b.y0 = clientB.y0;
    b.width = m_aif.m_btnNavPointSz.x;
    b.height = m_aif.m_btnNavPointSz.y;

    // 0x40200 style, control id 104 (the nav-point button reported through OnWndNotify).
    if (!m_btnNavPoint->Create({}, 0x40200u, b, 104u) || !m_btnNavPoint->SetupForQuest(questType, questId))
    {
        m_btnNavPoint->DecRef();
        m_btnNavPoint = nullptr;
        return 0;
    }

    AddChild(m_btnNavPoint);
    return 1;
}

bool QuestDizWnd::NeedNavPointBtnBeAdded(help::QuestType questType, int questId) const
{
    if (!help::CanNavPointBeAddedOnQuest(questType, questId))
    {
        return false;
    }
    if (questType != help::QUESTTYPE_STATIC)
    {
        return true;
    }
    if (questId == -1)
    {
        return true;
    }
    ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
    return !quest || quest->GetParentId() == -1;
}

int QuestDizWnd::SetupDizWnd(QuestInfo const* questInfo)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    if (m_wndQuestDiz)
    {
        m_wndQuestDiz->DecRef();
    }
    m_wndQuestDiz = nullptr;

    if (!questInfo)
    {
        return 0;
    }

    m_wndQuestDiz = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_wndQuestDiz)
    {
        return 0;
    }

    CStr const caption = questInfo->GetFullDiz();
    BoundsBase<float> const clientB = GetClientBounds();
    float const reserved = m_aif.m_btnNavPointSz.x + m_aif.m_spaceX;
    float const maxWidth = clientB.width - reserved;
    float const textHeight = GetGfxServer()->MeasureText(caption, 0, m3d::TW_WORD_WRAP, maxWidth).y;

    BoundsBase<float> b;
    b.x0 = clientB.x0 + reserved;
    b.y0 = clientB.y0;
    b.width = maxWidth;
    b.height = textHeight;

    if (!m_wndQuestDiz->Create(caption, 0x340u, b, 0))
    {
        m_wndQuestDiz->DecRef();
        m_wndQuestDiz = nullptr;
        return 0;
    }

    m_wndQuestDiz->SetDefaultFont(0);
    m_wndQuestDiz->SetTextColor(m_textColor);
    AddChild(m_wndQuestDiz);
    return 1;
}

int QuestDizWnd::SetupDataPlaceWnd(QuestInfo const* questInfo)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    if (m_wndDataPlace)
    {
        m_wndDataPlace->DecRef();
    }
    m_wndDataPlace = nullptr;

    if (!questInfo)
    {
        return 0;
    }

    m_wndDataPlace = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_wndDataPlace)
    {
        return 0;
    }

    CStr const caption = ai::theObjects->GetObjectFullName(questInfo->GetHirerName());
    BoundsBase<float> const clientB = GetClientBounds();
    PointBase<float> const textSz = GetGfxServer()->MeasureText(
        caption, 1, m3d::TW_WORD_WRAP, (clientB.width - m_aif.m_btnNavPointSz.x) - m_aif.m_spaceX);

    float y0 = clientB.y0;
    if (m_wndQuestDiz)
    {
        BoundsBase<float> const dizB = m_wndQuestDiz->GetBounds();
        y0 = m_aif.m_spaceY + dizB.height + dizB.y0;
    }

    BoundsBase<float> b;
    b.x0 = (clientB.x0 + clientB.width) - textSz.x;  // right-aligned
    b.y0 = y0;
    b.width = textSz.x;
    b.height = textSz.y;

    if (!m_wndDataPlace->Create(caption, 0x340u, b, 0))
    {
        m_wndDataPlace->DecRef();
        m_wndDataPlace = nullptr;
        return 0;
    }

    m_wndDataPlace->SetTextColor(m_textColor);
    m_wndDataPlace->SetDefaultFont(1);
    AddChild(m_wndDataPlace);
    return 1;
}

int QuestDizWnd::SetupChildQuestList(help::QuestType questType, int questId)
{
    M3D_APP->m_pInterfaceManager->RemoveWindow(IW_WND_CHILD_QUEST_LIST);
    m_lstChildQuests = nullptr;

    if (questId == -1 || questType == help::QUESTTYPE_NUM_QUEST_TYPES)
    {
        return 0;
    }
    // Only parent (static) quests get a child-quest list.
    if (questType != help::QUESTTYPE_STATIC)
    {
        return 1;
    }

    m_lstChildQuests = static_cast<QuestItemList*>(M3D_KERNEL->New("QuestItemList"));
    if (!m_lstChildQuests)
    {
        return 0;
    }

    BoundsBase<float> const clientB = GetClientBounds();
    float y0 = clientB.y0;
    if (m_wndDataPlace)
    {
        BoundsBase<float> const dpB = m_wndDataPlace->GetBounds();
        y0 = m_aif.m_spaceY + dpB.height + dpB.y0;
    }
    else if (m_wndQuestDiz)
    {
        BoundsBase<float> const dizB = m_wndQuestDiz->GetBounds();
        y0 = dizB.height + dizB.y0 + m_aif.m_spaceY;
    }

    BoundsBase<float> listB;
    listB.x0 = clientB.x0;
    listB.y0 = y0;
    listB.width = clientB.width;
    listB.height = 0.0f;

    if (!m_lstChildQuests->CreateQuestItemList(QuestItemList::LISTTYPE_CHILD_QUESTS, listB, 0x40240u, 0))
    {
        m_lstChildQuests = nullptr;
        return 0;
    }

    m_lstChildQuests->SetupForParentQuest(questId);
    AddChild(m_lstChildQuests.get());
    M3D_APP->m_pInterfaceManager->AddWindowById(m_lstChildQuests.get(), IW_WND_CHILD_QUEST_LIST, true, true);
    M3D_APP->m_pInterfaceManager->SetEventsForWindow(
        IW_WND_CHILD_QUEST_LIST,
        {IE_EV_SM_QUESTSTATE_CHANGED, IE_EV_SM_QUEST_WAS_TAKEN, IE_EV_UM_NAVPOINT_ADDED, IE_EV_UM_NAVPOINT_DELETED});
    return 1;
}

// --- nav points ------------------------------------------------------------------------------------

int QuestDizWnd::AddNavPoint(NavPoint::NavPointType npType)
{
    // Only quest nav points (main / user) can be placed from here.
    if (static_cast<unsigned int>(npType) >= NavPoint::NAVPOINT_TYPE_USER_LOCATION)
    {
        return 0;
    }

    NavPoint::ObjectType objectType;
    if (m_questType == help::QUESTTYPE_STATIC)
    {
        objectType = NavPoint::OBJECT_TYPE_STATIC_QUEST;
    }
    else if (m_questType == help::QUESTTYPE_DYNAMIC)
    {
        objectType = NavPoint::OBJECT_TYPE_DYNAMIC_QUEST;
    }
    else
    {
        objectType = NavPoint::OBJECT_TYPE_NUM_OBJECT_TYPES;
    }

    int const result = M3D_APP->m_pInterfaceManager->GetNavPointManager()->AddNavPointObjectDependend(
        help::GetCurrentLevelName(), npType, objectType, m_questId, true);
    return result != -1;
}

int QuestDizWnd::RemoveNavPoint(int npId)
{
    return M3D_APP->m_pInterfaceManager->GetNavPointManager()->RemoveNavPointById(npId);
}

void QuestDizWnd::OnBtnNavPointClick(m3d::ui::Wnd* wndFrom, unsigned int, m3d::AIParam const&)
{
    if (!wndFrom || !wndFrom->IsKindOf(RT_CLASS_LOCAL(NavPointButton)))
    {
        return;
    }
    auto* btn = static_cast<NavPointButton*>(wndFrom);
    if (btn->GetNavPointId() == -1)
    {
        AddNavPoint(btn->GetNavPointType());
    }
    else
    {
        M3D_APP->m_pInterfaceManager->GetNavPointManager()->RemoveNavPointById(btn->GetNavPointId());
    }
}

// --- events -----------------------------------------------------------------------------------------

int QuestDizWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (idFrom != 104 || message != 1)
    {
        return 0;
    }
    OnBtnNavPointClick(from, message, data);
    return 1;
}

int QuestDizWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((dataType == IE_EV_UM_NAVPOINT_ADDED || dataType == IE_EV_UM_NAVPOINT_DELETED) && data && IsChildOf(M3D_APP))
    {
        OnNavPoint(data);
    }
    return 1;
}

void QuestDizWnd::OnNavPointAdded(void* data)
{
    if (data && IsChildOf(M3D_APP))
    {
        OnNavPoint(data);
    }
}

void QuestDizWnd::OnNavPointDeleted(void* data)
{
    if (data && IsChildOf(M3D_APP))
    {
        OnNavPoint(data);
    }
}

void QuestDizWnd::OnNavPoint(void* data)
{
    if (!data)
    {
        return;
    }

    // data is the m3d::Event broadcast for UM_NAVPOINT_ADDED/DELETED: m_intEv[0] = quest id,
    // m_intEv[1] = NavPoint::ObjectType (see ExMachina 1.02 NoCD, QuestDizWnd::OnNavPoint @ 0x4F8600).
    auto const* ev = static_cast<m3d::Event*>(data);
    int const questId = ev->m_intEv[0];
    int const objectType = ev->m_intEv[1];

    help::QuestType questType;
    if (objectType == NavPoint::OBJECT_TYPE_STATIC_QUEST)
    {
        questType = help::QUESTTYPE_STATIC;
    }
    else if (objectType == NavPoint::OBJECT_TYPE_DYNAMIC_QUEST)
    {
        questType = help::QUESTTYPE_DYNAMIC;
    }
    else
    {
        return;
    }

    // Only rebuild the button when the event concerns the quest this window is showing.
    if (questId != -1 && m_questType == questType && m_questId == questId)
    {
        SetupNavPointBtn(questType, questId);
    }
}

int QuestDizWnd::GameDataClear(bool)
{
    if (m_wndQuestDiz)
    {
        m_wndQuestDiz->DecRef();
    }
    m_wndQuestDiz = nullptr;

    if (m_wndDataPlace)
    {
        m_wndDataPlace->DecRef();
    }
    m_wndDataPlace = nullptr;

    if (m_btnNavPoint)
    {
        m_btnNavPoint->DecRef();
    }
    m_btnNavPoint = nullptr;

    M3D_APP->m_pInterfaceManager->RemoveWindow(IW_WND_CHILD_QUEST_LIST);
    m_lstChildQuests = nullptr;

    m_questType = help::QUESTTYPE_NUM_QUEST_TYPES;
    m_questId = -1;
    return 1;
}
