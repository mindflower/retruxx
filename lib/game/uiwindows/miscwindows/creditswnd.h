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
        /* 0x0000 */ CStr m_wndTextName;
        /* 0x000c */ CStr m_wndPortraitName;
        /* 0x0018 */ CStr m_texIdBg;
    }; /* size: 0x0024 */

    struct PageInfo
    {
        PageInfo(CreditsWnd::PageInfo const&);
        PageInfo();
        /* 0x0000 */ CStr m_text;
        /* 0x000c */ float m_fontSize;
        /* 0x0010 */ CStr m_modelName;
        /* 0x001c */ unsigned int m_modelSkin;
        /* 0x0020 */ unsigned int m_modelCfg;
        /* 0x0024 */ float m_showTime;
    }; /* size: 0x0028 */

    struct Pointer
    {
        Pointer(CreditsWnd::Pointer const&);
        Pointer();
        ~Pointer();
        void Update(float startTime, float delayPeriod, bool bInversedDirection);
        void Draw(m3d::ui::DrawInfo const& di);
        void Init();
        void Clear();
        /* 0x0000 */ float m_minAngle;
        /* 0x0004 */ float m_maxAngle;
        /* 0x0008 */ float m_curAngle;
        /* 0x000c */ PointBase<float> m_texSz;
        /* 0x0014 */ PointBase<float> m_rotationCenter;
        /* 0x001c */ PointBase<float> m_rotationScreenCenter;
        /* 0x0024 */ CStr m_texId;
        /* 0x0030 */ m3d::rend::TexHandle m_texture;
    }; /* size: 0x0034 */

    using PageInfoVector = std::vector<CreditsWnd::PageInfo*, std::allocator<CreditsWnd::PageInfo*> >;

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
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

    /* 0x0224 */ m3d::ui::Wnd* m_wndText;
    /* 0x0228 */ ItemModelWnd* m_wndPortrait;
    /* 0x022c */ m3d::rend::TexHandle m_texBgShow;
    /* 0x0230 */ m3d::rend::TexHandle m_texBgBreak;
    /* 0x0234 */ float m_breakTime;
    /* 0x0238 */ int m_showSoundTableId;
    /* 0x023c */ int m_breakSoundTableId;
    /* 0x0240 */ int m_ambientSoundTableId;
    /* 0x0244 */ int m_musicTableId;
    /* 0x0248 */ int m_switchSoundChannelId;
    /* 0x024c */ int m_ambientSoundChannelId;
    /* 0x0250 */ int m_musicChannelId;
    /* 0x0254 */ std::vector<CreditsWnd::PageInfo*, std::allocator<CreditsWnd::PageInfo*> > m_pageInfos;
    /* 0x0264 */ int m_curPageId;
    /* 0x0268 */ bool m_bInBreak;
    /* 0x026c */ CreditsWnd::Pointer m_pointer;
    /* 0x02a0 */ float m_startTime;
    /* 0x02a4 */ m3d::CVar m_cvPathToPageInfo;
    /* 0x02d0 */ CreditsWnd::AuxInfo m_aif;

    CreditsWnd();
    CreditsWnd(CreditsWnd const& rhs);

public:
    virtual ~CreditsWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(CreditsWnd);
}; /* size: 0x02f4 */
