#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class EditWnd;
    }
}

class NewProfileWnd :  public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_editNameName = "editName";
        CStr m_btnCancelName = "btnCancel";
    };

public:
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~NewProfileWnd();

protected:
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataSetup();
    void OnEditChange();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    int CreateNewProfile();
    NewProfileWnd(NewProfileWnd const &);
    NewProfileWnd();

public:
    RT_CLASS_DECLARE(NewProfileWnd);

private:
    m3d::ui::EditWnd *m_editName = nullptr;
    m3d::ui::ButtonWnd *m_btnCancel = nullptr;
    AuxInfo m_aif;
};
