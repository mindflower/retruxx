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

int ControlOptionsWnd::ApplyChanges(bool bForce)
{
    // RVA 0x4B0FA0
    if ((m_gameDataFlags & 1) != 0)
    {
        return m_wndKeyBindings->ApplyChanges(bForce);
    }
    return 1;
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
    // No explicit body in the shipped build; ~ref_ptr / ~AuxInfo / ~Wnd chain.
}

m3d::Object* ControlOptionsWnd::Clone()
{
    // RVA 0x4AFFF0: allocates, runs the plain Wnd ctor + m_aif, copies nothing.
    return new ControlOptionsWnd(*this);
}

void ControlOptionsWnd::OnCheckMouseFlipYClick(m3d::AIParam const&)
{
    // RVA 0x4B0D10
    ApplyMouseFlipY();
}

void ControlOptionsWnd::OnCheckMouseFlipXClick(m3d::AIParam const&)
{
    // RVA 0x4B0D40
    ApplyMouseFlipX();
}

void ControlOptionsWnd::UpdateControls()
{
    // RVA 0x4B0D90
    UpdateMouseSensitivityControls();
    UpdateMouseFlipYControls();
    UpdateMouseFlipXControls();
}

void ControlOptionsWnd::UpdateMouseFlipYControls()
{
    // RVA 0x4B0EA0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_checkMouseFlipY->SetCheck(M3D_APP->IsMouseYAxisFlipped());
    }
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
    // RVA 0x4B0ED0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_checkMouseFlipX->SetCheck(M3D_APP->IsMouseXAxisFlipped());
    }
}

void ControlOptionsWnd::ApplyMouseFlipY()
{
    // RVA 0x4B0F40
    if ((m_gameDataFlags & 1) != 0)
    {
        M3D_APP->SetMouseYAxisFlipped(m_checkMouseFlipY->GetCheck() != 0);
    }
}

void ControlOptionsWnd::ApplyMouseFlipX()
{
    // RVA 0x4B0F70
    if ((m_gameDataFlags & 1) != 0)
    {
        M3D_APP->SetMouseXAxisFlipped(m_checkMouseFlipX->GetCheck() != 0);
    }
}

void ControlOptionsWnd::InitMouseFlipXControls()
{
    // RVA 0x4B0B90: empty in the shipped build.
}

void ControlOptionsWnd::InitMouseFlipYControls()
{
    // RVA 0x4B0B80: empty in the shipped build.
}

ControlOptionsWnd::ControlOptionsWnd(ControlOptionsWnd const&)
{
    // RVA-less: default-constructs the Wnd base + m_aif + m_wndKeyBindings and
    // copies nothing from the source.
}

ControlOptionsWnd::ControlOptionsWnd()
{
}

void ControlOptionsWnd::OnBtnMouseSensitivityNextClick(m3d::AIParam const&)
{
    // RVA 0x4B0CE0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_sliderMouseSensitivity->SetNotch(m_sliderMouseSensitivity->GetNotch() + 1);
    }
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
    // RVA 0x4B0CB0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_sliderMouseSensitivity->SetNotch(m_sliderMouseSensitivity->GetNotch() - 1);
    }
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
    // RVA 0x4B0DE0
    if ((m_gameDataFlags & 1) != 0)
    {
        ++m_sliderMouseSensitivityBlocked;
        m_sliderMouseSensitivity->SetNotch(static_cast<int>(M3D_APP->GetMouseSensitivity() * 100.0));
    }
}

int ControlOptionsWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4B0D70
    UpdateControls();
    return Wnd::OnBeforeAddToWndStation();
}

void ControlOptionsWnd::ApplyMouseSensitivity()
{
    // RVA 0x4B0F00
    if ((m_gameDataFlags & 1) != 0)
    {
        M3D_APP->SetMouseSensitivity(static_cast<float>(m_sliderMouseSensitivity->GetNotch() * 0.0099999998));
    }
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
    // RVA 0x4B0AC0 (thunk)
    InitMouseSensitivityControls();
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
