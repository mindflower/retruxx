#pragma once

class MainMenuUI :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~MainMenuUI();
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    void OnFinishVideoPlaying();
    virtual class m3d::Object * Clone();
protected:
    virtual int OnBeforeAddToWndStation();
    MainMenuUI();
    MainMenuUI(class MainMenuUI const &);
    void OnNewGame();
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    virtual int GameDataSetup();
    void OnStartVideoPlaying();
    void QuitToWindows();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void OnCurProfileChanged();
    virtual int GameDataUpdate(void *,int);
private:
    m3d::ui::Wnd *m_wndVersion;
    m3d::ui::Wnd *m_wndProfile;
    MainMenuUI::AuxInfo m_aif;
};
