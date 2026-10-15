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
    }  // namespace ui
}  // namespace m3d

class SplashWnd : public m3d::ui::Wnd
{
public:
    static int const NUM_LEVEL_SPLASHES;
    void StartSplashing(int numSplashes);
    void ShowSplash(int processStatus, CStr const& text);

    struct AuxInfo
    {
        CStr m_progressBarName;
        CStr m_wndImageName;
        CStr m_lblTextName;
        AuxInfo(SplashWnd::AuxInfo const&);
        AuxInfo();
    };

    using ImageVector = retruxx::vector<m3d::rend::TexHandle>;

protected:
    virtual int GameDataSetup() override;
    m3d::rend::TexHandle GetRandomImage(retruxx::vector<m3d::rend::TexHandle> const& images) const;
    retruxx::vector<m3d::rend::TexHandle> GetLevelSplashes(CStr const& levelName) const;

    m3d::ui::ProgressBarWnd* m_progressBar;
    m3d::ui::ImageWnd* m_wndImage;
    m3d::ui::Wnd* m_lblText;
    SplashWnd::AuxInfo m_aif;
    int m_curSplash;
    int m_numSplashes;
    CStr m_text;

    SplashWnd();
    SplashWnd(SplashWnd const& rhs);

public:
    virtual ~SplashWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SplashWnd);
};
