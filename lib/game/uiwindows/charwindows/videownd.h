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
        CStr m_wndModelName;
        AuxInfo(VideoWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    bool IsDisabled() const;
    void OnNewFrame();
    void UpdateBgMode();
    void ShowBgModel(VideoWnd::BgMode bgMode);
    void UpdateBgModel(VideoWnd::BgMode bgMode);
    VideoWnd::BgMode DetectBgMode() const;

    ContextModelWnd* m_wndModel;
    VideoWnd::AuxInfo m_aif;
    ref_ptr<m3d::ui::Wnd> m_srcWindow;
    VideoWnd::BgMode m_bgMode;

    VideoWnd();
    VideoWnd(VideoWnd const& rhs);

public:
    virtual ~VideoWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classVideoWnd;
};
