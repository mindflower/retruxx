#include "videownd.h"

RT_CLASS_EXPORTS_BEGIN(VideoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VideoWnd);

VideoWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* VideoWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::ShowModelByPrototypeId(int, ref_ptr<m3d::ui::Wnd>)
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::GetPrototypeId() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* VideoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Object* VideoWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::HideModel(ref_ptr<m3d::ui::Wnd>)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* VideoWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::GetObjId() const
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::ShowModelByObjId(int, ref_ptr<m3d::ui::Wnd>)
{
    throw std::logic_error("Not implemented");
}

ref_ptr<m3d::ui::Wnd> VideoWnd::GetSrcWindow() const
{
    throw std::logic_error("Not implemented");
}

VideoWnd::~VideoWnd()
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

VideoWnd::VideoWnd()
{
    throw std::logic_error("Not implemented");
}

VideoWnd::VideoWnd(VideoWnd const&)
{
    throw std::logic_error("Not implemented");
}

bool VideoWnd::IsDisabled() const
{
    throw std::logic_error("Not implemented");
}

void VideoWnd::ShowBgModel(BgMode)
{
    throw std::logic_error("Not implemented");
}

void VideoWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void VideoWnd::UpdateBgMode()
{
    throw std::logic_error("Not implemented");
}

void VideoWnd::UpdateBgModel(BgMode)
{
    throw std::logic_error("Not implemented");
}

int VideoWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

VideoWnd::BgMode VideoWnd::DetectBgMode() const
{
    throw std::logic_error("Not implemented");
}
