#include "gameoptionswnd.h"

#include "m3dapp.h"
#include "core/log.h"
#include "server/objects/base/globalproperties.h"
#include "ui/button.h"
#include "ui/comboboxwnd.h"
#include "ui/slider.h"

RT_CLASS_DEFINE(GameOptionsWnd);

GameOptionsWnd::AuxInfo::AuxInfo()
{
}

GameOptionsWnd::~GameOptionsWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GameOptionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* GameOptionsWnd::CreateObject()
{
    return new GameOptionsWnd;
}

m3d::Object* GameOptionsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GameOptionsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(GameOptionsWnd);
}

void GameOptionsWnd::InitNumRepliesControls()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::ApplyNumReplies()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::UpdateControls()
{
    throw std::logic_error("Not implemented");
}

GameOptionsWnd::GameOptionsWnd()
{
}

GameOptionsWnd::GameOptionsWnd(GameOptionsWnd const&)
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::InitAutoHelpControls()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::OnSliderNumRepliesChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::OnCheckAutoHelpClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int GameOptionsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::UpdateNumRepliesControls()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::ApplyGameDifficulty()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::UpdateNumRepliesPrevNextButtonsState()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto notch = m_sliderNumReplies->GetNotch();
        m_btnNumRepliesPrev->EnableWindow(notch > m_sliderNumReplies->GetMin());
        m_btnNumRepliesNext->EnableWindow(notch < m_sliderNumReplies->GetMax());
    }
}

void GameOptionsWnd::OnBtnNumRepliesNextClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::OnCbGameDifficultyChange(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::OnBtnNumRepliesPrevClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::InitGameDifficultyControls()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < ai::theGlobProp.m_difficultyLevelCoeffs.size(); ++i)
        {
            auto name = m3d::Application::g_pApp->GetStringByStringId0(ai::theGlobProp.m_difficultyLevelCoeffs[i].m_name);
            auto idx = m_cbGameDifficulty->AddItem(name);
            if (idx != -1)
            {
                m_cbGameDifficulty->SetItemData(idx, i);
            }
        }
    }
}

int GameOptionsWnd::GameDataSetup()
{
    using namespace m3d::ui;
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto autoHelp = GetChildByName(m_aif.m_checkAutoHelpName);
        if (autoHelp && autoHelp->IsKindOf(RT_CLASS_LOCAL(CheckWnd)))
        {
            m_checkAutoHelp = dynamic_cast<CheckWnd*>(autoHelp);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_checkAutoHelpName + " is not found or incorrect type");
            res = 0;
        }

        auto replies = GetChildByName(m_aif.m_sliderNumRepliesName);
        if (replies && replies->IsKindOf(RT_CLASS_LOCAL(SliderWnd)))
        {
            m_sliderNumReplies = dynamic_cast<SliderWnd*>(replies);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderNumRepliesName + " is not found or incorrect type");
            res = 0;
        }

        auto repliesPrev = GetChildByName(m_aif.m_btnNumRepliesPrevName);
        if (repliesPrev && repliesPrev->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnNumRepliesPrev = dynamic_cast<ButtonWnd*>(repliesPrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnNumRepliesPrevName + " is not found or incorrect type");
            res = 0;
        }

        auto repliesNext = GetChildByName(m_aif.m_btnNumRepliesNextName);
        if (repliesNext && repliesNext->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnNumRepliesNext = dynamic_cast<ButtonWnd*>(repliesNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnNumRepliesNextName + " is not found or incorrect type");
            res = 0;
        }

        auto diff = GetChildByName(m_aif.m_cbGameDifficultyName);
        if (diff && diff->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)))
        {
            m_cbGameDifficulty = dynamic_cast<ComboBoxWnd*>(diff);
            if (res)
            {
                this->m_gameDataFlags |= 1u;
                if ((m_gameDataFlags & 1) != 0)
                {
                    m_sliderNumReplies->SetMinMax(0, 100);
                    UpdateNumRepliesPrevNextButtonsState();
                }
                InitGameDifficultyControls();
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_cbGameDifficultyName + " is not found or incorrect type");
            res = 0;
        }
        if ((this->m_gameDataFlags & 1) != 0)
            return 1;
        M3D_LOG_INFO("GameOptionsWnd: error - fail to init because of a bad resource");
        return 0;
    }
}

void GameOptionsWnd::UpdateAutoHelpControls()
{
    throw std::logic_error("Not implemented");
}

int GameOptionsWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::UpdateGameDifficultyControls()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::ApplyAutoHelp()
{
    throw std::logic_error("Not implemented");
}

void GameOptionsWnd::InitControls()
{
    throw std::logic_error("Not implemented");
}
