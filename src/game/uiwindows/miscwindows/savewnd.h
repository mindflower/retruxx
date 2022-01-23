#pragma once
#include "lswnd.h"

namespace m3d
{
    namespace ui
    {
        class EditWnd;
    }
}

class SaveWnd :  public LSWnd
{
public:
    class SAuxInfo
    {
    public:
        SAuxInfo();

    private:
        CStr m_btnSaveName;
        CStr m_btnDeleteName;
        CStr m_editSaveName;
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual ~SaveWnd();

protected:
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    virtual void OnSaveSelectionChange();
    virtual void OnEnter();
    void OnEditChange();
    virtual int SetScreenshotForSave(int);
    void OnSave();
    void OnDelete();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    SaveWnd(SaveWnd const &);
    SaveWnd();

public:
    RT_CLASS_DECLARE(SaveWnd);

private:
    m3d::ui::ButtonWnd *m_btnSave;
    m3d::ui::ButtonWnd *m_btnDelete;
    m3d::ui::EditWnd *m_editSaveName;
    SaveWnd::SAuxInfo m_saif;
    bool m_bNeedScreenshot;
};
