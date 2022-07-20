#pragma once
#include <core/console/cvar.h>
#include <ui/wnd.h>

class ItemModelWnd;

class CreditsWnd :  public m3d::ui::ModalWnd
{
public:
    class PageInfo
    {
    public:
        PageInfo();

    private:
        CStr m_text;
        float m_fontSize;
        CStr m_modelName;
        unsigned int m_modelSkin;
        unsigned int m_modelCfg;
        float m_showTime;
    };

    class Pointer
    {
    public:
        Pointer();
        void Update(float, float, bool);
        void Draw(m3d::ui::DrawInfo const&);
        ~Pointer();

    private:
        void Clear(void);
        void Init(void);

    private:
        float m_minAngle = -0.78539819;
        float m_maxAngle = 0.78539819;
        float m_curAngle = -0.78539819;
        PointBase<float> m_texSz {32.0, 256.0};
        PointBase<float> m_rotationCenter{ 16.0, 129.0 };
        PointBase<float> m_rotationScreenCenter{ 186.0, 290.0 };
        CStr m_texId = "CreditsPointer";
        m3d::rend::TexHandle m_texture;
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndTextName;
        CStr m_wndPortraitName;
        CStr m_texIdBg;
    };

public:
    virtual ~CreditsWnd();
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();

protected:
    virtual int OnAfterRemoveFromWndStation();
    void StartShowingPages();
    void UpdateStartTime();
    CreditsWnd();
    CreditsWnd(CreditsWnd const &);
    void UpdateBg();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    void UpdatePortrait();
    void Exit();
    void DoShowPage();
    void PlaySwitchSound();
    void DoBreak();
    void StopSwitchSound();
    void CheckAndShowCurrentPage();
    void UpdateText();
    bool IsCurPageIdValid() const ;
    int PlaySoundA(int,bool);
    bool IsTimeEnded() const ;
    void StopSound(int);
    void LoadPageInfo();
    virtual int OnBeforeAddToWndStation();
    void StopAllSoundsAndMusic();
    void UpdateControls();
    void ClearPages();
    void UpdatePointer();
    void PlayMusic();
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    void StopMusic();
    void Reset();
    void UpdatePortraitAnimation();
    void OnNewFrame();
    virtual int GameDataSetup();
    bool NeedExit() const ;
    void CheckAndShowNextPage();
    void PlayAmbientSound();
    void StopAmbientSound();

public:
    RT_CLASS_DECLARE(CreditsWnd);

private:
    m3d::ui::Wnd *m_wndText;
    ItemModelWnd *m_wndPortrait;
    m3d::rend::TexHandle m_texBgShow;
    m3d::rend::TexHandle m_texBgBreak;
    float m_breakTime;
    int m_showSoundTableId;
    int m_breakSoundTableId;
    int m_ambientSoundTableId;
    int m_musicTableId;
    int m_switchSoundChannelId;
    int m_ambientSoundChannelId;
    int m_musicChannelId;
    std::vector<CreditsWnd::PageInfo *> m_pageInfos;
    int m_curPageId;
    bool m_bInBreak;
    CreditsWnd::Pointer m_pointer;
    float m_startTime;
    m3d::CVar m_cvPathToPageInfo;
    CreditsWnd::AuxInfo m_aif;
};
