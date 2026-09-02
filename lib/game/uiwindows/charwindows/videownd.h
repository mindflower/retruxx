#pragma once
#include "childpanel.h"
#include <core/ref_ptr.h>

class ContextModelWnd;

class VideoWnd : public ChildPanel
{
public:
    int ShowModelByObjId(int objId, ref_ptr<m3d::ui::Wnd> srcWindow);
    int ShowModelByPrototypeId(int prototyprId, ref_ptr<m3d::ui::Wnd> srcWindow);
    int HideModel(ref_ptr<m3d::ui::Wnd> srcWindow);
    ref_ptr<m3d::ui::Wnd> GetSrcWindow() const;
    int GetObjId() const;
    int GetPrototypeId() const;

    enum BgMode
    {
        BGMODE_BUYVEHICLE = 0,
        BGMODE_WORKSHOP = 1,
        BGMODE_SHOP = 2,
        BGMODE_GROUND = 3,
        BGMODE_NUM_BGMODES = 4,
        BGMODE_INVALID = 4,
    };

    struct VideoWnd::AuxInfo
    {
        /* 0x0000 */ CStr m_wndModelName;
        AuxInfo(VideoWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    bool IsDisabled() const;
    void OnNewFrame();
    void UpdateBgMode();
    void ShowBgModel(VideoWnd::BgMode bgMode);
    void UpdateBgModel(VideoWnd::BgMode bgMode);
    VideoWnd::BgMode DetectBgMode() const;

    /* 0x0224 */ ContextModelWnd* m_wndModel;
    /* 0x0228 */ VideoWnd::AuxInfo m_aif;
    /* 0x0234 */ ref_ptr<m3d::ui::Wnd> m_srcWindow;
    /* 0x0238 */ VideoWnd::BgMode m_bgMode;

    VideoWnd();
    VideoWnd(VideoWnd const& rhs);

public:
    virtual ~VideoWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classVideoWnd;
}; /* size: 0x023c */
