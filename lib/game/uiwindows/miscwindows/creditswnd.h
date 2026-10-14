#pragma once
#include <vector>

#include <core/console/cvar.h>
#include <ui/wnd.h>

class ItemModelWnd;

class CreditsWnd : public m3d::ui::ModalWnd
{
    struct AuxInfo
    {
        AuxInfo(CreditsWnd::AuxInfo const&);
        AuxInfo();
        CStr m_wndTextName;
        CStr m_wndPortraitName;
        CStr m_texIdBg;
    };

    struct PageInfo
    {
        PageInfo(CreditsWnd::PageInfo const&);
        PageInfo();
        CStr m_text;
        float m_fontSize;
        CStr m_modelName;
        unsigned int m_modelSkin;
        unsigned int m_modelCfg;
        float m_showTime;
    };

    struct Pointer
    {
        Pointer(CreditsWnd::Pointer const&);
        Pointer();
        ~Pointer();
        void Update(float startTime, float delayPeriod, bool bInversedDirection);
        void Draw(m3d::ui::DrawInfo const& di);
        void Init();
        void Clear();
        float m_minAngle;
        float m_maxAngle;
        float m_curAngle;
        PointBase<float> m_texSz;
        PointBase<float> m_rotationCenter;
        PointBase<float> m_rotationScreenCenter;
        CStr m_texId;
        m3d::rend::TexHandle m_texture;
    };

    using PageInfoVector = std::vector<CreditsWnd::PageInfo*, std::allocator<CreditsWnd::PageInfo*> >;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void LoadPageInfo();
    void OnNewFrame();
    void StartShowingPages();
    void CheckAndShowCurrentPage();
    void CheckAndShowNextPage();
    void DoShowPage();
    void DoBreak();
    void UpdateControls();
    void UpdatePortrait();
    void UpdateText();
    void UpdateBg();
    void UpdatePointer();
    void UpdateStartTime();
    void PlaySwitchSound();
    void PlayAmbientSound();
    void StopSwitchSound();
    void StopAmbientSound();
    int PlaySoundA(int soundTableId, bool bLooped);
    void StopSound(int soundChannelId);
    void PlayMusic();
    void StopMusic();
    void StopAllSoundsAndMusic();
    void Exit();
    bool IsTimeEnded() const;
    bool NeedExit() const;
    bool IsCurPageIdValid() const;
    void ClearPages();
    void Reset();
    void UpdatePortraitAnimation();

    m3d::ui::Wnd* m_wndText;
    ItemModelWnd* m_wndPortrait;
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
    std::vector<CreditsWnd::PageInfo*, std::allocator<CreditsWnd::PageInfo*> > m_pageInfos;
    int m_curPageId;
    bool m_bInBreak;
    CreditsWnd::Pointer m_pointer;
    float m_startTime;
    m3d::CVar m_cvPathToPageInfo;
    CreditsWnd::AuxInfo m_aif;

    CreditsWnd();
    CreditsWnd(CreditsWnd const& rhs);

public:
    virtual ~CreditsWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(CreditsWnd);
};
