#pragma once

class SaveSellList :  public m3d::ui::Wnd
{
public:
    virtual class m3d::Object * Clone();
    static struct m3d::Class * GetBaseClass();
    void ShowAtBegin();
    static class m3d::Object * CreateObject();
    int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    void GetSavedObjectInfos(class std::vector<class ObjectInfo *,class std::allocator<class ObjectInfo *> > &) const ;
    virtual ~SaveSellList();
    void SetCurSel(int);
    int AddSaveButton(class ObjectInfo *);
    int GetSaveButtonIdByObjectInfo(class ObjectInfo *) const ;
    class ObjectInfo * GetObjectInfoBySaveButtonId(int) const ;
    int GetCurSel() const ;
    int RemoveSaveButton(class ObjectInfo *);
    virtual struct m3d::Class * GetClass() const ;
    void ShowAtEnd();
protected:
    SaveSellList(class SaveSellList const &);
    SaveSellList();
    void ScrollNext();
    virtual int GameDataSave(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    void UpdateNextPrevButtonState();
    void EnsureShowButton(int);
    void ScrollPrev();
    bool CanScrollPrev();
    void RecalcLayot();
    virtual int GameDataClear(bool);
    bool CanScrollNext();
    void SelectButton(int);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int GameDataLoad(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
private:
    m3d::ui::ButtonWnd *m_btnNext;
    m3d::ui::ButtonWnd *m_btnPrev;
    SaveSellList::AuxInfo m_aif;
    std::vector<SaveSellButton *> m_saveButtons;
    BoundsBase<float> m_listBounds;
    int m_firstSaveBtnId;
    int m_lastSaveBtnId;
    int m_selBtnId;
};

class SaveSellButton :  public m3d::ui::ButtonWnd
{
public:
    virtual ~SaveSellButton();
    virtual class m3d::Object * Clone();
    int SetUp(class ObjectInfo *,float,int);
    static struct m3d::Class * GetBaseClass();
    void Select(bool);
    static class m3d::Object * CreateObject();
    bool IsSelected() const ;
    virtual struct m3d::Class * GetClass() const ;
    class ObjectInfo * GetObjectInfo() const ;
protected:
    SaveSellButton();
    SaveSellButton(class SaveSellButton const &);
private:
    ObjectInfo *m_objectInfo;
    m3d::ui::Wnd *m_lblObjectName;
    bool m_bSelected;
};
