#include "izvratrepositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(IzvratRepositoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(IzvratRepositoryWnd);

IzvratRepositoryWnd::FrameSegment::FrameSegment(FrameSegment const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* IzvratRepositoryWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* IzvratRepositoryWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> IzvratRepositoryWnd::GetMaxRepositoryBounds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IzvratRepositoryWnd::SetIzvratRepository(ai::IzvratRepository*, RepositoryType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* IzvratRepositoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(RepositoryWnd);
}

ai::IzvratRepository* IzvratRepositoryWnd::GetIzvratRepository() const
{
    return RT_DYNCAST(GetRepository(), ai::IzvratRepository);
}

IzvratRepositoryWnd::~IzvratRepositoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* IzvratRepositoryWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IzvratRepositoryWnd::CreateScrollWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IzvratRepositoryWnd::CalculateSegmentsBounds(std::map<int, int> const&, std::vector<FrameSegment>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IzvratRepositoryWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IzvratRepositoryWnd::CalculateFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IzvratRepositoryWnd::FindPerimeter(std::map<int, int>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IzvratRepositoryWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepository* IzvratRepositoryWnd::GetRepository() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int IzvratRepositoryWnd::SetRepository(ai::GeomRepository*, RepositoryWnd::RepositoryType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

IzvratRepositoryWnd::IzvratRepositoryWnd(IzvratRepositoryWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

IzvratRepositoryWnd::IzvratRepositoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void IzvratRepositoryWnd::DrawBunch(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
