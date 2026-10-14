#pragma once
#include <game/uimisc/navpoint.h>
#include <ui/image.h>
#include <ui/ui.h>

class ObjectInfo;

class MapMarkWnd : public m3d::ui::Wnd
{
public:
    ObjectInfo* GetObjectInfo() const;

protected:
    int SetUpForObjectInfo(ObjectInfo* objectInfo);
    virtual void FullUpdate();
    ObjectInfo* m_objectInfo;

    MapMarkWnd();
    MapMarkWnd(MapMarkWnd const& rhs);

public:
    virtual ~MapMarkWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMapMarkWnd;
};

class MapMarkIcoWnd : public MapMarkWnd
{
public:
    int SetUp(ObjectInfo* objectInfo, PointBase<float> const& objWndOrigin);
    void Select(bool bSelect);
    bool IsSelected() const;

    struct AuxInfo
    {
        PointBase<float> m_size;
        PointBase<float> m_sizeSelected;
        unsigned int m_colorEnemy;
        unsigned int m_colorNeutral;
        unsigned int m_colorFriend;
        CStr m_texIdSelection;
        AuxInfo(MapMarkIcoWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    virtual void FullUpdate() override;
    void UpdateIcon();
    void UpdateTooltip();
    void UpdateToleranceColor();
    bool m_bSelected;
    m3d::rend::TexHandle m_texSelection;

    static inline MapMarkIcoWnd::AuxInfo m_aif;

    MapMarkIcoWnd();
    MapMarkIcoWnd(MapMarkIcoWnd const& rhs);

public:
    virtual ~MapMarkIcoWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMapMarkIcoWnd;
};

class PlayerMarkWnd : public m3d::ui::ImageWnd
{
public:
    void SetImageCoords(PointBase<float> const& wndCoords);
    void SetImageAngle(float angle);
    void SetImageSize(PointBase<float> const& size);
    int CreatePlayerMarkWnd();

    struct AuxInfo
    {
        PointBase<float> m_defaultSize;
        CStr m_texName;
        AuxInfo(PlayerMarkWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    PointBase<float> m_wndCoords;
    float m_angle;
    PointBase<float> m_size;
    PlayerMarkWnd::AuxInfo m_aif;
    PlayerMarkWnd();
    PlayerMarkWnd(PlayerMarkWnd const& rhs);

public:
    virtual ~PlayerMarkWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classPlayerMarkWnd;
};

class NavPointMarkWnd : public m3d::ui::ImageWnd
{
public:
    virtual int CreateNavPointMarkWnd(PointBase<float> const& wndCoords, int navPointId);
    int GetNavPointId() const;

    struct AuxInfo
    {
        CStr m_locationTooltipStrName;
        CStr m_texNames[3];
        PointBase<float> m_size;
        AuxInfo(NavPointMarkWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    m3d::rend::TexHandle GetNpIcoByType(NavPoint::NavPointType npType) const;
    CStr GetTooltipForNp(int npId) const;
    static inline NavPointMarkWnd::AuxInfo m_aif;
    int m_navPointId;
    NavPointMarkWnd();
    NavPointMarkWnd(NavPointMarkWnd const& rhs);

public:
    virtual ~NavPointMarkWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classNavPointMarkWnd;
};

class MapMarkNameWnd : public MapMarkWnd
{
public:
    int SetUp(ObjectInfo* objectInfo, BoundsBase<float> const& mapMarkIcoB, BoundsBase<float> const& clipB);

    struct NameAuxInfo
    {
        CStr m_paneName;
        PointBase<float> m_space;
        unsigned int m_textColor;
        NameAuxInfo(MapMarkNameWnd::NameAuxInfo const&);
        NameAuxInfo();
    };

protected:
    static inline MapMarkNameWnd::NameAuxInfo m_nAif;
    MapMarkNameWnd();
    MapMarkNameWnd(MapMarkNameWnd const& rhs);

public:
    virtual ~MapMarkNameWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMapMarkNameWnd;
};
