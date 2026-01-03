#pragma once
#include "repositorywnd.h"
#include <server/izvratrepository.h>

class IzvratRepositoryWnd : public RepositoryWnd
{
public:
    ai::IzvratRepository* GetIzvratRepository() const;
    virtual int SetIzvratRepository(ai::IzvratRepository* repository, RepositoryWnd::RepositoryType type) /* 0x1bc */;
    BoundsBase<float> GetMaxRepositoryBounds() const;

    struct IzvratRepositoryWnd::FrameSegment
    {
        /* 0x0000 */ int m_frameTexId;
        /* 0x0004 */ BoundsBase<float> m_rect;
        FrameSegment(IzvratRepositoryWnd::FrameSegment const& __that);
        FrameSegment(int frameTexId, BoundsBase<float> const& rect);
    }; /* size: 0x0014 */

    using FrameSegmentVector =
        std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>>;
    using PerimeterCellsMap = std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>;
    using PerimeterCellsPair = std::pair<int, int>;

protected:
    ai::GeomRepository* GetRepository() const;
    virtual int SetRepository(ai::GeomRepository* repository, RepositoryWnd::RepositoryType type) override /* 0x174 */;
    virtual void DrawBunch(m3d::ui::DrawInfo const& di) override /* 0x184 */;
    virtual void DrawGrid(m3d::ui::DrawInfo const& di) override /* 0x188 */;
    virtual void DrawFrame(m3d::ui::DrawInfo const& di) override /* 0x18c */;
    virtual int CreateScrollWnd() override /* 0x178 */;
    void CalculateFrame();
    void FindPerimeter(
        std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>& perimeterCells) const;
    void CalculateSegmentsBounds(
        std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>> const& perimeterCells,
        std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>>&
            frameSegments);
    /* 0x0324 */ std::vector<IzvratRepositoryWnd::FrameSegment, std::allocator<IzvratRepositoryWnd::FrameSegment>>
        m_frameSegments;
    IzvratRepositoryWnd();
    IzvratRepositoryWnd(IzvratRepositoryWnd const& rhs);

public:
    virtual ~IzvratRepositoryWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classIzvratRepositoryWnd;
}; /* size: 0x0334 */
