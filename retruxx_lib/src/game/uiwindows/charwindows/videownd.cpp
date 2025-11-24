#include "videownd.h"

RT_CLASS_EXPORTS_BEGIN(VideoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VideoWnd);

VideoWnd::AuxInfo::AuxInfo()
{
    m_wndModelName = "wndModel";
}

m3d::Object* VideoWnd::CreateObject()
{
    return new VideoWnd;
}

int VideoWnd::ShowModelByPrototypeId(int, ref_ptr<m3d::ui::Wnd>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VideoWnd::GetPrototypeId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* VideoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Object* VideoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VideoWnd::HideModel(ref_ptr<m3d::ui::Wnd>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* VideoWnd::GetClass() const
{
    return RT_CLASS_LOCAL(VideoWnd);
}

int VideoWnd::GetObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VideoWnd::ShowModelByObjId(int, ref_ptr<m3d::ui::Wnd>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ref_ptr<m3d::ui::Wnd> VideoWnd::GetSrcWindow() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

VideoWnd::~VideoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VideoWnd::GameDataSetup()
{
    // TODO: implement VideoWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

VideoWnd::VideoWnd()
{
    m_wndModel = 0;
    m_bgMode = BGMODE_NUM_BGMODES;
}

VideoWnd::VideoWnd(VideoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool VideoWnd::IsDisabled() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VideoWnd::ShowBgModel(BgMode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VideoWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VideoWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VideoWnd::UpdateBgMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void VideoWnd::UpdateBgModel(BgMode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int VideoWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

VideoWnd::BgMode VideoWnd::DetectBgMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
