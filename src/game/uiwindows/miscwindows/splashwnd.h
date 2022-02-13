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
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_progressBarName = "progressBar";
        CStr m_wndImageName = "wndImage";
        CStr m_lblTextName = "lblText";
    };

public:
    virtual m3d::Class* GetClass(void) const;
    static m3d::Object* CreateObject();
    void ShowSplash(int,CStr const&);
    virtual ~SplashWnd(void);
    void StartSplashing(int);
    static m3d::Class* GetBaseClass();

protected:
    virtual int GameDataSetup();
    std::vector<m3d::rend::TexHandle, std::allocator<m3d::rend::TexHandle> > GetLevelSplashes(CStr const&) const;
    SplashWnd();
    SplashWnd(SplashWnd const&);
    m3d::rend::TexHandle GetRandomImage(std::vector<m3d::rend::TexHandle, std::allocator<m3d::rend::TexHandle> > const&) const;

public:
    RT_CLASS_DECLARE(SplashWnd);

private:
    m3d::ui::ProgressBarWnd* m_progressBar = nullptr;
    m3d::ui::ImageWnd* m_wndImage = nullptr;
    m3d::ui::Wnd* m_lblText = 0;
    SplashWnd::AuxInfo m_aif;
    int m_curSplash = -1;
    int m_numSplashes = 11;
    CStr m_text;
};
