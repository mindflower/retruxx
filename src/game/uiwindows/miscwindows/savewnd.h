#pragma once

class SaveWnd :  public LSWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~SaveWnd();
protected:
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual void OnSaveSelectionChange();
    virtual void OnEnter();
    void OnEditChange();
    virtual int SetScreenshotForSave(int);
    void OnSave();
    void OnDelete();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    SaveWnd(class SaveWnd const &);
    SaveWnd();
private:
    m3d::ui::ButtonWnd *m_btnSave;
    m3d::ui::ButtonWnd *m_btnDelete;
    m3d::ui::EditWnd *m_editSaveName;
    SaveWnd::SAuxInfo m_saif;
    bool m_bNeedScreenshot;
};
