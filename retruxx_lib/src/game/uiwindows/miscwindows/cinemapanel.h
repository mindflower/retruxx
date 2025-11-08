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
        /* 0x0000 */ CStr m_wndPortraitName;
        /* 0x000c */ CStr m_wndPortraitOverlayName;
        /* 0x0018 */ CStr m_wndTextName;
        /* 0x0024 */ CStr m_wndUpPanelName;
        /* 0x0030 */ CStr m_wndDownPanelName;
        /* 0x003c */ CStr m_wndScrollImageName;
        /* 0x0048 */ CStr m_wndScrollImageUpOverlayName;
        /* 0x0054 */ CStr m_wndScrollImageDownOverlayName;
        /* 0x0060 */ CStr m_wndScrollTextName;
        AuxInfo(const CinemaPanel::AuxInfo&);
        AuxInfo();
    }; /* size: 0x006c */
    
    struct MessageTimeInfo
    {
        /* 0x0000 */ int m_msgId;
        /* 0x0004 */ float m_delay;
        MessageTimeInfo(int msgId, float delay);
    }; /* size: 0x0008 */

protected:
    /* 0x0220 */ std::deque<CinemaPanel::MessageTimeInfo, std::allocator<CinemaPanel::MessageTimeInfo> > m_msgInfos;

    enum PanelType
    {
        PANELTYPE_NORMAL = 0,
        PANELTYPE_SCROLL = 1,
        PANELTYPE_NUM_PANEL_TYPES = 2,
    };

protected:
    virtual int OnPaint(const m3d::ui::DrawInfo& di) override /* 0x88 */;
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
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
    /* 0x0234 */ CinemaPanel::AuxInfo m_aif;
    /* 0x02a0 */ int m_soundTableId;
    /* 0x02a4 */ int m_soundChannelId;
    /* 0x02a8 */ int m_minTimeToExists;
    /* 0x02ac */ unsigned int m_curMessageStartTime;
    /* 0x02b0 */ unsigned int m_curMessageEndTime;
    /* 0x02b4 */ bool m_bIsShowingMessage;
    /* 0x02b5 */ bool m_bSkipMessage;
    /* 0x02b6 */ char Padding_196[2];
    /* 0x02b8 */ m3d::ui::Wnd* m_wndText;
    /* 0x02bc */ ItemModelWnd* m_wndsPortraits[2];
    /* 0x02c4 */ CinemaPanel::PanelType m_panelType;
    /* 0x02c8 */ m3d::ui::ImageWnd* m_wndPortraitOverlay;
    /* 0x02cc */ m3d::ui::Wnd* m_wndUpPanel;
    /* 0x02d0 */ m3d::ui::Wnd* m_wndDownPanel;
    /* 0x02d4 */ m3d::ui::ImageWnd* m_wndScrollImage;
    /* 0x02d8 */ m3d::ui::ImageWnd* m_wndScrollImageUpOverlay;
    /* 0x02dc */ m3d::ui::ImageWnd* m_wndScrollImageDownOverlay;
    /* 0x02e0 */ AutoScrollTextWnd* m_wndScrollText;
    CinemaPanel();
    CinemaPanel(const CinemaPanel& rhs);

public:
    virtual  ~CinemaPanel() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classCinemaPanel;
}; /* size: 0x02e4 */
