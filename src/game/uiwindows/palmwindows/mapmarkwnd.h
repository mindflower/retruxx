#pragma once

class MapMarkIcoWnd :  public MapMarkWnd
{
public:
    int SetUp(class ObjectInfo *,class PointBase<float> const &);
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    void Select(bool);
    virtual ~MapMarkIcoWnd();
    virtual class m3d::Object * Clone();
    bool IsSelected() const ;
protected:
    MapMarkIcoWnd();
    MapMarkIcoWnd(class MapMarkIcoWnd const &);
    virtual void FullUpdate();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    void UpdateToleranceColor();
    void UpdateIcon();
    void UpdateTooltip();
private:
    bool m_bSelected;
    m3d::rend::TexHandle m_texSelection;
};

class PlayerMarkWnd :  public m3d::ui::ImageWnd
{
public:
    int CreatePlayerMarkWnd();
    static struct m3d::Class * __fastcall GetBaseClass();
    void SetImageCoords(class PointBase<float> const &);
    virtual class m3d::Object * Clone();
    void SetImageAngle(float);
    virtual ~PlayerMarkWnd();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    void SetImageSize(class PointBase<float> const &);
protected:
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    PlayerMarkWnd();
    PlayerMarkWnd(class PlayerMarkWnd const &);
private:
    PointBase<float> m_wndCoords;
    float m_angle;
    PointBase<float> m_size;
    PlayerMarkWnd::AuxInfo m_aif;
};

class NavPointMarkWnd :  public m3d::ui::ImageWnd
{
public:
    virtual int CreateNavPointMarkWnd(class PointBase<float> const &,int);
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    int GetNavPointId() const ;
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~NavPointMarkWnd();
protected:
    class m3d::rend::TexHandle GetNpIcoByType(enum NavPoint::NavPointType) const ;
    CStr GetTooltipForNp(int) const ;
    NavPointMarkWnd();
    NavPointMarkWnd(class NavPointMarkWnd const &);
private:
    int m_navPointId;
};

class MapMarkNameWnd :  public MapMarkWnd
{
public:
    virtual ~MapMarkNameWnd();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    int SetUp(class ObjectInfo *,struct BoundsBase<float> const &,struct BoundsBase<float> const &);
protected:
    MapMarkNameWnd(class MapMarkNameWnd const &);
    MapMarkNameWnd();
private:
};

class MapMarkWnd :  public m3d::ui::Wnd
{
public:
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    virtual ~MapMarkWnd();
    virtual struct m3d::Class * GetClass() const ;
    class ObjectInfo * GetObjectInfo() const ;
protected:
    virtual void FullUpdate();
    MapMarkWnd();
    MapMarkWnd(class MapMarkWnd const &);
    int SetUpForObjectInfo(class ObjectInfo *);
private:
    ObjectInfo *m_objectInfo;
};
