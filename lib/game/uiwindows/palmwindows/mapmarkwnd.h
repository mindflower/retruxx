#pragma once
#include <game/uimisc/navpoint.h>
#include <ui/image.h>
#include <ui/ui.h>

class MapMarkWnd : public m3d::ui::Wnd
{
public:
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Object* Clone();
    virtual ~MapMarkWnd();
    virtual m3d::Class* GetClass() const;
    class ObjectInfo* GetObjectInfo() const;
protected:
    virtual void FullUpdate();
    MapMarkWnd();
    MapMarkWnd(MapMarkWnd const&);
    int SetUpForObjectInfo(ObjectInfo*);

public:
    RT_CLASS_DECLARE(MapMarkWnd);

private:
    ObjectInfo* m_objectInfo;
};

class MapMarkIcoWnd :  public MapMarkWnd
{
public:
    int SetUp(ObjectInfo *, PointBase<float> const &);
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    void Select(bool);
    virtual ~MapMarkIcoWnd();
    virtual m3d::Object * Clone();
    bool IsSelected() const ;
protected:
    MapMarkIcoWnd();
    MapMarkIcoWnd(MapMarkIcoWnd const &);
    virtual void FullUpdate();
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    void UpdateToleranceColor();
    void UpdateIcon();
    void UpdateTooltip();

public:
    RT_CLASS_DECLARE(MapMarkIcoWnd);

private:
    bool m_bSelected;
    m3d::rend::TexHandle m_texSelection;
};

class PlayerMarkWnd :  public m3d::ui::ImageWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        PointBase<float> m_defaultSize;
        CStr m_texName;
    };

public:
    int CreatePlayerMarkWnd();
    static m3d::Class * GetBaseClass();
    void SetImageCoords(PointBase<float> const &);
    virtual m3d::Object * Clone();
    void SetImageAngle(float);
    virtual ~PlayerMarkWnd();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    void SetImageSize(PointBase<float> const &);
protected:
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    PlayerMarkWnd();
    PlayerMarkWnd(PlayerMarkWnd const &);

public:
    RT_CLASS_DECLARE(PlayerMarkWnd);

private:
    PointBase<float> m_wndCoords;
    float m_angle;
    PointBase<float> m_size;
    PlayerMarkWnd::AuxInfo m_aif;
};

class NavPointMarkWnd :  public m3d::ui::ImageWnd
{
public:
    virtual int CreateNavPointMarkWnd(PointBase<float> const &,int);
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    int GetNavPointId() const ;
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual ~NavPointMarkWnd();

protected:
    m3d::rend::TexHandle GetNpIcoByType(NavPoint::NavPointType) const ;
    CStr GetTooltipForNp(int) const ;
    NavPointMarkWnd();
    NavPointMarkWnd(NavPointMarkWnd const &);

public:
    RT_CLASS_DECLARE(NavPointMarkWnd);

private:
    int m_navPointId;
};

class MapMarkNameWnd :  public MapMarkWnd
{
public:
    virtual ~MapMarkNameWnd();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    int SetUp(ObjectInfo *, BoundsBase<float> const &, BoundsBase<float> const &);
protected:
    MapMarkNameWnd(MapMarkNameWnd const &);
    MapMarkNameWnd();

public:
    RT_CLASS_DECLARE(MapMarkNameWnd);
};
