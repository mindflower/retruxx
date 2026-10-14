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
        unsigned int id);
    int SetupForParentQuest(int parentQuestId);

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override;
    virtual int DeleteItem(int itemIdx) override;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override;
    virtual void SetCurSel(int idx) override;
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
    QuestItemList::ListType m_listType;
    int m_parentQuestId;
    QuestItemList();
    QuestItemList(QuestItemList const& rhs);

public:
    virtual ~QuestItemList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classQuestItemList;
};
