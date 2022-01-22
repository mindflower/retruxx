#pragma once
#include <game/uimisc/guihelper.h>
#include <ui/uilistboxwnd.h>

class QuestItem;

class QuestItemList :  public m3d::ui::ListBoxWnd<QuestItem *>
{
public:
    enum ListType
    {
        LISTTYPE_PARENT_QUESTS = 0x0,
        LISTTYPE_CHILD_QUESTS = 0x1,
    };

public:
    int FullUpdate();
    static m3d::Class * GetBaseClass();
    virtual int CreateQuestItemList(ListType, BoundsBase<float> const &,unsigned int,unsigned int);
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    int CreateFromPattern(m3d::ui::Wnd const *,bool);
    int SetupForParentQuest(int);
    virtual m3d::Object * Clone();
    virtual ~QuestItemList();

protected:
    QuestItemList(QuestItemList const &);
    QuestItemList();
    virtual void SetCurSel(int);
    void OnNavPointAdded(void *);
    virtual int OnAfterAddToWndStation();
    int CreateItems();
    void OnQuestStateChanged(void *);
    int AddQuestItem(help::QuestType,int,bool);
    void OnNavPointDeleted(void *);
    int CreateStaticQuestItems();
    void OnDynamicQuestStateChanged(void *);
    QuestItem * GetQuestItemByQuestId(help::QuestType,int) const ;
    void GetDynamicQuestIdsToShow(std::vector<int, std::allocator<int> > &) const ;
    void GetStaticQuestIdsToShow(std::vector<int, std::allocator<int> > &) const ;
    virtual int GameDataClear(bool);
    virtual int MeasureItem(int, BoundsBase<float> &) const ;
    virtual int GameDataUpdate(void *,int);
    int GetQuestItemIdxByQuestId(help::QuestType,int) const ;
    int UpdateQuestItem(help::QuestType,int);
    void UpdateBounds();
    virtual int RenderItem(int, PointBase<float> const &, m3d::ui::DrawInfo const &);
    void OnNavPoint(void *);
    int CreateDynamicQuestItems();
    void OnQuestWasTaken(void *);
    QuestItem * GetQuestItemForAlphaNpTexture(help::QuestType,int) const ;
    QuestItem * CreateQuestItem(help::QuestType,int);
    virtual int OnBeforeAddToWndStation();
    void SortStaticQuests(std::vector<int, std::allocator<int> > &) const ;
    virtual int CompareItem(int,int);
    bool CanQuestBeShown(help::QuestType,int) const ;
    virtual int DeleteItem(int);
    void SortDynamicQuests(std::vector<int, std::allocator<int> > &) const ;

public:
    RT_CLASS_DECLARE(QuestItemList);

private:
    QuestItemList::ListType m_listType;
    int m_parentQuestId;
};
