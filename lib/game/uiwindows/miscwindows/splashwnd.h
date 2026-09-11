#pragma once
#include <ui/ui.h>
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
        class ProgressBarWnd;
        class ButtonWnd;
    }
}

class SplashWnd : public m3d::ui::Wnd
{
public:
    static int const NUM_LEVEL_SPLASHES;
    void StartSplashing(int numSplashes);
    void ShowSplash(int processStatus, CStr const& text);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_progressBarName;
        /* 0x000c */ CStr m_wndImageName;
        /* 0x0018 */ CStr m_lblTextName;
        AuxInfo(SplashWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0024 */

    using ImageVector = retruxx::vector<m3d::rend::TexHandle>;

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    m3d::rend::TexHandle GetRandomImage(retruxx::vector<m3d::rend::TexHandle> const& images) const;
    retruxx::vector<m3d::rend::TexHandle> GetLevelSplashes(CStr const& levelName) const;

    /* 0x0220 */ m3d::ui::ProgressBarWnd* m_progressBar;
    /* 0x0224 */ m3d::ui::ImageWnd* m_wndImage;
    /* 0x0228 */ m3d::ui::Wnd* m_lblText;
    /* 0x022c */ SplashWnd::AuxInfo m_aif;
    /* 0x0250 */ int m_curSplash;
    /* 0x0254 */ int m_numSplashes;
    /* 0x0258 */ CStr m_text;

    SplashWnd();
    SplashWnd(SplashWnd const& rhs);

public:
    virtual ~SplashWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(SplashWnd);
}; /* size: 0x0264 */
