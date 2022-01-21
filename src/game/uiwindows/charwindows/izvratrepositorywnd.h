#pragma once

class IzvratRepositoryWnd :  public RepositoryWnd
{
public:
    class FrameSegment
    {
    public:
        FrameSegment(struct FrameSegment const&);
    protected:
    private:
        int m_frameTexId;
        BoundsBase<float> m_rect;
    };

public:
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    struct BoundsBase<float> GetMaxRepositoryBounds() const ;
    virtual int SetIzvratRepository(class ai::IzvratRepository *,enum RepositoryWnd::RepositoryType);
    static struct m3d::Class * GetBaseClass();
    class ai::IzvratRepository * GetIzvratRepository() const ;
    virtual ~IzvratRepositoryWnd();
    virtual class m3d::Object * Clone();
protected:
    virtual int CreateScrollWnd();
    void CalculateSegmentsBounds(class std::map<int,int,struct std::less<int>,class std::allocator<struct std::pair<int const ,int> > > const &,class std::vector<struct FrameSegment,class std::allocator<struct FrameSegment> > &);
    virtual void DrawGrid(struct m3d::ui::DrawInfo const &);
    void CalculateFrame();
    void FindPerimeter(class std::map<int,int,struct std::less<int>,class std::allocator<struct std::pair<int const ,int> > > &) const ;
    virtual void DrawFrame(struct m3d::ui::DrawInfo const &);
    virtual int SetRepository(class ai::GeomRepository *,enum RepositoryWnd::RepositoryType);
    IzvratRepositoryWnd(class IzvratRepositoryWnd const &);
    IzvratRepositoryWnd();
    virtual void DrawBunch(struct m3d::ui::DrawInfo const &);
private:
    std::vector<IzvratRepositoryWnd::FrameSegment> m_frameSegments;
};
