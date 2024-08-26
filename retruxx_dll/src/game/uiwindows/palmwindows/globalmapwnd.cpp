#include "globalmapwnd.h"

RT_CLASS_EXPORTS_BEGIN(LevelConnectionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LevelConnectionsWnd);

LevelConnectionsWnd::ConnectionDrawInfo::ConnectionDrawInfo(ConnectionDrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

LevelConnectionsWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelConnectionsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LevelConnectionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* LevelConnectionsWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelConnectionsWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void LevelConnectionsWnd::SetDrawInfo(std::vector<ConnectionDrawInfo, std::allocator<ConnectionDrawInfo>> const&)
{
    throw std::logic_error("Not implemented");
}

LevelConnectionsWnd::~LevelConnectionsWnd()
{
    throw std::logic_error("Not implemented");
}

void LevelConnectionsWnd::DrawConnections(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int LevelConnectionsWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

LevelConnectionsWnd::LevelConnectionsWnd()
{
    throw std::logic_error("Not implemented");
}

LevelConnectionsWnd::LevelConnectionsWnd(LevelConnectionsWnd const&)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(GlobalMapWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GlobalMapWnd);

GlobalMapWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GlobalMapWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GlobalMapWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Object* GlobalMapWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

GlobalMapWnd::~GlobalMapWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GlobalMapWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void GlobalMapWnd::UpdateConnectionsDrawInfo()
{
    throw std::logic_error("Not implemented");
}

GlobalMapWnd::GlobalMapWnd()
{
    throw std::logic_error("Not implemented");
}

GlobalMapWnd::GlobalMapWnd(GlobalMapWnd const&)
{
    throw std::logic_error("Not implemented");
}

int GlobalMapWnd::CreateConnectionsWnd()
{
    throw std::logic_error("Not implemented");
}

bool GlobalMapWnd::ConnectionExists(CStr const&, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int GlobalMapWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int GlobalMapWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void GlobalMapWnd::ClearConnections()
{
    throw std::logic_error("Not implemented");
}

int GlobalMapWnd::InitLevelIcons()
{
    throw std::logic_error("Not implemented");
}

int GlobalMapWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int GlobalMapWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void GlobalMapWnd::UpdateConnections()
{
    throw std::logic_error("Not implemented");
}

int GlobalMapWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void GlobalMapWnd::LaunchLocalMap()
{
    throw std::logic_error("Not implemented");
}

void GlobalMapWnd::OnStartLevel()
{
    throw std::logic_error("Not implemented");
}

void GlobalMapWnd::OnLocationStateChanged()
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(LevelIco)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LevelIco);

m3d::Class* LevelIco::GetClass() const
{
    throw std::logic_error("Not implemented");
}

LevelIco::~LevelIco()
{
    throw std::logic_error("Not implemented");
}

int LevelIco::SetUpForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LevelIco::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

int LevelIco::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

CStr const& LevelIco::GetLevelName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelIco::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelIco::Clone()
{
    throw std::logic_error("Not implemented");
}

void LevelIco::OnStartLevel()
{
    throw std::logic_error("Not implemented");
}

LevelIco::LevelIco(LevelIco const&)
{
    throw std::logic_error("Not implemented");
}

LevelIco::LevelIco()
{
    throw std::logic_error("Not implemented");
}

int LevelIco::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int LevelIco::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void LevelIco::SetState(State)
{
    throw std::logic_error("Not implemented");
}

void LevelIco::UpdateState()
{
    throw std::logic_error("Not implemented");
}

int LevelIco::OnMouseButton0(unsigned, PointBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

bool LevelIco::IsValid() const
{
    throw std::logic_error("Not implemented");
}
