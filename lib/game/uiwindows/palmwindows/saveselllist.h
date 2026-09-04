#pragma once
#include <ui/button.h>
#include <ui/ui.h>

class SaveSellButton : public m3d::ui::ButtonWnd
{
public:
    class AuxInfo
    {
        friend class SaveSellButton;

    public:
        AuxInfo();

    private:
        CStr m_paneName;
        CStr m_selectPaneName;
        float m_space;
        unsigned int m_textColor;
    };

public:
    virtual ~SaveSellButton();
    virtual m3d::Object* Clone();
    int SetUp(class ObjectInfo*, float, int);
    static m3d::Class* GetBaseClass();
    void Select(bool);
    static m3d::Object* CreateObject();
    bool IsSelected() const;
    virtual m3d::Class* GetClass() const;
    ObjectInfo* GetObjectInfo() const;

protected:
    SaveSellButton();
    SaveSellButton(SaveSellButton const&);

public:
    RT_CLASS_DECLARE(SaveSellButton);

    static SaveSellButton::AuxInfo m_aif;

private:
    ObjectInfo* m_objectInfo;
    m3d::ui::Wnd* m_lblObjectName;
    bool m_bSelected;
};


class SaveSellList :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
        friend class SaveSellList;

    public:
        AuxInfo();

    private:
        CStr m_btnNextName;
        CStr m_btnPrevName;
        CStr m_wndEmbossBtnNextName;
        CStr m_wndEmbossBtnPrevName;
        float m_space;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    void ShowAtBegin();
    static m3d::Object * CreateObject();
    int CreateFromPattern(m3d::ui::Wnd const *,bool);
    void GetSavedObjectInfos(std::vector<ObjectInfo *, std::allocator<ObjectInfo *> > &) const ;
    virtual ~SaveSellList();
    void SetCurSel(int);
    int AddSaveButton(ObjectInfo *);
    int GetSaveButtonIdByObjectInfo(ObjectInfo *) const ;
    ObjectInfo * GetObjectInfoBySaveButtonId(int) const ;
    int GetCurSel() const ;
    int RemoveSaveButton(ObjectInfo *);
    virtual m3d::Class * GetClass() const ;
    void ShowAtEnd();

protected:
    SaveSellList(SaveSellList const &);
    SaveSellList();
    void ScrollNext();
    virtual int GameDataSave(m3d::cmn::XmlFile *, m3d::cmn::XmlNode *);
    void UpdateNextPrevButtonState();
    void EnsureShowButton(int);
    void ScrollPrev();
    bool CanScrollPrev();
    void RecalcLayot();
    virtual int GameDataClear(bool);
    bool CanScrollNext();
    void SelectButton(int);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    virtual int GameDataLoad(m3d::cmn::XmlFile *, m3d::cmn::XmlNode *);

public:
    RT_CLASS_DECLARE(SaveSellList);

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
