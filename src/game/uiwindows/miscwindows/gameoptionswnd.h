#pragma once
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class ComboBoxWnd;
        class SliderWnd;
        class ButtonWnd;
        class CheckWnd;
    }
}

class GameOptionsWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_checkAutoHelpName = "checkAutoHelp";
        CStr m_sliderNumRepliesName = "sliderNumReplies";
        CStr m_btnNumRepliesPrevName = "btnNumRepliesPrev";
        CStr m_btnNumRepliesNextName = "btnNumRepliesNext";
        CStr m_cbGameDifficultyName = "cbGameDifficulty";
    };

public:
    virtual ~GameOptionsWnd();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;

protected:
    void InitNumRepliesControls();
    void ApplyNumReplies();
    void UpdateControls();
    GameOptionsWnd();
    GameOptionsWnd(GameOptionsWnd const &);
    void InitAutoHelpControls();
    void OnSliderNumRepliesChange(m3d::AIParam const &);
    void OnCheckAutoHelpClick(m3d::AIParam const &);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void UpdateNumRepliesControls();
    void ApplyGameDifficulty();
    void UpdateNumRepliesPrevNextButtonsState();
    void OnBtnNumRepliesNextClick(m3d::AIParam const &);
    void OnCbGameDifficultyChange(m3d::AIParam const &);
    void OnBtnNumRepliesPrevClick(m3d::AIParam const &);
    void InitGameDifficultyControls();
    virtual int GameDataSetup();
    void UpdateAutoHelpControls();
    virtual int OnBeforeAddToWndStation();
    void UpdateGameDifficultyControls();
    void ApplyAutoHelp();
    void InitControls();

public:
    RT_CLASS_DECLARE(GameOptionsWnd);

private:
    m3d::ui::CheckWnd *m_checkAutoHelp = nullptr;
    m3d::ui::SliderWnd *m_sliderNumReplies = nullptr;
    m3d::ui::ButtonWnd *m_btnNumRepliesPrev = nullptr;
    m3d::ui::ButtonWnd *m_btnNumRepliesNext = nullptr;
    m3d::ui::ComboBoxWnd *m_cbGameDifficulty = nullptr;
    int m_sliderNumRepliesBlocked = 0;
    int m_cbGameDifficultyBlocked = 0;
    GameOptionsWnd::AuxInfo m_aif;
};
