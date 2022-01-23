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

    private:
        CStr m_wndProfileListName;
        CStr m_btnChoseName;
        CStr m_btnDeleteName;
        CStr m_lblCurProfileName;
        CStr m_strIdCurProfile;
        unsigned int m_curProfileNameColor;
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
    m3d::ui::StringsListBoxWnd *m_wndProfileList;
    m3d::ui::ButtonWnd *m_btnChose;
    m3d::ui::ButtonWnd *m_btnDelete;
    m3d::ui::Wnd *m_lblCurProfile;
    ChangeProfileWnd::AuxInfo m_aif;
};
