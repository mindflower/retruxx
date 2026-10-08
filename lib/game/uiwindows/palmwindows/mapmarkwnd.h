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
    virtual void FullUpdate() /* 0x11c */;
    /* 0x0220 */ ObjectInfo* m_objectInfo;

    MapMarkWnd();
    MapMarkWnd(MapMarkWnd const& rhs);

public:
    virtual ~MapMarkWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classMapMarkWnd;
}; /* size: 0x0224 */

class MapMarkIcoWnd : public MapMarkWnd
{
public:
    int SetUp(ObjectInfo* objectInfo, PointBase<float> const& objWndOrigin);
    void Select(bool bSelect);
    bool IsSelected() const;

    struct AuxInfo
    {
        /* 0x0000 */ PointBase<float> m_size;
        /* 0x0008 */ PointBase<float> m_sizeSelected;
        /* 0x0010 */ unsigned int m_colorEnemy;
        /* 0x0014 */ unsigned int m_colorNeutral;
        /* 0x0018 */ unsigned int m_colorFriend;
        /* 0x001c */ CStr m_texIdSelection;
        AuxInfo(MapMarkIcoWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0028 */

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x00 */;
    virtual void FullUpdate() override /* 0x11c */;
    void UpdateIcon();
    void UpdateTooltip();
    void UpdateToleranceColor();
    /* 0x0224 */ bool m_bSelected;
    /* 0x0225 */ char Padding_149[3];
    /* 0x0228 */ m3d::rend::TexHandle m_texSelection;

    static inline MapMarkIcoWnd::AuxInfo m_aif;

    MapMarkIcoWnd();
    MapMarkIcoWnd(MapMarkIcoWnd const& rhs);

public:
    virtual ~MapMarkIcoWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classMapMarkIcoWnd;
}; /* size: 0x022c */

class PlayerMarkWnd : public m3d::ui::ImageWnd
{
public:
    void SetImageCoords(PointBase<float> const& wndCoords);
    void SetImageAngle(float angle);
    void SetImageSize(PointBase<float> const& size);
    int CreatePlayerMarkWnd();

    struct AuxInfo
    {
        /* 0x0000 */ PointBase<float> m_defaultSize;
        /* 0x0008 */ CStr m_texName;
        AuxInfo(PlayerMarkWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0014 */

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    /* 0x0234 */ PointBase<float> m_wndCoords;
    /* 0x023c */ float m_angle;
    /* 0x0240 */ PointBase<float> m_size;
    /* 0x0248 */ PlayerMarkWnd::AuxInfo m_aif;
    PlayerMarkWnd();
    PlayerMarkWnd(PlayerMarkWnd const& rhs);

public:
    virtual ~PlayerMarkWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classPlayerMarkWnd;
}; /* size: 0x025c */

class NavPointMarkWnd : public m3d::ui::ImageWnd
{
public:
    virtual int CreateNavPointMarkWnd(PointBase<float> const& wndCoords, int navPointId) /* 0x124 */;
    int GetNavPointId() const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_locationTooltipStrName;
        /* 0x000c */ CStr m_texNames[3];
        /* 0x0030 */ PointBase<float> m_size;
        AuxInfo(NavPointMarkWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0038 */

protected:
    m3d::rend::TexHandle GetNpIcoByType(NavPoint::NavPointType npType) const;
    CStr GetTooltipForNp(int npId) const;
    static inline NavPointMarkWnd::AuxInfo m_aif;
    /* 0x0234 */ int m_navPointId;
    NavPointMarkWnd();
    NavPointMarkWnd(NavPointMarkWnd const& rhs);

public:
    virtual ~NavPointMarkWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classNavPointMarkWnd;
}; /* size: 0x0238 */

class MapMarkNameWnd : public MapMarkWnd
{
public:
    int SetUp(ObjectInfo* objectInfo, BoundsBase<float> const& mapMarkIcoB, BoundsBase<float> const& clipB);

    struct NameAuxInfo
    {
        /* 0x0000 */ CStr m_paneName;
        /* 0x000c */ PointBase<float> m_space;
        /* 0x0014 */ unsigned int m_textColor;
        NameAuxInfo(MapMarkNameWnd::NameAuxInfo const&);
        NameAuxInfo();
    }; /* size: 0x0018 */

protected:
    static inline MapMarkNameWnd::NameAuxInfo m_nAif;
    MapMarkNameWnd();
    MapMarkNameWnd(MapMarkNameWnd const& rhs);

public:
    virtual ~MapMarkNameWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classMapMarkNameWnd;
}; /* size: 0x0224 */
