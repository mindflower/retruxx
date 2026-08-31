#include "taksebequestlogwnd.h"
#include "questitemswnd.h"
#include "questdizwnd.h"
#include "questitemlist.h"
#include "questitem.h"
#include "mapcombobox.h"

#include <algorithm>
#include <vector>

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <ui/button.h>
#include <ui/comboboxwnd.h>
#include <ui/listbox.h>

#include "game/m3dgame.h"
#include "game/profile.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimanager/uidefs.h"
#include "game/uimisc/levelinfo.h"
#include "game/uimisc/questinfo.h"
#include "server/quest.h"

namespace
{
    // Pattern-control ids of the child widgets that notify this window.
    enum
    {
        CTRL_ID_CB_MAPS = 0x12C,
        CTRL_ID_BTN_FILTER = 0x12D,
        CTRL_ID_LST_QUESTS = 0x12F,
        CTRL_ID_BTN_SHOW_QUEST_ITEMS = 0x130,
    };

    // Notify message codes coming through OnWndNotify.
    enum
    {
        WND_NOTIFY_CLICK = 1,
        WND_NOTIFY_SEL_CHANGED = 5,
    };

    // Complete-filter part bit flags (low word of m_filter).
    enum
    {
        COMPLETE_FILTER_ACTIVE = 1,     // show not-yet-completed quests
        COMPLETE_FILTER_COMPLETED = 2,  // show completed quests
        COMPLETE_FILTER_FAILED = 4,     // show failed quests
        COMPLETE_FILTER_ALL = 7,
    };

    // GameDataUpdate data types this window reacts to.
    enum
    {
        DATATYPE_CUR_PROFILE_CHANGED = 40,
        DATATYPE_CUR_PROFILE_PARAM_CHANGED = 42,
    };
}  // namespace

RT_CLASS_EXPORTS_BEGIN(TakSebeQuestLogWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TakSebeQuestLogWnd);

TakSebeQuestLogWnd::TsqlAuxInfo::TsqlAuxInfo()
{
    m_cbMapsName = "cbMaps";
    m_lstQuestsName = "lstQuests";
    m_wndQuestDizName = "wndQuestDiz";
    m_btnFilterName = "btnFilter";
    m_btnShowQuestItemsName = "btnShowQuestItemsList";
    m_icoIdFilterShowAll_out = "QuestFilterShowAll_out";
    m_icoIdFilterShowAll_in = "QuestFilterShowAll_int";
    m_icoIdFilterShowAll_down = "QuestFilterShowAll_down";
    m_icoIdFilterShowNoncompleted_out = "QuestFilterShowNoncompleted_out";
    m_icoIdFilterShowNoncompleted_in = "QuestFilterShowNoncompleted_in";
    m_icoIdFilterShowNoncompleted_down = "QuestFilterShowNoncompleted_down";
    m_strIdFilterShowAll = "ShowAllQuests";
    m_strIdFilterShowNoncompleted = "ShowNoncompletedQuests";
    m_strIdShowQuestItems = "ShowQuestItems";
    m_strIdShowQuests = "ShowQuests";
    m_strIdQuestItems = "QuestItems";
    m_strIdQuests = "Quests";
    m_wndLstQuestsFrameName = "lstQuestsFrame";
    m_wndEmbossCbMapsName = "emboss_cbMaps";
    m_wndEmbossBtnFilterName = "emboss_btnFilter";
}

TakSebeQuestLogWnd::~TakSebeQuestLogWnd()
{
    m_wndQuestItems = nullptr;
    m_lstQuests = nullptr;
    m_wndQuestDiz = nullptr;
    m_cbMaps = nullptr;
    delete m_btnFilter;
}

m3d::Class* TakSebeQuestLogWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Object* TakSebeQuestLogWnd::CreateObject()
{
    return new TakSebeQuestLogWnd;
}

m3d::Object* TakSebeQuestLogWnd::Clone()
{
    return new TakSebeQuestLogWnd(*this);
}

m3d::Class* TakSebeQuestLogWnd::GetClass() const
{
    return RT_CLASS_LOCAL(TakSebeQuestLogWnd);
}

TakSebeQuestLogWnd::TakSebeQuestLogWnd(TakSebeQuestLogWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

TakSebeQuestLogWnd::TakSebeQuestLogWnd()
{
    m_cbMaps = nullptr;
    m_lstQuests = nullptr;
    m_wndQuestDiz = nullptr;
    m_wndQuestItems = nullptr;
    m_filter = 0xFFFF0007;  // map part = -1 (no map filter), complete part = 7 (show all)
    m_btnFilter = nullptr;
    m_btnShowQuestItems = nullptr;
    m_wndLstQuestsFrame = nullptr;
    m_wndEmbossCbMaps = nullptr;
    m_wndEmbossBtnFilter = nullptr;
}

// --- filter word helpers ---------------------------------------------------------------------------
// m_filter packs two 16-bit values: low word = "complete filter part" (COMPLETE_FILTER_* bits),
// high word = "map filter part" (a level-info id, or -1 = no filter, -2 = any map).

short TakSebeQuestLogWnd::GetCompleteFilterPart(long filter) const
{
    return static_cast<short>(filter & 0xFFFF);
}

short TakSebeQuestLogWnd::GetMapFilterPart(long filter) const
{
    return static_cast<short>(static_cast<unsigned long>(filter) >> 16);
}

int TakSebeQuestLogWnd::MakeFilterFromParts(short mapPart, short completePart)
{
    return SetFilter(
        (static_cast<int>(static_cast<unsigned short>(mapPart)) << 16) | static_cast<unsigned short>(completePart));
}

int TakSebeQuestLogWnd::SetCompleteFilterPart(short completePart)
{
    return SetFilter((static_cast<int>(GetMapFilterPart(m_filter)) << 16) | static_cast<unsigned short>(completePart));
}

int TakSebeQuestLogWnd::SetMapFilterPart(short mapPart)
{
    return SetFilter(
        (static_cast<int>(static_cast<unsigned short>(mapPart)) << 16) |
        static_cast<unsigned short>(GetCompleteFilterPart(m_filter)));
}

int TakSebeQuestLogWnd::ShowAll()
{
    return SetFilter((static_cast<int>(GetMapFilterPart(m_filter)) << 16) | COMPLETE_FILTER_ALL);
}

int TakSebeQuestLogWnd::ShowNoncompleted()
{
    return SetFilter((static_cast<int>(GetMapFilterPart(m_filter)) << 16) | COMPLETE_FILTER_ACTIVE);
}

void TakSebeQuestLogWnd::ToggleFilter()
{
    int completePart =
        (GetCompleteFilterPart(m_filter) == COMPLETE_FILTER_ACTIVE) ? COMPLETE_FILTER_ALL : COMPLETE_FILTER_ACTIVE;
    SetFilter((static_cast<int>(GetMapFilterPart(m_filter)) << 16) | completePart);
}

int TakSebeQuestLogWnd::SetFilter(long newFilter)
{
    if (GetCompleteFilterPart(newFilter) != GetCompleteFilterPart(m_filter))
    {
        Profile* profile = M3D_APP->GetProfileManager()->GetCurProfile();
        if (!profile)
        {
            return 1;
        }
        m3d::AIParam param{static_cast<int>(GetCompleteFilterPart(newFilter))};
        profile->SetParam(PP_QUESTS_FILTER, param);
    }

    // Only the map part is written into the member here; the complete part round-trips
    // through the profile (see OnCompleteFilterPartChanged -> SetFilter0).
    if (GetMapFilterPart(newFilter) != GetMapFilterPart(m_filter))
    {
        int cand = (static_cast<int>(GetMapFilterPart(newFilter)) << 16) |
            static_cast<unsigned short>(GetCompleteFilterPart(m_filter));
        if (cand != m_filter)
        {
            m_filter = cand;
            if (IsChildOf(M3D_APP))
            {
                OnChangeFilter();
            }
        }
    }
    return 1;
}

int TakSebeQuestLogWnd::SetFilter0(long filter)
{
    if (filter == m_filter)
    {
        return 1;
    }
    m_filter = filter;
    if (IsChildOf(M3D_APP))
    {
        return OnChangeFilter();
    }
    return 1;
}

int TakSebeQuestLogWnd::OnChangeFilter()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    UpdateFilterButtonState();
    M3D_ASSERT(m_lstQuests);
    return m_lstQuests->CreateItems();
}

// --- profile-driven updates ----------------------------------------------------------------------

void TakSebeQuestLogWnd::OnCompleteFilterPartChanged()
{
    Profile* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return;
    }
    m3d::AIParam param;
    if (profile->GetParam(PP_QUESTS_FILTER, param))
    {
        unsigned int completePart = param.GetAsID();
        if (completePart & COMPLETE_FILTER_ALL)
        {
            SetFilter0(
                (static_cast<int>(GetMapFilterPart(m_filter)) << 16) | static_cast<unsigned short>(completePart));
        }
    }
}

void TakSebeQuestLogWnd::OnCurProfileChanged()
{
    OnCompleteFilterPartChanged();
}

void TakSebeQuestLogWnd::OnCurProfileParamChanged(void* ev)
{
    if (ev && *reinterpret_cast<int*>(static_cast<char*>(ev) + 0x34) == PP_QUESTS_FILTER)
    {
        OnCompleteFilterPartChanged();
    }
}

int TakSebeQuestLogWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == DATATYPE_CUR_PROFILE_CHANGED ||
        (dataType == DATATYPE_CUR_PROFILE_PARAM_CHANGED && data &&
         *reinterpret_cast<int*>(static_cast<char*>(data) + 0x34) == PP_QUESTS_FILTER))
    {
        OnCompleteFilterPartChanged();
    }
    return 1;
}

// --- notifications ------------------------------------------------------------------------------------

int TakSebeQuestLogWnd::OnWndNotify(
    m3d::ui::Wnd* from,
    unsigned int idFrom,
    unsigned int message,
    m3d::AIParam const& data)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, idFrom, message, data))
    {
        return 1;
    }

    switch (idFrom)
    {
    case CTRL_ID_CB_MAPS:
        if (message == WND_NOTIFY_SEL_CHANGED)
        {
            ChangeMap();
            return 1;
        }
        break;
    case CTRL_ID_BTN_FILTER:
        if (message == WND_NOTIFY_CLICK)
        {
            ToggleFilter();
            return 1;
        }
        break;
    case CTRL_ID_LST_QUESTS:
        if (message == WND_NOTIFY_SEL_CHANGED)
        {
            OnChangeSelectedQuest();
            return 1;
        }
        break;
    case CTRL_ID_BTN_SHOW_QUEST_ITEMS:
        if (message == WND_NOTIFY_CLICK)
        {
            ToggleQuestItemsWnd();
            return 1;
        }
        break;
    }
    return 0;
}

int TakSebeQuestLogWnd::OnChangeSelectedQuest()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    M3D_ASSERT(m_lstQuests);

    help::QuestType questType = help::QUESTTYPE_INVALID;
    int questId = -1;
    int sel = m_lstQuests->GetCurSel();
    if (sel != -1)
    {
        M3D_ASSERT(m_lstQuests);
        if (QuestItem* item = m_lstQuests->GetItem(sel))
        {
            questId = item->GetQuestId();
            questType = item->GetQuestType();
        }
    }

    M3D_ASSERT(m_wndQuestDiz);
    return m_wndQuestDiz->SetupForQuest(questType, questId);
}

int TakSebeQuestLogWnd::ChangeMap()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    M3D_ASSERT(m_cbMaps);
    int sel = m_cbMaps->GetCurSel();
    if (sel == -1)
    {
        return 1;
    }
    M3D_ASSERT(m_cbMaps);
    int mapId = m_cbMaps->GetItemData(sel);
    return SetFilter(
        (static_cast<int>(static_cast<unsigned short>(mapId)) << 16) |
        static_cast<unsigned short>(GetCompleteFilterPart(m_filter)));
}

// --- quest visibility predicates ---------------------------------------------------------------------

bool TakSebeQuestLogWnd::IsCompleteStatusFit(help::QuestType questType, int questId, short completePart) const
{
    switch (help::GetQuestUnifyStatusByQuestId(questType, questId))
    {
    case help::QUESTSTATUS_NONCOMPLETE:
        return (completePart & COMPLETE_FILTER_ACTIVE) != 0;
    case help::QUESTSTATUS_COMPLETE:
        return questType == help::QUESTTYPE_DYNAMIC ? false : ((completePart & COMPLETE_FILTER_COMPLETED) >> 1) != 0;
    case help::QUESTSTATUS_FAILED:
        return questType == help::QUESTTYPE_DYNAMIC ? false : ((completePart & COMPLETE_FILTER_FAILED) >> 2) != 0;
    default:
        return false;
    }
}

bool TakSebeQuestLogWnd::IsMapFit(help::QuestType questType, int questId, short mapPart) const
{
    if (mapPart == -2)
    {
        return true;
    }

    std::vector<CStr> mapNames;
    if (mapPart == 0)
    {
        ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
        QuestInfo const* questInfo = quest ?
            M3D_APP->m_pInterfaceManager->GetQuestInfoManager()->GetQuestInfoByQuestInfoName(quest->GetName()) :
            nullptr;
        if (!questInfo)
        {
            return false;
        }
        mapNames = questInfo->GetMapNames();
    }
    else if (mapPart == 1)
    {
        mapNames.push_back(help::GetCurrentLevelName());
    }
    // mapPart >= 2: mapNames stays empty

    std::vector<int> levelIds;
    for (auto const& name : mapNames)
    {
        int id = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelInfoId(name);
        if (std::find(levelIds.begin(), levelIds.end(), id) == levelIds.end())
        {
            levelIds.push_back(id);
        }
    }

    if (mapPart != -1 && !levelIds.empty())
    {
        return std::find(levelIds.begin(), levelIds.end(), static_cast<int>(mapPart)) != levelIds.end();
    }
    return false;
}

bool TakSebeQuestLogWnd::CanQuestBeShown(help::QuestType questType, int questId) const
{
    if (!IsMapFit(questType, questId, GetMapFilterPart(m_filter)))
    {
        return false;
    }

    short completePart = GetCompleteFilterPart(m_filter);
    switch (help::GetQuestUnifyStatusByQuestId(questType, questId))
    {
    case help::QUESTSTATUS_NONCOMPLETE:
        break;
    case help::QUESTSTATUS_COMPLETE:
        if (questType == help::QUESTTYPE_DYNAMIC)
        {
            return false;
        }
        completePart >>= 1;
        break;
    case help::QUESTSTATUS_FAILED:
        if (questType == help::QUESTTYPE_DYNAMIC)
        {
            return false;
        }
        completePart >>= 2;
        break;
    default:
        return false;
    }
    if ((completePart & 1) == 0)
    {
        return false;
    }

    if (questType != help::QUESTTYPE_STATIC)
    {
        return true;
    }

    // Static quests: require a matching registered QuestInfo.
    ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
    if (!quest)
    {
        return false;
    }
    return M3D_APP->m_pInterfaceManager->GetQuestInfoManager()->GetQuestInfoByQuestInfoName(quest->GetName()) !=
        nullptr;
}

// --- button visuals / tooltips ---------------------------------------------------------------------

void TakSebeQuestLogWnd::UpdateFilterButtonState()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    bool showAll = (GetCompleteFilterPart(m_filter) == COMPLETE_FILTER_ALL);

    CStr const& icoOut = showAll ? m_tsqlAif.m_icoIdFilterShowAll_out : m_tsqlAif.m_icoIdFilterShowNoncompleted_out;
    CStr const& icoIn = showAll ? m_tsqlAif.m_icoIdFilterShowAll_in : m_tsqlAif.m_icoIdFilterShowNoncompleted_in;
    CStr const& icoDown = showAll ? m_tsqlAif.m_icoIdFilterShowAll_down : m_tsqlAif.m_icoIdFilterShowNoncompleted_down;

    auto* mgr = M3D_APP->m_pInterfaceManager;
    m_btnFilter->SetImaged(
        mgr->GetIcoByName(icoOut, 0), mgr->GetIcoByName(icoIn, 0), mgr->GetIcoByName(icoDown, 0), {});
    UpdateFilterButtonTooltip();
}

void TakSebeQuestLogWnd::UpdateFilterButtonTooltip()
{
    CStr const& strId = (GetCompleteFilterPart(m_filter) == COMPLETE_FILTER_ACTIVE) ?
        m_tsqlAif.m_strIdFilterShowAll :
        m_tsqlAif.m_strIdFilterShowNoncompleted;
    CStr tooltip = M3D_APP->GetStringByStringId0(strId);
    m_btnFilter->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void TakSebeQuestLogWnd::UpdateQuestItemsButtonState()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    M3D_ASSERT(m_wndQuestItems);
    bool inItemMode = IsDirectChild(m_wndQuestItems.get());
    CStr const& strId = inItemMode ? m_tsqlAif.m_strIdQuests : m_tsqlAif.m_strIdQuestItems;
    m_btnShowQuestItems->SetText(M3D_APP->GetStringByStringId0(strId));
    UpdateQuestItemsButtonTooltip();
}

void TakSebeQuestLogWnd::UpdateQuestItemsButtonTooltip()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    M3D_ASSERT(m_wndQuestItems);
    bool inItemMode = IsDirectChild(m_wndQuestItems.get());
    CStr const& strId = inItemMode ? m_tsqlAif.m_strIdShowQuests : m_tsqlAif.m_strIdShowQuestItems;
    CStr tooltip = M3D_APP->GetStringByStringId0(strId);
    m_btnShowQuestItems->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

// --- quest-items mode --------------------------------------------------------------------------------

bool TakSebeQuestLogWnd::IsInQuestItemMode()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return false;
    }
    M3D_ASSERT(m_wndQuestItems);
    return IsDirectChild(m_wndQuestItems.get());
}

void TakSebeQuestLogWnd::ToggleQuestItemsWnd()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    M3D_ASSERT(m_wndQuestItems);
    ShowQuestItemsWnd(!IsDirectChild(m_wndQuestItems.get()));
}

void TakSebeQuestLogWnd::ShowQuestItemsWnd(bool bShow)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    M3D_ASSERT(m_cbMaps);
    M3D_ASSERT(m_lstQuests);
    M3D_ASSERT(m_wndQuestDiz);

    m3d::ui::Wnd* normalCtrls[7] = {
        m_cbMaps.get(),
        m_lstQuests.get(),
        m_wndQuestDiz.get(),
        m_btnFilter,
        m_wndLstQuestsFrame,
        m_wndEmbossCbMaps,
        m_wndEmbossBtnFilter,
    };

    if (!bShow)
    {
        // Leave quest-items mode: restore all normal controls, detach the items window.
        for (int i = 0; i < 7; ++i)
        {
            if (normalCtrls[i] && !normalCtrls[i]->GetParent())
            {
                AddChild(normalCtrls[i]);
            }
        }
        if (m_wndQuestItems && IsDirectChild(m_wndQuestItems.get()))
        {
            RemoveChild(m_wndQuestItems.get());
        }
    }
    else
    {
        // Enter quest-items mode: detach every normal control except the map combo box.
        for (int i = 1; i < 7; ++i)
        {
            if (normalCtrls[i] && IsDirectChild(normalCtrls[i]))
            {
                RemoveChild(normalCtrls[i]);
            }
        }
        if (m_wndQuestItems && !m_wndQuestItems->GetParent())
        {
            AddChild(m_wndQuestItems.get());
        }
    }
    UpdateQuestItemsButtonState();
}

int TakSebeQuestLogWnd::OnBeforeAddToWndStation()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        UpdateFilterButtonState();
        UpdateQuestItemsButtonState();
    }
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

// --- setup ------------------------------------------------------------------------------------------

int TakSebeQuestLogWnd::GameDataSetup()
{
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_btnFilterName), m3d::ui::ButtonWnd))
        {
            m_btnFilter = child;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_tsqlAif.m_btnFilterName + " is not found or incorrect type");
            res = 0;
        }

        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_btnShowQuestItemsName), m3d::ui::ButtonWnd))
        {
            m_btnShowQuestItems = child;
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_tsqlAif.m_btnShowQuestItemsName + " is not found or incorrect type");
            res = 0;
        }

        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_wndLstQuestsFrameName), m3d::ui::Wnd))
        {
            m_wndLstQuestsFrame = child;
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_tsqlAif.m_wndLstQuestsFrameName + " is not found or incorrect type");
            res = 0;
        }

        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_wndEmbossBtnFilterName), m3d::ui::Wnd))
        {
            m_wndEmbossBtnFilter = child;
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_tsqlAif.m_wndEmbossBtnFilterName + " is not found or incorrect type");
            res = 0;
        }

        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_wndEmbossCbMapsName), m3d::ui::Wnd))
        {
            m_wndEmbossCbMaps = child;
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_tsqlAif.m_wndEmbossCbMapsName + " is not found or incorrect type");
            res = 0;
        }

        // m_cbMaps: wrap the ComboBoxWnd child with a MapComboBox created via RTTI.
        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_cbMapsName), m3d::ui::ComboBoxWnd))
        {
            m_cbMaps = static_cast<MapComboBox*>(M3D_KERNEL->New("MapComboBox"));
            if (m_cbMaps)
            {
                if (!m_cbMaps->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_tsqlAif.m_cbMapsName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_tsqlAif.m_cbMapsName +
                    " - cannot find rtti class MapComboBox");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_tsqlAif.m_cbMapsName + " is not found or incorrect type");
            res = 0;
        }

        // m_lstQuests: wrap the StringsListBoxWnd child with a QuestItemList created via RTTI.
        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_lstQuestsName), m3d::ui::StringsListBoxWnd))
        {
            m_lstQuests = static_cast<QuestItemList*>(M3D_KERNEL->New("QuestItemList"));
            if (m_lstQuests)
            {
                if (!m_lstQuests->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_tsqlAif.m_lstQuestsName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_tsqlAif.m_lstQuestsName +
                    " - cannot find rtti class QuestItemList");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: control " + m_tsqlAif.m_lstQuestsName + " is not found or incorrect type");
            res = 0;
        }

        // m_wndQuestDiz: wrap the Wnd child with a QuestDizWnd created via RTTI.
        if (auto* child = RT_DYNCAST(GetChildByName(m_tsqlAif.m_wndQuestDizName), m3d::ui::Wnd))
        {
            m_wndQuestDiz = static_cast<QuestDizWnd*>(M3D_KERNEL->New("QuestDizWnd"));
            if (m_wndQuestDiz)
            {
                if (!m_wndQuestDiz->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_tsqlAif.m_wndQuestDizName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_tsqlAif.m_wndQuestDizName +
                    " - cannot find rtti class QuestDizWnd");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: control " + m_tsqlAif.m_wndQuestDizName + " is not found or incorrect type");
            res = 0;
        }

        // m_wndQuestItems: taken from the interface manager, not from a pattern child.
        ref_ptr<m3d::ui::Wnd> questItemsWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_QUEST_ITEMS);
        if (questItemsWnd && RT_DYNCAST(questItemsWnd.get(), QuestItemsWnd))
        {
            m_wndQuestItems = static_cast<QuestItemsWnd*>(questItemsWnd.get());
            if (res)
            {
                M3D_ASSERT(m_lstQuests);
                M3D_APP->m_pInterfaceManager->AddWindowById(m_lstQuests.get(), IW_WND_QUEST_LIST, true, true);
                M3D_ASSERT(m_cbMaps);
                M3D_APP->m_pInterfaceManager->AddWindowById(m_cbMaps.get(), IW_WND_MAP_LIST_IN_QUESTS, true, true);
                M3D_ASSERT(m_wndQuestDiz);
                M3D_APP->m_pInterfaceManager->AddWindowById(m_wndQuestDiz.get(), IW_WND_QUEST_DIZ, true, true);

                M3D_ASSERT(m_lstQuests);
                MoveChildToFirstPosition(m_lstQuests.get());
                M3D_ASSERT(m_cbMaps);
                MoveChildToFirstPosition(m_cbMaps.get());

                m_gameDataFlags |= 1u;
                OnCompleteFilterPartChanged();
                ShowQuestItemsWnd(IsInQuestItemMode());
            }
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_ERR("TakSebeQuestLogWnd: error - fail to init because of a bad resource");
    return 0;
}
