#pragma once

class CinemaPanel :  public m3d::ui::Wnd
{
public:
    class MessageTimeInfo
    {
    public:
        MessageTimeInfo(int, float);
    protected:
    private:
        int m_msgId;
        float m_delay;
    };

public:
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    void AddMessage(int,float);
    unsigned int GetTimeToTheEndOfMsg();
    bool MessageQueueIsEmpty() const ;
    void OnHide();
    void SkipMessage();
    void ClearMessages();
    static class m3d::Object * CreateObject();
    virtual ~CinemaPanel();
    virtual struct m3d::Class * GetClass() const ;
    void Clear();
protected:
    void ClearNormal();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    void SetPanelTypeForMsg(int);
    int _SetMsg(int);
    void SetupPortrait(class MsgInfo const *);
    void ClearBase();
    void HideAllControls();
    void GetControlsByPanelType(enum PanelType,class std::vector<class m3d::ui::Wnd *,class std::allocator<class m3d::ui::Wnd *> > &,bool) const ;
    void SetupTextScroll(class MsgInfo const *);
    virtual int GameDataClear(bool);
    void GetAllControls(class std::vector<class m3d::ui::Wnd *,class std::allocator<class m3d::ui::Wnd *> > &) const ;
    void SetupSound(class MsgInfo const *);
    void DeleteAllControls();
    virtual int GameDataSetup();
    void SetupTextNormal(class MsgInfo const *);
    CinemaPanel(class CinemaPanel const &);
    CinemaPanel();
    void InitControlsForMsgScroll(class MsgInfo const *);
    void InitControlsForMsgNormal(class MsgInfo const *);
    void UpdateAnimation();
    void SetupTime(class MsgInfo const *);
    void ShowControlsForPanelType(enum PanelType);
    void ClearScroll();
    enum PanelType GetPanelTypeByMsgType(enum MsgInfo::MsgType) const ;
    void InitControlsForMsgBase(class MsgInfo const *);
    void SetupImagesScroll(class MsgInfo const *);
    int StopSound();
    void SetPanelType(enum PanelType);
private:
    std::deque<CinemaPanel::MessageTimeInfo> m_msgInfos;
    CinemaPanel::AuxInfo m_aif;
    int m_soundTableId;
    int m_soundChannelId;
    int m_minTimeToExists;
    unsigned int m_curMessageStartTime;
    unsigned int m_curMessageEndTime;
    bool m_bIsShowingMessage;
    bool m_bSkipMessage;
    m3d::ui::Wnd *m_wndText;
    ItemModelWnd *m_wndsPortraits[2];
    CinemaPanel::PanelType m_panelType;
    m3d::ui::ImageWnd *m_wndPortraitOverlay;
    m3d::ui::Wnd *m_wndUpPanel;
    m3d::ui::Wnd *m_wndDownPanel;
    m3d::ui::ImageWnd *m_wndScrollImage;
    m3d::ui::ImageWnd *m_wndScrollImageUpOverlay;
    m3d::ui::ImageWnd *m_wndScrollImageDownOverlay;
    AutoScrollTextWnd *m_wndScrollText;
};
