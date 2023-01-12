#include "soundoptionswnd.h"

#include "m3dapp.h"
#include "core/log.h"
#include "ui/button.h"
#include "ui/slider.h"

RT_CLASS_EXPORTS_BEGIN(SoundOptionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SoundOptionsWnd);

SoundOptionsWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* SoundOptionsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SoundOptionsWnd);
}

m3d::Object* SoundOptionsWnd::CreateObject()
{
    return new SoundOptionsWnd;
}

m3d::Object* SoundOptionsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SoundOptionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

SoundOptionsWnd::~SoundOptionsWnd()
{
    throw std::logic_error("Not implemented");
}

int SoundOptionsWnd::OnBeforeAddToWndStation()
{
    //TODO: implement SoundOptionsWnd::OnBeforeAddToWndStation
    //SoundOptionsWnd::UpdateControls(this);
    return Wnd::OnBeforeAddToWndStation();
}

void SoundOptionsWnd::InitControls()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_sliderMusicVolume->SetMinMax(0, 100);
        UpdateMusicVolumePrevNextButtonsState();
        if ((m_gameDataFlags & 1) != 0)
        {
            m_sliderEffectsVolume->SetMinMax(0, 100);
            UpdateEffectsVolumePrevNextButtonsState();
            if ((m_gameDataFlags & 1) != 0)
            {
                m_sliderSpeakVolume->SetMinMax(0, 100);
                UpdateSpeakVolumePrevNextButtonsState();
            }
        }
    }
}

void SoundOptionsWnd::InitEffectsVolumeControls()
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::ApplyEffectsVolume()
{
    throw std::logic_error("Not implemented");
}

int SoundOptionsWnd::OnWndNotify(m3d::ui::Wnd *from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if ((this->m_style & 0x100000) != 0)
        m3d::ui::Wnd::ReflectChildNotifyToParent(from, id, msg, data);
    switch (id)
    {
    case 0x27D8u:
    {
        if (msg != 5)
            return 0;
        SoundOptionsWnd::OnSliderMusicVolumeChange(data);
        return 1;
    }
    case 0x27D9u:
	    {
		    if (msg != 1)
		    	return 0;
    		SoundOptionsWnd::OnBtnMusicVolumePrevClick(data);
            return 1;
	    }
    case 0x27DAu:
	    {
		    if (msg != 1)
		    	return 0;
    		SoundOptionsWnd::OnBtnMusicVolumeNextClick(data);
            return 1;
	    }
    case 0x27DBu:
	    {
		    if (msg != 5)
		    	return 0;
    		SoundOptionsWnd::OnSliderEffectsVolumeChange(data);
            return 1;
	    }
    case 0x27DCu:
	    {
		    if (msg != 1)
		    	return 0;
    		SoundOptionsWnd::OnBtnEffectsVolumePrevClick(data);
            return 1;
	    }
    case 0x27DDu:
	    {
		    if (msg != 1)
		    	return 0;
    		SoundOptionsWnd::OnBtnEffectsVolumeNextClick(data);
            return 1;
	    }
    case 0x27DEu:
	    {
		    if (msg != 5)
		    	return 0;
    		SoundOptionsWnd::OnSliderSpeakVolumeChange(data);
            return 1;
	    }
    case 0x27DFu:
	    {
		    if (msg != 1)
		    	return 0;
    		SoundOptionsWnd::OnBtnSpeakVolumePrevClick(data);
            return 1;
	    }
    case 0x27E0u:
	    {
		    if (msg != 1)
		    	return 0;
    		SoundOptionsWnd::OnBtnSpeakVolumeNextClick(data);
            return 1;
	    }
    default:
        return 0;
    }
}

void SoundOptionsWnd::OnBtnMusicVolumeNextClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::OnBtnMusicVolumePrevClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::OnBtnEffectsVolumePrevClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::ApplySpeakVolume()
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::UpdateControls()
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::OnBtnEffectsVolumeNextClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::UpdateMusicVolumeControls()
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::UpdateMusicVolumePrevNextButtonsState()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto notch = m_sliderMusicVolume->GetNotch();
        m_btnMusicVolumePrev->EnableWindow(notch > m_sliderMusicVolume->GetMin());
        m_btnMusicVolumeNext->EnableWindow(notch < m_sliderMusicVolume->GetMax());
    }
}

void SoundOptionsWnd::OnSliderSpeakVolumeChange(m3d::AIParam const&)
{
    UpdateSpeakVolumePrevNextButtonsState();
    if (IsChildOf(m3d::Application::g_pApp) && !m_sliderSpeakVolumeBlocked)
    {
        ApplySpeakVolume();
    }
    if (m_sliderSpeakVolumeBlocked > 0)
        m_sliderSpeakVolumeBlocked = m_sliderSpeakVolumeBlocked - 1;
}

void SoundOptionsWnd::UpdateSpeakVolumeControls()
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::OnSliderMusicVolumeChange(m3d::AIParam const&)
{
    UpdateMusicVolumePrevNextButtonsState();
    if (IsChildOf(m3d::Application::g_pApp) && !m_sliderMusicVolumeBlocked)
    {
        ApplyMusicVolume();
    }
    if (m_sliderMusicVolumeBlocked > 0)
        m_sliderMusicVolumeBlocked = m_sliderMusicVolumeBlocked - 1;
}

void SoundOptionsWnd::UpdateSpeakVolumePrevNextButtonsState()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto notch = m_sliderSpeakVolume->GetNotch();
        m_btnSpeakVolumePrev->EnableWindow(notch > m_sliderSpeakVolume->GetMin());
        m_btnSpeakVolumeNext->EnableWindow(notch < m_sliderSpeakVolume->GetMax());
    }
}

void SoundOptionsWnd::ApplyMusicVolume()
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::OnBtnSpeakVolumePrevClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::OnBtnSpeakVolumeNextClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::UpdateEffectsVolumePrevNextButtonsState()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto notch = m_sliderEffectsVolume->GetNotch();
        m_btnEffectsVolumePrev->EnableWindow(notch > m_sliderEffectsVolume->GetMin());
        m_btnEffectsVolumeNext->EnableWindow(notch < m_sliderEffectsVolume->GetMax());
    }
}

void SoundOptionsWnd::InitMusicVolumeControls()
{
    throw std::logic_error("Not implemented");
}

void SoundOptionsWnd::OnSliderEffectsVolumeChange(m3d::AIParam const&)
{
    UpdateEffectsVolumePrevNextButtonsState();
    if (IsChildOf(m3d::Application::g_pApp) && !m_sliderEffectsVolumeBlocked)
    {
        ApplyEffectsVolume();
    }
    if (m_sliderEffectsVolumeBlocked > 0)
        m_sliderEffectsVolumeBlocked = m_sliderEffectsVolumeBlocked - 1;
}

void SoundOptionsWnd::InitSpeakVolumeControls()
{
    throw std::logic_error("Not implemented");
}

int SoundOptionsWnd::GameDataSetup()
{
    using namespace m3d::ui;
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto musicVolume = GetChildByName(m_aif.m_sliderMusicVolumeName);
        if (musicVolume && musicVolume->IsKindOf(RT_CLASS_LOCAL(SliderWnd)))
        {
            m_sliderMusicVolume = dynamic_cast<SliderWnd*>(musicVolume);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderMusicVolumeName + " is not found or incorrect type");
            res = 0;
        }

        auto effectsVolume = GetChildByName(m_aif.m_sliderEffectsVolumeName);
        if (effectsVolume && effectsVolume->IsKindOf(RT_CLASS_LOCAL(SliderWnd)))
        {
            m_sliderEffectsVolume = dynamic_cast<SliderWnd*>(effectsVolume);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderEffectsVolumeName + " is not found or incorrect type");
            res = 0;
        }

        auto speakVolume = GetChildByName(m_aif.m_sliderSpeakVolumeName);
        if (speakVolume && speakVolume->IsKindOf(RT_CLASS_LOCAL(SliderWnd)))
        {
            m_sliderSpeakVolume = dynamic_cast<SliderWnd*>(speakVolume);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderSpeakVolumeName + " is not found or incorrect type");
            res = 0;
        }

        auto musicVolumePrev = GetChildByName(m_aif.m_btnMusicVolumePrevName);
        if (musicVolumePrev && musicVolumePrev->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnMusicVolumePrev = dynamic_cast<ButtonWnd*>(musicVolumePrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnMusicVolumePrevName + " is not found or incorrect type");
            res = 0;
        }

        auto musicVolumeNext = GetChildByName(m_aif.m_btnMusicVolumeNextName);
        if (musicVolumeNext && musicVolumeNext->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnMusicVolumeNext = dynamic_cast<ButtonWnd*>(musicVolumeNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnMusicVolumeNextName + " is not found or incorrect type");
            res = 0;
        }

        auto effectsVolumePrev = GetChildByName(m_aif.m_btnEffectsVolumePrevName);
        if (effectsVolumePrev && effectsVolumePrev->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnEffectsVolumePrev = dynamic_cast<ButtonWnd*>(effectsVolumePrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnEffectsVolumePrevName + " is not found or incorrect type");
            res = 0;
        }

        auto effectsVolumeNext = GetChildByName(m_aif.m_btnEffectsVolumeNextName);
        if (effectsVolumeNext && effectsVolumeNext->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnEffectsVolumeNext = dynamic_cast<ButtonWnd*>(effectsVolumeNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnEffectsVolumeNextName + " is not found or incorrect type");
            res = 0;
        }

        auto speakVolumePrev = GetChildByName(m_aif.m_btnSpeakVolumePrevName);
        if (speakVolumePrev && speakVolumePrev->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnSpeakVolumePrev = dynamic_cast<ButtonWnd*>(speakVolumePrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnSpeakVolumePrevName + " is not found or incorrect type");
            res = 0;
        }

        auto speakVolumeNext = GetChildByName(m_aif.m_btnSpeakVolumeNextName);
        if (speakVolumeNext && speakVolumeNext->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnSpeakVolumeNext = dynamic_cast<ButtonWnd*>(speakVolumeNext);
            if (res)
            {
                m_gameDataFlags |= 1u;
                InitControls();
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnSpeakVolumeNextName + " is not found or incorrect type");
            res = 0;
        }
    }
    if ((m_gameDataFlags & 1) != 0)
        return 1;
    M3D_LOG_INFO("SoundOptionsWnd: error - fail to init because of a bad resource");
    return 0;

}

SoundOptionsWnd::SoundOptionsWnd(SoundOptionsWnd const&)
{
    throw std::logic_error("Not implemented");
}

SoundOptionsWnd::SoundOptionsWnd()
{
}

void SoundOptionsWnd::UpdateEffectsVolumeControls()
{
    throw std::logic_error("Not implemented");
}
