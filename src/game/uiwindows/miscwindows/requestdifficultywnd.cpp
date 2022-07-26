#include "requestdifficultywnd.h"

#include "core/log.h"
#include "ui/comboboxwnd.h"

RT_CLASS_DEFINE(RequestDifficultyWnd);

RequestDifficultyWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* RequestDifficultyWnd::GetClass() const
{
    return RT_CLASS_LOCAL(RequestDifficultyWnd);
}

m3d::Object* RequestDifficultyWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

RequestDifficultyWnd::~RequestDifficultyWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RequestDifficultyWnd::CreateObject()
{
    return new RequestDifficultyWnd;
}

m3d::Class* RequestDifficultyWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

RequestDifficultyWnd::RequestDifficultyWnd(RequestDifficultyWnd const&)
{
    throw std::logic_error("Not implemented");
}

RequestDifficultyWnd::RequestDifficultyWnd()
{
}

void RequestDifficultyWnd::FillDifficultyLevelsList()
{
    throw std::logic_error("Not implemented");
}

int RequestDifficultyWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

int RequestDifficultyWnd::OnBeforeRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void RequestDifficultyWnd::Clear()
{
    throw std::logic_error("Not implemented");
}

void RequestDifficultyWnd::ApplyDifficultyLevel()
{
    throw std::logic_error("Not implemented");
}

int RequestDifficultyWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int RequestDifficultyWnd::GameDataSetup()
{
    using namespace m3d::ui;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto diff = GetChildByName(m_aif.m_cbDifficultyLevelsName);
        if (diff && diff->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            this->m_gameDataFlags |= 1u;
            m_cbDifficultyLevels = dynamic_cast<ComboBoxWnd*>(diff);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbDifficultyLevelsName + " is not found or incorrect type");
        }
    }
    if ((m_gameDataFlags & 1) != 0)
        return 1;

    M3D_LOG_INFO("RequestDifficultyWnd: error - fail to init because of a bad resource");
    return 0;
}
