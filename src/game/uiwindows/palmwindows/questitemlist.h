#pragma once

class QuestItemList :  public m3d::ui::ListBoxWnd<QuestItem *>
{
public:
    int FullUpdate();
    static struct m3d::Class * GetBaseClass();
    virtual int CreateQuestItemList(enum ListType,struct BoundsBase<float> const &,unsigned int,unsigned int);
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    int SetupForParentQuest(int);
    virtual class m3d::Object * Clone();
    virtual ~QuestItemList();
protected:
    QuestItemList(class QuestItemList const &);
    QuestItemList();
    virtual void SetCurSel(int);
    void OnNavPointAdded(void *);
    virtual int OnAfterAddToWndStation();
    int CreateItems();
    void OnQuestStateChanged(void *);
    int AddQuestItem(enum help::QuestType,int,bool);
    void OnNavPointDeleted(void *);
    int CreateStaticQuestItems();
    void OnDynamicQuestStateChanged(void *);
    class QuestItem * GetQuestItemByQuestId(enum help::QuestType,int) const ;
    void GetDynamicQuestIdsToShow(class std::vector<int,class std::allocator<int> > &) const ;
    void GetStaticQuestIdsToShow(class std::vector<int,class std::allocator<int> > &) const ;
    virtual int GameDataClear(bool);
    virtual int MeasureItem(int,struct BoundsBase<float> &) const ;
    virtual int GameDataUpdate(void *,int);
    int GetQuestItemIdxByQuestId(enum help::QuestType,int) const ;
    int UpdateQuestItem(enum help::QuestType,int);
    void UpdateBounds();
    virtual int RenderItem(int,class PointBase<float> const &,struct m3d::ui::DrawInfo const &);
    void OnNavPoint(void *);
    int CreateDynamicQuestItems();
    void OnQuestWasTaken(void *);
    class QuestItem * GetQuestItemForAlphaNpTexture(enum help::QuestType,int) const ;
    class QuestItem * CreateQuestItem(enum help::QuestType,int);
    virtual int OnBeforeAddToWndStation();
    void SortStaticQuests(class std::vector<int,class std::allocator<int> > &) const ;
    virtual int CompareItem(int,int);
    bool CanQuestBeShown(enum help::QuestType,int) const ;
    virtual int DeleteItem(int);
    void SortDynamicQuests(class std::vector<int,class std::allocator<int> > &) const ;
private:
    QuestItemList::ListType m_listType;
    int m_parentQuestId;
};
