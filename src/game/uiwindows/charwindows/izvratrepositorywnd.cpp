#include "izvratrepositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(IzvratRepositoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(IzvratRepositoryWnd);

IzvratRepositoryWnd::FrameSegment::FrameSegment(FrameSegment const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* IzvratRepositoryWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* IzvratRepositoryWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> IzvratRepositoryWnd::GetMaxRepositoryBounds() const
{
    throw std::logic_error("Not implemented");
}

int IzvratRepositoryWnd::SetIzvratRepository(ai::IzvratRepository*, RepositoryType)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* IzvratRepositoryWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

ai::IzvratRepository* IzvratRepositoryWnd::GetIzvratRepository() const
{
    throw std::logic_error("Not implemented");
}

IzvratRepositoryWnd::~IzvratRepositoryWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* IzvratRepositoryWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int IzvratRepositoryWnd::CreateScrollWnd()
{
    throw std::logic_error("Not implemented");
}

void IzvratRepositoryWnd::CalculateSegmentsBounds(std::map<int, int> const&, std::vector<FrameSegment>&)
{
    throw std::logic_error("Not implemented");
}

void IzvratRepositoryWnd::DrawGrid(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void IzvratRepositoryWnd::CalculateFrame()
{
    throw std::logic_error("Not implemented");
}

void IzvratRepositoryWnd::FindPerimeter(std::map<int, int>&) const
{
    throw std::logic_error("Not implemented");
}

void IzvratRepositoryWnd::DrawFrame(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int IzvratRepositoryWnd::SetRepository(ai::GeomRepository*, RepositoryType)
{
    throw std::logic_error("Not implemented");
}

IzvratRepositoryWnd::IzvratRepositoryWnd(IzvratRepositoryWnd const&)
{
    throw std::logic_error("Not implemented");
}

IzvratRepositoryWnd::IzvratRepositoryWnd()
{
    throw std::logic_error("Not implemented");
}

void IzvratRepositoryWnd::DrawBunch(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}
