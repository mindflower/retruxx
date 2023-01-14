#include "gameoptionswnd.h"

#include "game/m3dgame.h"
#include "core/log.h"
#include "game/profile.h"
#include "game/uimisc/helpmanager.h"
#include "server/objects/base/globalproperties.h"
#include "ui/button.h"
#include "ui/comboboxwnd.h"
#include "ui/slider.h"

RT_CLASS_EXPORTS_BEGIN(GameOptionsWnd)
RT_CLASS_EXPORTS_END;
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
    if ((m_gameDataFlags & 1) != 0)
    {
        auto profile = M3D_APP->GetProfileManager()->GetCurProfile();
        if (profile)
        {
            m3d::AIParam const param{static_cast<float>(m_sliderNumReplies->GetNotch() * 0.0099999998)};
            profile->SetParam(PP_NUM_RADIO_REPLIES_COEFF, param);
        }
    }
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
    UpdateNumRepliesPrevNextButtonsState();
    if (IsChildOf(m3d::Application::g_pApp) && !m_sliderNumRepliesBlocked)
    {
        ApplyNumReplies();
    }
    if (m_sliderNumRepliesBlocked > 0)
        m_sliderNumRepliesBlocked = m_sliderNumRepliesBlocked - 1;
}

void GameOptionsWnd::OnCheckAutoHelpClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int GameOptionsWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if ((this->m_style & 0x100000) != 0)
        m3d::ui::Wnd::ReflectChildNotifyToParent(from, id, msg, data);
    switch (id)
    {
    case 0x283Du:
	    {
			if (msg != 5)
				return 0;
    		GameOptionsWnd::OnSliderNumRepliesChange(data);
            return 1;
	    }
    case 0x283Eu:
	    {
		    if (msg != 1)
                return 0;
    		GameOptionsWnd::OnBtnNumRepliesPrevClick(data);
            return 1;
	    }
    case 0x283Fu:
	    {
		    if (msg != 1)
                return 0;
    		GameOptionsWnd::OnBtnNumRepliesNextClick(data);
            return 1;
	    }
    case 0x2840u:
	    {
		    if (msg != 1)
                return 0;
    		GameOptionsWnd::ApplyAutoHelp();
            return 1;
	    }
    case 0x2841u:
	    {
		    if (msg != 5)
                return 0;
    		GameOptionsWnd::OnCbGameDifficultyChange(data);
            return 1;
	    }
    default:
        return 0;
    }
}

void GameOptionsWnd::UpdateNumRepliesControls()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto profile = M3D_APP->GetProfileManager()->GetCurProfile();
        if (profile)
        {
            m3d::AIParam param;
            if (profile->GetParam(PP_NUM_RADIO_REPLIES_COEFF, param))
            {
                ++m_sliderNumRepliesBlocked;
                m_sliderNumReplies->SetNotch(param.GetAsFloat());
            }
        }
    }
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
    if ((m_gameDataFlags & 1) != 0)
    {
        m_sliderNumReplies->SetNotch(m_sliderNumReplies->GetNotch() + 1);
    }
}

void GameOptionsWnd::OnCbGameDifficultyChange(m3d::AIParam const&)
{
    if (IsChildOf(M3D_APP) && !m_cbGameDifficultyBlocked)
        ApplyGameDifficulty();
    auto blocked = m_cbGameDifficultyBlocked;
    if (blocked > 0)
        m_cbGameDifficultyBlocked = blocked - 1;
}

void GameOptionsWnd::OnBtnNumRepliesPrevClick(m3d::AIParam const&)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_sliderNumReplies->SetNotch(m_sliderNumReplies->GetNotch() - 1);
    }
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
    if ((m_gameDataFlags & 1) != 0)
    {
        auto profile = M3D_APP->GetProfileManager()->GetCurProfile();
        if (profile)
        {
            m3d::AIParam param;
            if (profile->GetParam(PP_AUTOHELP_ENABLED, param))
            {
                m_checkAutoHelp->SetCheck(param.GetAsID());
            }
        }
    }
}

int GameOptionsWnd::OnBeforeAddToWndStation()
{
    UpdateAutoHelpControls();
    UpdateNumRepliesControls();
    UpdateGameDifficultyControls();
    return Wnd::OnBeforeAddToWndStation();
}

void GameOptionsWnd::UpdateGameDifficultyControls()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto const level = M3D_APP->GetCurDifficultyLevel();
        ++m_cbGameDifficultyBlocked;
        m_cbGameDifficulty->SetCurSel(-1);
        for (int i = 0; i < m_cbGameDifficulty->GetCount(); ++i)
        {
            if (m_cbGameDifficulty->GetItemData(i) == level)
            {
                ++m_cbGameDifficultyBlocked;
                m_cbGameDifficulty->SetCurSel(i);
                break;
            }
        }
        if (m_cbGameDifficulty->GetCurSel() == -1)
        {
            m_cbGameDifficulty->SetText("unknown level: " + CStr(level));
        }
    }
}

void GameOptionsWnd::ApplyAutoHelp()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto const enableAutoHelp = m_checkAutoHelp->GetCheck() != 0;
        M3D_APP->m_pInterfaceManager->GetHelpManager()->EnableAutoHelp(enableAutoHelp);
        if (enableAutoHelp)
        {
            M3D_APP->m_pInterfaceManager->GetHelpManager()->ResetAutoHelp();
        }
    }
}

void GameOptionsWnd::InitControls()
{
    throw std::logic_error("Not implemented");
}
