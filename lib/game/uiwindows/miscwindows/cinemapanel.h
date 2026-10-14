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

class CinemaPanel : public m3d::ui::Wnd
{
    friend class CMiracle3d;

public:
    void Clear();
    void OnHide();
    unsigned int GetTimeToTheEndOfMsg();
    void AddMessage(int msgId, float delay);
    void SkipMessage();
    bool HasMsg();
    void ClearMessages();
    bool MessageQueueIsEmpty() const;

    struct AuxInfo
    {
        CStr m_wndPortraitName;
        CStr m_wndPortraitOverlayName;
        CStr m_wndTextName;
        CStr m_wndUpPanelName;
        CStr m_wndDownPanelName;
        CStr m_wndScrollImageName;
        CStr m_wndScrollImageUpOverlayName;
        CStr m_wndScrollImageDownOverlayName;
        CStr m_wndScrollTextName;
        AuxInfo(const CinemaPanel::AuxInfo&);
        AuxInfo();
    };
    
    struct MessageTimeInfo
    {
        int m_msgId;
        float m_delay;
        MessageTimeInfo(int msgId, float delay);
    };

protected:
    std::deque<CinemaPanel::MessageTimeInfo, std::allocator<CinemaPanel::MessageTimeInfo> > m_msgInfos;

    enum PanelType
    {
        PANELTYPE_NORMAL = 0,
        PANELTYPE_SCROLL = 1,
        PANELTYPE_NUM_PANEL_TYPES = 2,
    };

protected:
    virtual int OnPaint(const m3d::ui::DrawInfo& di) override;
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    int _SetMsg(int msgId);
    int StopSound();
    void UpdateAnimation();
    void SetPanelType(CinemaPanel::PanelType panelType);
    void SetPanelTypeForMsg(int msgId);
    void ShowControlsForPanelType(CinemaPanel::PanelType panelType);
    void InitControlsForMsgBase(const MsgInfo* msgInfo);
    void InitControlsForMsgNormal(const MsgInfo* msgInfo);
    void InitControlsForMsgScroll(const MsgInfo* msgInfo);
    void GetControlsByPanelType(CinemaPanel::PanelType panelType, std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*> >& controls, bool bAdd) const;
    void GetAllControls(std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*> >& controls) const;
    void HideAllControls();
    void DeleteAllControls();
    CinemaPanel::PanelType GetPanelTypeByMsgType(MsgInfo::MsgType msgType) const;
    void SetupSound(const MsgInfo* msgInfo);
    void SetupTime(const MsgInfo* msgInfo);
    void SetupPortrait(const MsgInfo* msgInfo);
    void SetupTextNormal(const MsgInfo* msgInfo);
    void SetupTextScroll(const MsgInfo* msgInfo);
    void SetupImagesScroll(const MsgInfo* msgInfo);
    void ClearBase();
    void ClearNormal();
    void ClearScroll();
    static const int NUM_PORTRAIT_SLOTS;
    static const int CINEMA_MSG_DEFAULT_TIME;
    CinemaPanel::AuxInfo m_aif;
    int m_soundTableId;
    int m_soundChannelId;
    int m_minTimeToExists;
    unsigned int m_curMessageStartTime;
    unsigned int m_curMessageEndTime;
    bool m_bIsShowingMessage;
    bool m_bSkipMessage;
    char Padding_196[2];
    m3d::ui::Wnd* m_wndText;
    ItemModelWnd* m_wndsPortraits[2];
    CinemaPanel::PanelType m_panelType;
    m3d::ui::ImageWnd* m_wndPortraitOverlay;
    m3d::ui::Wnd* m_wndUpPanel;
    m3d::ui::Wnd* m_wndDownPanel;
    m3d::ui::ImageWnd* m_wndScrollImage;
    m3d::ui::ImageWnd* m_wndScrollImageUpOverlay;
    m3d::ui::ImageWnd* m_wndScrollImageDownOverlay;
    AutoScrollTextWnd* m_wndScrollText;
    CinemaPanel();
    CinemaPanel(const CinemaPanel& rhs);

public:
    virtual  ~CinemaPanel() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classCinemaPanel;
};
