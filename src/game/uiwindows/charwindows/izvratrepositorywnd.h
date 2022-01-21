#pragma once
#include "repositorywnd.h"
#include <server/izvratrepository.h>

class IzvratRepositoryWnd :  public RepositoryWnd
{
public:
    class FrameSegment
    {
    public:
        FrameSegment(FrameSegment const&);

    private:
        int m_frameTexId;
        BoundsBase<float> m_rect;
    };

public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    BoundsBase<float> GetMaxRepositoryBounds() const ;
    virtual int SetIzvratRepository(ai::IzvratRepository *,RepositoryType);
    static m3d::Class * GetBaseClass();
    ai::IzvratRepository * GetIzvratRepository() const ;
    virtual ~IzvratRepositoryWnd();
    virtual m3d::Object * Clone();

protected:
    virtual int CreateScrollWnd();
    void CalculateSegmentsBounds(std::map<int,int> const &,std::vector<FrameSegment> &);
    virtual void DrawGrid(m3d::ui::DrawInfo const &);
    void CalculateFrame();
    void FindPerimeter(std::map<int,int> &) const ;
    virtual void DrawFrame(m3d::ui::DrawInfo const &);
    virtual int SetRepository(ai::GeomRepository *,RepositoryType);
    IzvratRepositoryWnd(IzvratRepositoryWnd const &);
    IzvratRepositoryWnd();
    virtual void DrawBunch(m3d::ui::DrawInfo const &);

public:
    RT_CLASS_INLINE_DECLARE(IzvratRepositoryWnd);

private:
    std::vector<FrameSegment> m_frameSegments;
};
