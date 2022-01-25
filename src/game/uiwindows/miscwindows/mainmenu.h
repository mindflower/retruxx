#pragma once
#include <ui/ui.h>

class MainMenuUI :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_wndVersionName = "wndVersion";
        CStr m_wndProfileName = "wndProfile";
        CStr m_strIdProfileDontChosen = "ProfileDontChosen";
        CStr m_strIdProfile = "Profile";
        unsigned int m_colorProfileName = 0xFFE5CA9D;
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual ~MainMenuUI();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    void OnFinishVideoPlaying();
    virtual m3d::Object * Clone();

protected:
    virtual int OnBeforeAddToWndStation();
    MainMenuUI();
    MainMenuUI(MainMenuUI const &);
    void OnNewGame();
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    virtual int GameDataSetup();
    void OnStartVideoPlaying();
    void QuitToWindows();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    void OnCurProfileChanged();
    virtual int GameDataUpdate(void *,int);

public:
    RT_CLASS_DECLARE(MainMenuUI);

private:
    m3d::ui::Wnd *m_wndVersion = nullptr;
    m3d::ui::Wnd *m_wndProfile = nullptr;
    MainMenuUI::AuxInfo m_aif;
};
