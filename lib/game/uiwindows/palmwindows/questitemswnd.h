#pragma once
#include <ui/ui.h>

class CheckList;

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}

class ContextModelWnd;

class QuestItemsWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndPictureName;
        CStr m_wndDizName;
        CStr m_wndItemsListName;
    };

public:
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual ~QuestItemsWnd();
    virtual m3d::Object * Clone();

protected:
    void ClearItems();
    void CreateItems();
    virtual int OnAfterAddToWndStation();
    void FullUpdate();
    virtual int GameDataSetup();
    void OnQuestItemsChanged();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    QuestItemsWnd(QuestItemsWnd const &);
    QuestItemsWnd();
    void OnItemsListSelChanged(m3d::AIParam const &);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();

public:
    RT_CLASS_DECLARE(QuestItemsWnd);

private:
    QuestItemsWnd::AuxInfo m_aif;
    ContextModelWnd *m_wndPicture;
    m3d::ui::TextBoxWnd *m_wndDiz;
    CheckList *m_wndItemsList;
};
