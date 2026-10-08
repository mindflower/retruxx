#pragma once
#include <game/uimisc/guihelper.h>
#include <ui/uilistboxwnd.h>

class QuestItem;

class QuestItemList : public m3d::ui::ListBoxWnd<QuestItem*>
{
    friend class TakSebeQuestLogWnd;

public:
    enum ListType
    {
        LISTTYPE_PARENT_QUESTS = 0,
        LISTTYPE_CHILD_QUESTS = 1,
    };

public:
    int FullUpdate();
    int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    virtual int CreateQuestItemList(
        QuestItemList::ListType listType,
        BoundsBase<float> const& rc,
        unsigned int flags,
        unsigned int id) /* 0x144 */;
    int SetupForParentQuest(int parentQuestId);

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override /* 0x138 */;
    virtual int DeleteItem(int itemIdx) override /* 0x13c */;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
    virtual void SetCurSel(int idx) override /* 0x11c */;
    QuestItem* CreateQuestItem(help::QuestType questType, int questId);
    int AddQuestItem(help::QuestType questType, int questId, bool select);
    int UpdateQuestItem(help::QuestType questType, int questId);
    bool CanQuestBeShown(help::QuestType questType, int questId) const;
    int GetQuestItemIdxByQuestId(help::QuestType questType, int questId) const;
    QuestItem* GetQuestItemByQuestId(help::QuestType questType, int questId) const;
    void UpdateBounds();
    void OnNavPoint(void* data);
    void OnQuestWasTaken(void* data);
    void OnQuestStateChanged(void* data);
    void OnDynamicQuestStateChanged(void* data);
    void OnNavPointAdded(void* data);
    void OnNavPointDeleted(void* data);
    int CreateItems();
    int CreateStaticQuestItems();
    int CreateDynamicQuestItems();
    void SortStaticQuests(std::vector<int, std::allocator<int>>& questIds) const;
    void SortDynamicQuests(std::vector<int, std::allocator<int>>& dQuestIds) const;
    void GetStaticQuestIdsToShow(std::vector<int, std::allocator<int>>& questIds) const;
    void GetDynamicQuestIdsToShow(std::vector<int, std::allocator<int>>& dQuestIds) const;
    QuestItem* GetQuestItemForAlphaNpTexture(help::QuestType questType, int questIdOnThatNpChanged) const;
    /* 0x0238 */ QuestItemList::ListType m_listType;
    /* 0x023c */ int m_parentQuestId;
    QuestItemList();
    QuestItemList(QuestItemList const& rhs);

public:
    virtual ~QuestItemList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classQuestItemList;
}; /* size: 0x0240 */
