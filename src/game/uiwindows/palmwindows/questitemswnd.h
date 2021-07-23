#pragma once

class QuestItemsWnd :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~QuestItemsWnd();
    virtual class m3d::Object * Clone();
protected:
    void ClearItems();
    void CreateItems();
    virtual int OnAfterAddToWndStation();
    void FullUpdate();
    virtual int GameDataSetup();
    void OnQuestItemsChanged();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    QuestItemsWnd(class QuestItemsWnd const &);
    QuestItemsWnd();
    void OnItemsListSelChanged(class m3d::AIParam const &);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
private:
    QuestItemsWnd::AuxInfo m_aif;
    ContextModelWnd *m_wndPicture;
    m3d::ui::TextBoxWnd *m_wndDiz;
    CheckList *m_wndItemsList;
};
