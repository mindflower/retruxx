#include "loadwnd.h"
#include <core/log.h>

RT_CLASS_DEFINE(LoadWnd);

LoadWnd::LAuxInfo::LAuxInfo()
{
}

m3d::Object* LoadWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LoadWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(LSWnd);
}

m3d::Class* LoadWnd::GetClass() const
{
    return RT_CLASS_LOCAL(LoadWnd);
}

m3d::Object* LoadWnd::CreateObject()
{
    return new LoadWnd;
}

LoadWnd::~LoadWnd()
{
    throw std::logic_error("Not implemented");
}

int LoadWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int LoadWnd::GameDataSetup()
{
    using namespace m3d::ui;
    if (!LSWnd::GameDataSetup())
    {
        return 0;
    }
    if ((m_gameDataFlags & 2) == 0)
    {
        auto btnLoadName = dynamic_cast<ButtonWnd*>(GetChildByName(m_laif.m_btnLoadName));
        if (btnLoadName && btnLoadName->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnLoad = btnLoadName;
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_laif.m_btnLoadName + " is not found or incorrect type");
            m_gameDataFlags &= 0xFFFFFFFE;
        }
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("LoadWnd: error - fail to init because of a bad resource");
    return 0;
}

void LoadWnd::OnEnter()
{
    throw std::logic_error("Not implemented");
}

LoadWnd::LoadWnd(LoadWnd const&)
{
    throw std::logic_error("Not implemented");
}

LoadWnd::LoadWnd()
{
}

void LoadWnd::OnSaveSelectionChange()
{
    throw std::logic_error("Not implemented");
}

int LoadWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void LoadWnd::OnLoad()
{
    throw std::logic_error("Not implemented");
}
