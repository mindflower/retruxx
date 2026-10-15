#pragma once
#include "repositorywnd.h"
#include <server/izvratrepository.h>

class IzvratRepositoryWnd : public RepositoryWnd
{
public:
    ai::IzvratRepository* GetIzvratRepository() const;
    virtual int SetIzvratRepository(ai::IzvratRepository* repository, RepositoryWnd::RepositoryType type);
    BoundsBase<float> GetMaxRepositoryBounds() const;

    struct IzvratRepositoryWnd::FrameSegment
    {
        int m_frameTexId;
        BoundsBase<float> m_rect;
        FrameSegment(IzvratRepositoryWnd::FrameSegment const& __that);
        FrameSegment(int frameTexId, BoundsBase<float> const& rect);
    };

    using FrameSegmentVector =
        std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>>;
    using PerimeterCellsMap = std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>;
    using PerimeterCellsPair = std::pair<int, int>;

protected:
    ai::GeomRepository* GetRepository() const;
    virtual int SetRepository(ai::GeomRepository* repository, RepositoryWnd::RepositoryType type) override;
    virtual void DrawBunch(m3d::ui::DrawInfo const& di) override;
    virtual void DrawGrid(m3d::ui::DrawInfo const& di) override;
    virtual void DrawFrame(m3d::ui::DrawInfo const& di) override;
    virtual int CreateScrollWnd() override;
    void CalculateFrame();
    void FindPerimeter(
        std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>& perimeterCells) const;
    void CalculateSegmentsBounds(
        std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>> const& perimeterCells,
        std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>>&
            frameSegments);
    std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>> m_frameSegments;
    IzvratRepositoryWnd();
    IzvratRepositoryWnd(IzvratRepositoryWnd const& rhs);

public:
    virtual ~IzvratRepositoryWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classIzvratRepositoryWnd;
};
