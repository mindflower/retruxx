#include "globalmapwnd.h"

RT_CLASS_EXPORTS_BEGIN(LevelConnectionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LevelConnectionsWnd);

LevelConnectionsWnd::ConnectionDrawInfo::ConnectionDrawInfo(ConnectionDrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelConnectionsWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LevelConnectionsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* LevelConnectionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* LevelConnectionsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(LevelConnectionsWnd);
}

m3d::Object* LevelConnectionsWnd::CreateObject()
{
    return new LevelConnectionsWnd;
}

void LevelConnectionsWnd::SetDrawInfo(std::vector<ConnectionDrawInfo, std::allocator<ConnectionDrawInfo>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelConnectionsWnd::~LevelConnectionsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelConnectionsWnd::DrawConnections(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelConnectionsWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelConnectionsWnd::LevelConnectionsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelConnectionsWnd::LevelConnectionsWnd(LevelConnectionsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(GlobalMapWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GlobalMapWnd);

GlobalMapWnd::AuxInfo::AuxInfo()
{
    m_levelIcoName = "_ico";
    m_wndChartName = "wndChart";
}

m3d::Object* GlobalMapWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* GlobalMapWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Object* GlobalMapWnd::CreateObject()
{
    return new GlobalMapWnd;
}

GlobalMapWnd::~GlobalMapWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* GlobalMapWnd::GetClass() const
{
    return RT_CLASS_LOCAL(GlobalMapWnd);
}

void GlobalMapWnd::UpdateConnectionsDrawInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

GlobalMapWnd::GlobalMapWnd()
{
    m_wndConnections = 0;
}

GlobalMapWnd::GlobalMapWnd(GlobalMapWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GlobalMapWnd::CreateConnectionsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool GlobalMapWnd::ConnectionExists(CStr const&, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GlobalMapWnd::GameDataSetup()
{
    // TODO: implement GlobalMapWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int GlobalMapWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GlobalMapWnd::ClearConnections()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GlobalMapWnd::InitLevelIcons()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GlobalMapWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GlobalMapWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GlobalMapWnd::UpdateConnections()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GlobalMapWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GlobalMapWnd::LaunchLocalMap()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GlobalMapWnd::OnStartLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GlobalMapWnd::OnLocationStateChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(LevelIco)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LevelIco);

m3d::Class* LevelIco::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelIco::~LevelIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelIco::SetUpForLevel(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* LevelIco::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

int LevelIco::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& LevelIco::GetLevelName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LevelIco::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* LevelIco::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelIco::OnStartLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelIco::LevelIco(LevelIco const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

LevelIco::LevelIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelIco::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelIco::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelIco::SetState(State)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void LevelIco::UpdateState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int LevelIco::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool LevelIco::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
