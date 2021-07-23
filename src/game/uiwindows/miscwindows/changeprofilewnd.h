#pragma once

class ChangeProfileWnd :  public m3d::ui::ModalWnd
{
public:
    virtual class m3d::Object * Clone();
    virtual ~ChangeProfileWnd();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual struct m3d::Class * GetClass() const ;
protected:
    class CStr GetSelectedProfileName() const ;
    ChangeProfileWnd();
    ChangeProfileWnd(class ChangeProfileWnd const &);
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
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
private:
    m3d::ui::StringsListBoxWnd *m_wndProfileList;
    m3d::ui::ButtonWnd *m_btnChose;
    m3d::ui::ButtonWnd *m_btnDelete;
    m3d::ui::Wnd *m_lblCurProfile;
    ChangeProfileWnd::AuxInfo m_aif;
};
