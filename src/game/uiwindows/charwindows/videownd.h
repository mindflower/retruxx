#pragma once
#include "childpanel.h"
#include <core/ref_ptr.h>

class ContextModelWnd;

class VideoWnd :  public ChildPanel
{
public:
    enum BgMode
    {
        BGMODE_BUYVEHICLE = 0x0,
        BGMODE_WORKSHOP = 0x1,
        BGMODE_SHOP = 0x2,
        BGMODE_GROUND = 0x3,
        BGMODE_NUM_BGMODES = 0x4,
        BGMODE_INVALID = 0x4,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndModelName;
    };

public:
    static m3d::Object * CreateObject();
    int ShowModelByPrototypeId(int, ref_ptr<m3d::ui::Wnd>);
    int GetPrototypeId() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    int HideModel(ref_ptr<m3d::ui::Wnd>);
    virtual m3d::Class * GetClass() const ;
    int GetObjId() const ;
    int ShowModelByObjId(int, ref_ptr<m3d::ui::Wnd>);
    ref_ptr<m3d::ui::Wnd> GetSrcWindow() const ;
    virtual ~VideoWnd();
protected:
    virtual int GameDataSetup();
    VideoWnd();
    VideoWnd(VideoWnd const &);
    bool IsDisabled() const ;
    void ShowBgModel(BgMode);
    void OnNewFrame();
    virtual int GameDataClear(bool);
    void UpdateBgMode();
    void UpdateBgModel(BgMode);
    virtual int GameDataUpdate(void *,int);
    BgMode DetectBgMode() const ;

public:
    RT_CLASS_DECLARE(VideoWnd);

private:
    ContextModelWnd *m_wndModel;
    VideoWnd::AuxInfo m_aif;
    ref_ptr<m3d::ui::Wnd> m_srcWindow;
    VideoWnd::BgMode m_bgMode;
};
