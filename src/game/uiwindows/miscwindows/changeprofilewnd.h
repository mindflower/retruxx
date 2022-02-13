#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class StringsListBoxWnd;
    }
}

class ChangeProfileWnd : public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_wndProfileListName = "wndProfileList";
        CStr m_btnChoseName = "btnChose";
        CStr m_btnDeleteName = "btnDelete";
        CStr m_lblCurProfileName = "lblCurProfileName";
        CStr m_strIdCurProfile = "CurProfile";
        unsigned int m_curProfileNameColor = 0xFF904040;
    };

public:
    virtual m3d::Object * Clone();
    virtual ~ChangeProfileWnd();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Class * GetClass() const ;

protected:
    CStr GetSelectedProfileName() const ;
    ChangeProfileWnd();
    ChangeProfileWnd(ChangeProfileWnd const &);
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    void OnChangeProfileSelection();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    int ChoseProfileAndExit();
    void OnProfilesListChanged();
    virtual int GameDataUpdate(void *,int);
    int ChoseProfile();
    void OnCurProfileChanged();
    int DeleteProfile();
    void Clear();
    int CreateProfile();
    void FullUpdate();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);

public:
    RT_CLASS_DECLARE(ChangeProfileWnd);

private:
    m3d::ui::StringsListBoxWnd *m_wndProfileList = nullptr;
    m3d::ui::ButtonWnd *m_btnChose = nullptr;
    m3d::ui::ButtonWnd *m_btnDelete = nullptr;
    m3d::ui::Wnd *m_lblCurProfile = nullptr;
    ChangeProfileWnd::AuxInfo m_aif;
};
