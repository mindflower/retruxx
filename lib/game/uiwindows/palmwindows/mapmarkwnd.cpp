#include "mapmarkwnd.h"

RT_CLASS_EXPORTS_BEGIN(MapMarkWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapMarkWnd);

m3d::Object* MapMarkWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapMarkWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* MapMarkWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkWnd::~MapMarkWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapMarkWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo* MapMarkWnd::GetObjectInfo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapMarkWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkWnd::MapMarkWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkWnd::MapMarkWnd(MapMarkWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapMarkWnd::SetUpForObjectInfo(ObjectInfo*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(MapMarkIcoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapMarkIcoWnd);

int MapMarkIcoWnd::SetUp(ObjectInfo*, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapMarkIcoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(MapMarkWnd);
}

m3d::Object* MapMarkIcoWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapMarkIcoWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapMarkIcoWnd::Select(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkIcoWnd::~MapMarkIcoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapMarkIcoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MapMarkIcoWnd::IsSelected() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkIcoWnd::MapMarkIcoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkIcoWnd::MapMarkIcoWnd(MapMarkIcoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapMarkIcoWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapMarkIcoWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapMarkIcoWnd::UpdateToleranceColor()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapMarkIcoWnd::UpdateIcon()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MapMarkIcoWnd::UpdateTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(PlayerMarkWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(PlayerMarkWnd);

PlayerMarkWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerMarkWnd::CreatePlayerMarkWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* PlayerMarkWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

void PlayerMarkWnd::SetImageCoords(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* PlayerMarkWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerMarkWnd::SetImageAngle(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

PlayerMarkWnd::~PlayerMarkWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* PlayerMarkWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* PlayerMarkWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerMarkWnd::SetImageSize(PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerMarkWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

PlayerMarkWnd::PlayerMarkWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PlayerMarkWnd::PlayerMarkWnd(PlayerMarkWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(NavPointMarkWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NavPointMarkWnd);

int NavPointMarkWnd::CreateNavPointMarkWnd(PointBase<float> const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NavPointMarkWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NavPointMarkWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointMarkWnd::GetNavPointId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NavPointMarkWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NavPointMarkWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

NavPointMarkWnd::~NavPointMarkWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle NavPointMarkWnd::GetNpIcoByType(NavPoint::NavPointType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr NavPointMarkWnd::GetTooltipForNp(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointMarkWnd::NavPointMarkWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointMarkWnd::NavPointMarkWnd(NavPointMarkWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(MapMarkNameWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapMarkNameWnd);

MapMarkNameWnd::~MapMarkNameWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapMarkNameWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapMarkNameWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(MapMarkWnd);
}

m3d::Object* MapMarkNameWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapMarkNameWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapMarkNameWnd::SetUp(ObjectInfo*, BoundsBase<float> const&, BoundsBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkNameWnd::MapMarkNameWnd(MapMarkNameWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapMarkNameWnd::MapMarkNameWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}
