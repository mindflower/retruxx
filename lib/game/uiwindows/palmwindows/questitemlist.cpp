#include "questitemlist.h"

#include "questitem.h"
#include "taksebequestlogwnd.h"

#include <algorithm>
#include <set>
#include <vector>

#include <core/aiparam.h>
#include <core/clazz.h>
#include <core/kernel.h>
#include <core/log.h>
#include <ui/listbox.h>
#include <ui/ui.h>

#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimanager/uidefs.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/navpoint.h"
#include "game/uimisc/objectcollection.h"
#include "server/event.h"
#include "server/objects/dynamicquest.h"
#include "server/quest.h"

namespace
{
    // ExMachina 1.02 NoCD: `anonymous namespace'::LessByMainStatus - std::stable_partition predicate
    // that keeps still-active (non-completed) quests ahead of the finished ones.
    bool LessByMainStatus(int questId)
    {
        return help::GetQuestUnifyStatusByQuestId(help::QUESTTYPE_STATIC, questId) == help::QUESTSTATUS_NONCOMPLETE;
    }

    // ExMachina 1.02 NoCD: help::DynamicQuestLessByTakeGameTime - orders dynamic quests by the game
    // time at which the player took them (oldest first).
    // TODO: needs the per-quest "taken" timestamp; falls back to quest-id order for now.
    bool DynamicQuestLessByTakeGameTime(int lhsQuestId, int rhsQuestId)
    {
        return lhsQuestId < rhsQuestId;
    }
}  // namespace

RT_CLASS_EXPORTS_BEGIN(QuestItemList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItemList);

QuestItemList::QuestItemList() : m_listType(LISTTYPE_PARENT_QUESTS), m_parentQuestId(-1)
{
}

QuestItemList::QuestItemList(QuestItemList const&) : m_listType(LISTTYPE_PARENT_QUESTS), m_parentQuestId(-1)
{
}

QuestItemList::~QuestItemList()
{
    // Items are released through GameDataClear/RemoveAllItems/DeleteItem; the base
    // ~ListBoxWnd / ~Wnd take care of the rest.
}

m3d::Class* QuestItemList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* QuestItemList::GetClass() const
{
    return RT_CLASS_LOCAL(QuestItemList);
}

m3d::Object* QuestItemList::CreateObject()
{
    return new QuestItemList;
}

m3d::Object* QuestItemList::Clone()
{
    return new QuestItemList(*this);
}

// --- construction ---------------------------------------------------------------------------------

int QuestItemList::CreateQuestItemList(
    ListType listType,
    BoundsBase<float> const& rc,
    unsigned int flags,
    unsigned int id)
{
    if (!m3d::ui::ListBoxWnd<QuestItem*>::Create(rc, flags, id))
    {
        return 0;
    }

    m_drawFlags |= 1u;
    m_listType = listType;

    // Child-quest lists are laid out inside a parent QuestItem and never scroll on their own.
    if (listType == LISTTYPE_CHILD_QUESTS && m_scrollVWnd)
    {
        m_scrollVWnd->DecRef();
        m_scrollVWnd = nullptr;
    }
    return 1;
}

int QuestItemList::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    auto* pattern = RT_DYNCAST(patternWnd, m3d::ui::StringsListBoxWnd const);
    if (!pattern)
    {
        M3D_LOG_INFO("QuestItemList::CreateFromPattern error - invalid pattenr wnd");
        return 0;
    }

    if (!CreateQuestItemList(LISTTYPE_PARENT_QUESTS, pattern->GetBounds(), pattern->GetStyle(), pattern->GetId()))
    {
        M3D_LOG_INFO("QuestItemList::CreateFromPattern error - cannot create window");
        return 0;
    }

    // Copy the remaining state / appearance from the pattern control.
    SetStyle(pattern->GetStyle());
    SetText(pattern->GetText());
    SetId(pattern->GetId());
    SetName(pattern->GetName());
    SetBounds(pattern->GetBounds(), true);
    SetDefaultFont(pattern->GetDefaultFont());
    SetFormatMode(pattern->GetFormatMode());
    SetColor(pattern->GetColor());
    SetTextColor(pattern->GetTextColor());
    SetTextColorDisabled(pattern->GetTextColorDisabled());
    SetClientEdges(pattern->GetClientEdges());
    SetPane(pattern->GetPaneName());
    SetPaneFlags(pattern->GetPaneFlags());
    SetScrollPane(pattern->GetScrollPaneName());
    SetBackground(pattern->GetBackground());

    CStr tooltip;
    pattern->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(pattern->GetOnShowAnimation());
    SetOnHideAnimation(pattern->GetOnHideAnimation());
    SetDrawFlags(const_cast<m3d::ui::StringsListBoxWnd*>(pattern)->GetDrawFlags());

    // Take the pattern's place in the window tree, then optionally destroy it.
    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        M3D_LOG_INFO("QuestItemList::CreateFromPattern error - invalid parent for parttern wnd");
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        delete const_cast<m3d::ui::StringsListBoxWnd*>(pattern);
    }
    return 1;
}

int QuestItemList::SetupForParentQuest(int parentQuestId)
{
    if (m_listType != LISTTYPE_CHILD_QUESTS)
    {
        return 0;
    }
    m_parentQuestId = parentQuestId;
    if (IsChildOf(M3D_APP))
    {
        CreateItems();
    }
    return 1;
}

// --- population -----------------------------------------------------------------------------------

int QuestItemList::FullUpdate()
{
    return CreateItems();
}

int QuestItemList::CreateItems()
{
    if (!Valid())
    {
        return 0;
    }

    // Remember which quest is currently selected so it can be reselected after the rebuild.
    help::QuestType selType = help::QUESTTYPE_NUM_QUEST_TYPES;
    int selId = -1;
    int const cur = GetCurSel();
    if (cur != -1)
    {
        if (QuestItem* sel = m_items[cur].m_item)
        {
            selType = sel->GetQuestType();
            selId = sel->GetQuestId();
        }
    }

    GameDataClear(false);

    // A child-quest list is meaningless until its parent quest has been set.
    if (m_listType == LISTTYPE_CHILD_QUESTS && m_parentQuestId == -1)
    {
        return 0;
    }

    int ok = CreateStaticQuestItems();
    ok &= CreateDynamicQuestItems();

    int const idx = GetQuestItemIdxByQuestId(selType, selId);
    if (idx != -1)
    {
        SetCurSel(idx);
    }
    else if (GetCount() > 0)
    {
        SetCurSel(GetCount() - 1);
    }
    else
    {
        SetCurSel(-1);
    }
    return ok;
}

int QuestItemList::CreateStaticQuestItems()
{
    std::vector<int> questIds;
    GetStaticQuestIdsToShow(questIds);
    SortStaticQuests(questIds);

    int ok = 1;
    for (int questId : questIds)
    {
        if (!Valid() || !CanQuestBeShown(help::QUESTTYPE_STATIC, questId))
        {
            ok = 0;
            continue;
        }
        QuestItem* item = CreateQuestItem(help::QUESTTYPE_STATIC, questId);
        if (!item)
        {
            ok = 0;
            continue;
        }
        AddItem(item);
        AddChild(item);
        if (m_listType == LISTTYPE_CHILD_QUESTS)
        {
            UpdateBounds();
        }
    }
    return ok;
}

int QuestItemList::CreateDynamicQuestItems()
{
    std::vector<int> questIds;
    GetDynamicQuestIdsToShow(questIds);
    SortDynamicQuests(questIds);

    // The original decompiled body only shows the "collect + sort" prologue; the item-creation
    // loop is identical to CreateStaticQuestItems (both are inlined side effects of CreateItems).
    int ok = 1;
    for (int questId : questIds)
    {
        if (!Valid() || !CanQuestBeShown(help::QUESTTYPE_DYNAMIC, questId))
        {
            ok = 0;
            continue;
        }
        QuestItem* item = CreateQuestItem(help::QUESTTYPE_DYNAMIC, questId);
        if (!item)
        {
            ok = 0;
            continue;
        }
        AddItem(item);
        AddChild(item);
        if (m_listType == LISTTYPE_CHILD_QUESTS)
        {
            UpdateBounds();
        }
    }
    return ok;
}

int QuestItemList::AddQuestItem(help::QuestType questType, int questId, bool select)
{
    if (!Valid())
    {
        return 0;
    }
    if (!CanQuestBeShown(questType, questId))
    {
        return 0;
    }
    QuestItem* item = CreateQuestItem(questType, questId);
    if (!item)
    {
        return 0;
    }
    int const idx = AddItem(item);
    AddChild(item);
    if (m_listType == LISTTYPE_CHILD_QUESTS)
    {
        UpdateBounds();
    }
    if (select)
    {
        SetCurSel(idx);
    }
    return 1;
}

QuestItem* QuestItemList::CreateQuestItem(help::QuestType questType, int questId)
{
    if (!Valid())
    {
        return nullptr;
    }
    if (questId == -1 || questType == help::QUESTTYPE_NUM_QUEST_TYPES)
    {
        return nullptr;
    }
    QuestItem* item = static_cast<QuestItem*>(M3D_KERNEL->New("QuestItem"));
    if (!item)
    {
        return nullptr;
    }
    BoundsBase<float> const clientB = GetClientBounds();
    if (!item->CreateQuestItem({clientB.x0, clientB.y0}, clientB.width, questType, questId, 0))
    {
        item->DecRef();
        return nullptr;
    }
    return item;
}

// --- which quests to show -----------------------------------------------------------------------

void QuestItemList::GetStaticQuestIdsToShow(std::vector<int, std::allocator<int>>& questIds) const
{
    questIds.clear();
    for (int questId : M3D_APP->m_pInterfaceManager->GetTakenQuestIds())
    {
        if (CanQuestBeShown(help::QUESTTYPE_STATIC, questId))
        {
            questIds.push_back(questId);
        }
    }
}

void QuestItemList::GetDynamicQuestIdsToShow(std::vector<int, std::allocator<int>>& questIds) const
{
    questIds.clear();
    ObjectCollection const& objects = M3D_APP->m_pInterfaceManager->GetObjectCollection();
    if (std::set<int> const* ids = objects.GetObjectsByClass(&ai::DynamicQuest::m_classDynamicQuest))
    {
        for (int questId : *ids)
        {
            if (CanQuestBeShown(help::QUESTTYPE_DYNAMIC, questId))
            {
                questIds.push_back(questId);
            }
        }
    }
}

bool QuestItemList::CanQuestBeShown(help::QuestType questType, int questId) const
{
    ref_ptr<m3d::ui::Wnd> wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_QUESTLOG);
    if (!wnd)
    {
        return false;
    }
    TakSebeQuestLogWnd* questLog = RT_DYNCAST(wnd.get(), TakSebeQuestLogWnd);
    if (!questLog || !questLog->CanQuestBeShown(questType, questId))
    {
        return false;
    }

    switch (questType)
    {
    case help::QUESTTYPE_STATIC:
    {
        ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
        if (!quest)
        {
            return false;
        }
        if (m_listType == LISTTYPE_PARENT_QUESTS)
        {
            return quest->GetParentId() == -1;
        }
        if (m_listType == LISTTYPE_CHILD_QUESTS)
        {
            return quest->GetParentId() == m_parentQuestId;
        }
        return false;
    }
    case help::QUESTTYPE_DYNAMIC:
        // Dynamic quests only ever appear in the top-level list.
        return m_listType == LISTTYPE_PARENT_QUESTS;
    default:
        return false;
    }
}

void QuestItemList::SortStaticQuests(std::vector<int, std::allocator<int>>& questIds) const
{
    std::stable_partition(questIds.begin(), questIds.end(), LessByMainStatus);
}

void QuestItemList::SortDynamicQuests(std::vector<int, std::allocator<int>>& questIds) const
{
    std::sort(questIds.begin(), questIds.end(), DynamicQuestLessByTakeGameTime);
}

// --- lookup ------------------------------------------------------------------------------------

int QuestItemList::GetQuestItemIdxByQuestId(help::QuestType questType, int questId) const
{
    for (int i = 0; i < GetCount(); ++i)
    {
        QuestItem* item = m_items[i].m_item;
        if (item && item->GetQuestType() == questType && item->GetQuestId() == questId)
        {
            return i;
        }
    }
    return -1;
}

QuestItem* QuestItemList::GetQuestItemByQuestId(help::QuestType questType, int questId) const
{
    int const idx = GetQuestItemIdxByQuestId(questType, questId);
    return idx == -1 ? nullptr : m_items[idx].m_item;
}

QuestItem* QuestItemList::GetQuestItemForAlphaNpTexture(help::QuestType questType, int questId) const
{
    if (m_listType == LISTTYPE_CHILD_QUESTS)
    {
        return nullptr;
    }

    int const idx = GetQuestItemIdxByQuestId(questType, questId);
    if (idx != -1)
    {
        if (QuestItem* item = m_items[idx].m_item)
        {
            return item;
        }
    }

    // Not shown directly - fall back to the parent quest's item (a child nav point still
    // changes the parent quest's alpha nav-point texture).
    if (questType != help::QUESTTYPE_DYNAMIC)
    {
        if (ai::Quest* quest = ai::theQuestManager->GetQuestById(questId))
        {
            return GetQuestItemByQuestId(questType, quest->GetParentId());
        }
    }
    return nullptr;
}

// --- events -----------------------------------------------------------------------------------

int QuestItemList::GameDataClear(bool)
{
    m3d::ui::ListBoxWnd<QuestItem*>::RemoveAllItems();
    return 1;
}

int QuestItemList::GameDataUpdate(void* data, int dataType)
{
    if (!Valid())
    {
        return 0;
    }
    switch (dataType)
    {
    case IE_EV_UM_NAVPOINT_ADDED:
        OnNavPointAdded(data);
        break;
    case IE_EV_UM_NAVPOINT_DELETED:
        OnNavPointDeleted(data);
        break;
    case IE_EV_SM_QUEST_WAS_TAKEN:
        OnQuestWasTaken(data);
        break;
    case IE_EV_SM_QUESTSTATE_CHANGED:
        OnQuestStateChanged(data);
        break;
    case IE_EV_SM_DYNAMIC_QUESTSTATE_CHANGED:
        OnDynamicQuestStateChanged(data);
        break;
    default:
        break;
    }
    return 1;
}

void QuestItemList::OnQuestStateChanged(void* data)
{
    if (data && IsChildOf(M3D_APP))
    {
        CreateItems();
    }
}

void QuestItemList::OnQuestWasTaken(void* data)
{
    if (data && IsChildOf(M3D_APP))
    {
        CreateItems();
    }
}

void QuestItemList::OnDynamicQuestStateChanged(void* data)
{
    if (data && IsChildOf(M3D_APP))
    {
        CreateItems();
    }
}

void QuestItemList::OnNavPointAdded(void* data)
{
    if (data && IsChildOf(M3D_APP))
    {
        OnNavPoint(data);
    }
}

void QuestItemList::OnNavPointDeleted(void* data)
{
    if (data && IsChildOf(M3D_APP))
    {
        OnNavPoint(data);
    }
}

void QuestItemList::OnNavPoint(void* data)
{
    if (!data)
    {
        return;
    }

    // data is the m3d::Event broadcast by NavPointManager::AddNavPoint through
    // Application::ImmediateMessage(UM_NAVPOINT_ADDED, navPointId, questId, objectType, navPointType,
    // levelName, aiParam) - see ExMachina 1.02 NoCD, QuestItemList::OnNavPoint @ 0x4FBF30, which reads
    // m_strEv (+0x0C), m_intEv[0] (+0x38) and m_intEv[1] (+0x3C).
    auto const* ev = static_cast<m3d::Event*>(data);

    // Only react to nav points placed on the level the player is currently on.
    if (help::GetCurrentLevelName() != ev->m_strEv)
    {
        return;
    }

    int const questId = ev->m_intEv[0];
    int const objectType = ev->m_intEv[1];

    help::QuestType questType = help::QUESTTYPE_NUM_QUEST_TYPES;
    if (objectType == NavPoint::OBJECT_TYPE_STATIC_QUEST)
    {
        questType = help::QUESTTYPE_STATIC;
    }
    else if (objectType == NavPoint::OBJECT_TYPE_DYNAMIC_QUEST)
    {
        questType = help::QUESTTYPE_DYNAMIC;
    }
    UpdateQuestItem(questType, questId);
}

int QuestItemList::UpdateQuestItem(help::QuestType questType, int questId)
{
    if (!Valid())
    {
        return 0;
    }

    int const idx = GetQuestItemIdxByQuestId(questType, questId);
    if (idx != -1)
    {
        if (QuestItem* item = m_items[idx].m_item)
        {
            item->FullUpdate();
        }
    }

    if (QuestItem* npItem = GetQuestItemForAlphaNpTexture(questType, questId))
    {
        npItem->UpdateOnQuestNavPointChangedChildIncluded();
    }
    return 1;
}

// --- add-to-station hooks -------------------------------------------------------------------------

int QuestItemList::OnBeforeAddToWndStation()
{
    CreateItems();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int QuestItemList::OnAfterAddToWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    if (m_listType == LISTTYPE_PARENT_QUESTS)
    {
        M3D_APP->EnqueueMessage(65691 /* UM_HELP */, 0, 0, 0, 0, {}, {});
    }
    return res;
}

// --- ListBoxWnd overrides -----------------------------------------------------------------------

int QuestItemList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    QuestItem* item = m_items[itemIdx].m_item;
    BoundsBase<float> const itemBounds = item->GetBounds();
    bounds.x0 = 0.0f;
    bounds.y0 = 0.0f;
    bounds.width = itemBounds.width;
    bounds.height = itemBounds.height;
    return 1;
}

int QuestItemList::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    QuestItem* item = m_items[itemIdx].m_item;
    BoundsBase<float> bounds = item->GetBounds();
    bounds.y0 = (di.m_clientRect.y0 - di.m_originalRect.y0) + org.y;
    item->SetBounds(bounds, true);
    return 1;
}

int QuestItemList::CompareItem(int, int)
{
    return 0;
}

int QuestItemList::DeleteItem(int itemIdx)
{
    if (QuestItem* item = m_items[itemIdx].m_item)
    {
        item->DecRef();
    }
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

void QuestItemList::SetCurSel(int idx)
{
    m3d::ui::ListBoxWnd<QuestItem*>::SetCurSel(idx);
    for (int i = 0; i < GetCount(); ++i)
    {
        if (QuestItem* item = m_items[i].m_item)
        {
            item->SetSelected(idx == i);
        }
    }
}

void QuestItemList::UpdateBounds()
{
    // Child-quest lists auto-size to the stacked height of their visible items.
    m_bounds.height = 0.0f;
    for (int i = 0; i < GetCount(); ++i)
    {
        if (m_items[i].m_item)
        {
            m_bounds.height += GetItemBounds(i).height;
        }
    }
}
