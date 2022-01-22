#pragma once
#include <deque>
#include <game/uimisc/msgmanager.h>
#include <ui/ui.h>

class AutoScrollTextWnd;

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class ItemModelWnd;

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

    enum PanelType
    {
        PANELTYPE_NORMAL = 0x0,
        PANELTYPE_SCROLL = 0x1,
        PANELTYPE_NUM_PANEL_TYPES = 0x2,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndPortraitName;
        CStr m_wndPortraitOverlayName;
        CStr m_wndTextName;
        CStr m_wndUpPanelName;
        CStr m_wndDownPanelName;
        CStr m_wndScrollImageName;
        CStr m_wndScrollImageUpOverlayName;
        CStr m_wndScrollImageDownOverlayName;
        CStr m_wndScrollTextName;
    };


public:
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    void AddMessage(int,float);
    unsigned int GetTimeToTheEndOfMsg();
    bool MessageQueueIsEmpty() const ;
    void OnHide();
    void SkipMessage();
    void ClearMessages();
    static m3d::Object * CreateObject();
    virtual ~CinemaPanel();
    virtual m3d::Class * GetClass() const ;
    void Clear();

protected:
    void ClearNormal();
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    void SetPanelTypeForMsg(int);
    int _SetMsg(int);
    void SetupPortrait(MsgInfo const *);
    void ClearBase();
    void HideAllControls();
    void GetControlsByPanelType(PanelType,std::vector<m3d::ui::Wnd *> &,bool) const ;
    void SetupTextScroll(MsgInfo const *);
    virtual int GameDataClear(bool);
    void GetAllControls(std::vector<m3d::ui::Wnd *> &) const ;
    void SetupSound(MsgInfo const *);
    void DeleteAllControls();
    virtual int GameDataSetup();
    void SetupTextNormal(MsgInfo const *);
    CinemaPanel(CinemaPanel const &);
    CinemaPanel();
    void InitControlsForMsgScroll(MsgInfo const *);
    void InitControlsForMsgNormal(MsgInfo const *);
    void UpdateAnimation();
    void SetupTime(MsgInfo const *);
    void ShowControlsForPanelType(PanelType);
    void ClearScroll();
    enum PanelType GetPanelTypeByMsgType(MsgInfo::MsgType) const ;
    void InitControlsForMsgBase(MsgInfo const *);
    void SetupImagesScroll(MsgInfo const *);
    int StopSound();
    void SetPanelType(PanelType);

public:
    RT_CLASS_DECLARE(CinemaPanel);

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
