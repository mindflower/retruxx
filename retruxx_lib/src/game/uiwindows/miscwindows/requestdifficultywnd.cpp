#include "requestdifficultywnd.h"

#include "core/log.h"
#include "ui/comboboxwnd.h"

RT_CLASS_EXPORTS_BEGIN(RequestDifficultyWnd)
RT_CLASS_EXPORTS_END;
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
    RETRUXX_NOT_IMPLEMENTED;
}

RequestDifficultyWnd::~RequestDifficultyWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

RequestDifficultyWnd::RequestDifficultyWnd()
{
}

void RequestDifficultyWnd::FillDifficultyLevelsList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RequestDifficultyWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RequestDifficultyWnd::OnBeforeRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RequestDifficultyWnd::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RequestDifficultyWnd::ApplyDifficultyLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RequestDifficultyWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
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
