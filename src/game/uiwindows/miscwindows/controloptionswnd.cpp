#include "controloptionswnd.h"
#include "bindkeyswnd.h"
#include "config.h"
#include "m3dapp.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "ui/slider.h"

RT_CLASS_EXPORTS_BEGIN(ControlOptionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ControlOptionsWnd);

ControlOptionsWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* ControlOptionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int ControlOptionsWnd::ApplyChanges(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ControlOptionsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ControlOptionsWnd);
}

m3d::Object* ControlOptionsWnd::CreateObject()
{
    return new ControlOptionsWnd;
}

ControlOptionsWnd::~ControlOptionsWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ControlOptionsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::OnCheckMouseFlipYClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::OnCheckMouseFlipXClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::UpdateControls()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::UpdateMouseFlipYControls()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::InitMouseSensitivityControls()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto mouseMaxSense = m3d::g_Kernel->GetEngineCfg().m_mouseMaxSensitivity.GetF();
        auto mouseMinSense = m3d::g_Kernel->GetEngineCfg().m_mouseMinSensitivity.GetF();
        m_sliderMouseSensitivity->SetMinMax(mouseMinSense*100.0, mouseMaxSense*100.0);
        UpdateMouseSensitivityPrevNextButtonsState();
    }
}

void ControlOptionsWnd::UpdateMouseFlipXControls()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::ApplyMouseFlipY()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::ApplyMouseFlipX()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::InitMouseFlipXControls()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::InitMouseFlipYControls()
{
    throw std::logic_error("Not implemented");
}

ControlOptionsWnd::ControlOptionsWnd(ControlOptionsWnd const&)
{
    throw std::logic_error("Not implemented");
}

ControlOptionsWnd::ControlOptionsWnd()
{
}

void ControlOptionsWnd::OnBtnMouseSensitivityNextClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int ControlOptionsWnd::GameDataSetup()
{
    using namespace m3d::ui;
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto sense = GetChildByName(m_aif.m_sliderMouseSensitivityName);
        if (sense && sense->IsKindOf(RT_CLASS_LOCAL(SliderWnd)))
        {
            m_sliderMouseSensitivity = dynamic_cast<SliderWnd*>(sense);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderMouseSensitivityName + " is not found or incorrect type");
            res = 0;
        }

        auto sensePrev = GetChildByName(m_aif.m_btnMouseSensitivityPrevName);
        if (sensePrev && sensePrev->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnMouseSensitivityPrev = dynamic_cast<ButtonWnd*>(sensePrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnMouseSensitivityPrevName + " is not found or incorrect type");
            res = 0;
        }

        auto senseNext = GetChildByName(m_aif.m_btnMouseSensitivityNextName);
        if (senseNext && senseNext->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
        {
            m_btnMouseSensitivityNext = dynamic_cast<ButtonWnd*>(senseNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnMouseSensitivityNextName + " is not found or incorrect type");
            res = 0;
        }

        auto flipY = GetChildByName(m_aif.m_checkMouseFlipYName);
        if (flipY && flipY->IsKindOf(RT_CLASS_LOCAL(CheckWnd)))
        {
            m_checkMouseFlipY = dynamic_cast<CheckWnd*>(flipY);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_checkMouseFlipYName + " is not found or incorrect type");
            res = 0;
        }

        auto flipX = GetChildByName(m_aif.m_checkMouseFlipXName);
        if (flipX && flipX->IsKindOf(RT_CLASS_LOCAL(CheckWnd)))
        {
            m_checkMouseFlipX = dynamic_cast<CheckWnd*>(flipX);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_checkMouseFlipXName + " is not found or incorrect type");
            res = 0;
        }

        auto wndKeyBindings = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp)->m_pInterfaceManager->GetWindow(0);
        if (wndKeyBindings)
        {
	        if (wndKeyBindings->IsKindOf(RT_CLASS_LOCAL(BindKeysWnd)))
	        {
                //TODO: check this!!!!!!!!!q
                m_wndKeyBindings = dynamic_cast<BindKeysWnd*>(&*wndKeyBindings);
                AddChild(m_wndKeyBindings);
                if (res)
                {
                    m_gameDataFlags |= 1u;
                    InitMouseSensitivityControls();
                }
	        }
        }
    }
    if ((this->m_gameDataFlags & 1) != 0)
        return 1;
	M3D_LOG_INFO("ControlOptionsWnd: error - fail to init because of a bad resource");
    return 0;
}

void ControlOptionsWnd::OnBtnMouseSensitivityPrevClick(m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int ControlOptionsWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if ((this->m_style & 0x100000) != 0)
        m3d::ui::Wnd::ReflectChildNotifyToParent(from, id, msg, data);
    switch (id)
    {
    case 0x28A0u:
	    {
		    if (msg != 5)
                return 0;
    		ControlOptionsWnd::OnSliderMouseSensitivityChange(data);
            return 1;
	    }
    case 0x28A1u:
	    {
		    if (msg != 1)
                return 0;
    		ControlOptionsWnd::OnBtnMouseSensitivityPrevClick(data);
            return 1;
	    }
    case 0x28A2u:
	    {
		    if (msg != 1)
                return 0;
    		ControlOptionsWnd::OnBtnMouseSensitivityNextClick(data);
            return 1;
	    }
    case 0x28A3u:
	    {
		    if (msg != 1)
                return 0;
    		ControlOptionsWnd::OnCheckMouseFlipYClick(data);
            return 1;
	    }
    case 0x28A4u:
	    {
		    if (msg != 1)
                return 0;
    		ControlOptionsWnd::OnCheckMouseFlipXClick(data);
            return 1;
	    }
    default:
        return 0;
    }
}

void ControlOptionsWnd::UpdateMouseSensitivityControls()
{
    throw std::logic_error("Not implemented");
}

int ControlOptionsWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::ApplyMouseSensitivity()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::UpdateMouseSensitivityPrevNextButtonsState()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        auto notch = m_sliderMouseSensitivity->GetNotch();
        m_btnMouseSensitivityPrev->EnableWindow(notch > m_sliderMouseSensitivity->GetMin());
        m_btnMouseSensitivityNext->EnableWindow(notch < m_sliderMouseSensitivity->GetMax());
    }
}

void ControlOptionsWnd::InitControls()
{
    throw std::logic_error("Not implemented");
}

void ControlOptionsWnd::OnSliderMouseSensitivityChange(m3d::AIParam const&)
{
    UpdateMouseSensitivityPrevNextButtonsState();
    if (IsChildOf(m3d::Application::g_pApp) && !m_sliderMouseSensitivityBlocked)
    {
        ApplyMouseSensitivity();
    }
    if (m_sliderMouseSensitivityBlocked > 0)
        m_sliderMouseSensitivityBlocked = m_sliderMouseSensitivityBlocked - 1;
}
