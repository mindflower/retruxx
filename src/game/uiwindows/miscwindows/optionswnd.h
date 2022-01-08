#pragma once

class OptionTabButton :  public m3d::ui::ButtonWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    enum OptionsWnd::Tab GetTabId() const ;
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    void Select(bool);
    int SetupForTab(enum OptionsWnd::Tab);
    virtual class m3d::Object * Clone();
    virtual ~OptionTabButton();
    bool IsSelected() const ;
    static struct m3d::Class * GetBaseClass();
protected:
    OptionTabButton(class OptionTabButton const &);
    OptionTabButton();
    void UpdatePane();
private:
    bool m_bSelected;
    OptionsWnd::Tab m_tabId;
};

class GameOptionsWnd :  public m3d::ui::Wnd
{
public:
    virtual ~GameOptionsWnd();
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
protected:
    void InitNumRepliesControls();
    void ApplyNumReplies();
    void UpdateControls();
    GameOptionsWnd();
    GameOptionsWnd(class GameOptionsWnd const &);
    void InitAutoHelpControls();
    void OnSliderNumRepliesChange(class m3d::AIParam const &);
    void OnCheckAutoHelpClick(class m3d::AIParam const &);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void UpdateNumRepliesControls();
    void ApplyGameDifficulty();
    void UpdateNumRepliesPrevNextButtonsState();
    void OnBtnNumRepliesNextClick(class m3d::AIParam const &);
    void OnCbGameDifficultyChange(class m3d::AIParam const &);
    void OnBtnNumRepliesPrevClick(class m3d::AIParam const &);
    void InitGameDifficultyControls();
    virtual int GameDataSetup();
    void UpdateAutoHelpControls();
    virtual int OnBeforeAddToWndStation();
    void UpdateGameDifficultyControls();
    void ApplyAutoHelp();
    void InitControls();
private:
    Clone();
    GameDataSetup();
    m3d::ui::CheckWnd *m_checkAutoHelp;
    m3d::ui::SliderWnd *m_sliderNumReplies;
    m3d::ui::ButtonWnd *m_btnNumRepliesPrev;
    m3d::ui::ButtonWnd *m_btnNumRepliesNext;
    m3d::ui::ComboBoxWnd *m_cbGameDifficulty;
    int m_sliderNumRepliesBlocked;
    int m_cbGameDifficultyBlocked;
    GameOptionsWnd::AuxInfo m_aif;
};
