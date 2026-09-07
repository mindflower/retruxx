#include "devidedlg.h"

#include <core/log.h>
#include <ui/button.h>
#include <ui/slider.h>

RT_CLASS_EXPORTS_BEGIN(DevideDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DevideDlg);

namespace
{
    // Ids the XML gives the three interactive widgets.
    unsigned const ID_BTN_PLUS = 10000;
    unsigned const ID_BTN_MINUS = 10001;
    unsigned const ID_SLIDER = 10002;
}  // namespace

DevideDlg::AuxInfo::AuxInfo()
{
    // RVA 0x442B00
    m_lblOriginalAmountName = "lblOriginalAmount";
    m_lblDragAmountName = "lblDragAmount";
    m_sliderName = "sldAmount";
    m_btnPlusName = "btnPlus";
    m_btnMinusName = "btnMinus";
}

DevideDlg::AuxInfo::AuxInfo(DevideDlg::AuxInfo const& rhs) :
    m_lblOriginalAmountName(rhs.m_lblOriginalAmountName),
    m_lblDragAmountName(rhs.m_lblDragAmountName),
    m_sliderName(rhs.m_sliderName),
    m_btnPlusName(rhs.m_btnPlusName),
    m_btnMinusName(rhs.m_btnMinusName)
{
}

DevideDlg::DevideDlg() :
    m_maxAmount(0),
    m_lblOriginalAmount(nullptr),
    m_lblDragAmount(nullptr),
    m_slider(nullptr),
    m_btnPlus(nullptr),
    m_btnMinus(nullptr),
    m_isInited(false)
{
}

DevideDlg::DevideDlg(DevideDlg const&) : DevideDlg()
{
    // NOTE: the shipped copy ctor (RVA 0x442D70) builds the base and the aux info
    // and swaps the vtable, leaving every other member uninitialised and copying
    // nothing from rhs. Delegating avoids that while copying just as little.
}

DevideDlg::~DevideDlg() = default;

m3d::Object* DevideDlg::Clone()
{
    // RVA 0x442A70
    return new DevideDlg(*this);
}

m3d::Object* DevideDlg::CreateObject()
{
    return new DevideDlg;
}

m3d::Class* DevideDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Class* DevideDlg::GetClass() const
{
    return RT_CLASS_LOCAL(DevideDlg);
}

int DevideDlg::GameDataSetup()
{
    // RVA 0x442DB0 - note the guard is m_isInited rather than the usual
    // m_gameDataFlags, so a second call is a no-op that still reports success.
    if (m_isInited)
    {
        m_isInited = true;
        return 1;
    }

    int res = 1;

    m3d::Object* lblOriginalAmount = GetChildByName(m_aif.m_lblOriginalAmountName);
    if (lblOriginalAmount && lblOriginalAmount->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        m_lblOriginalAmount = static_cast<m3d::ui::Wnd*>(lblOriginalAmount);
    }
    else
    {
        M3D_LOG_INFO(
            "Get control error: control " + m_aif.m_lblOriginalAmountName + " is not found or incorrect type");
        res = 0;
    }

    m3d::Object* lblDragAmount = GetChildByName(m_aif.m_lblDragAmountName);
    if (lblDragAmount && lblDragAmount->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        m_lblDragAmount = static_cast<m3d::ui::Wnd*>(lblDragAmount);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_lblDragAmountName + " is not found or incorrect type");
        res = 0;
    }

    m3d::Object* slider = GetChildByName(m_aif.m_sliderName);
    if (slider && slider->IsKindOf(&m3d::ui::SliderWnd::m_classSliderWnd))
    {
        m_slider = static_cast<m3d::ui::SliderWnd*>(slider);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_sliderName + " is not found or incorrect type");
        res = 0;
    }

    m3d::Object* btnPlus = GetChildByName(m_aif.m_btnPlusName);
    if (btnPlus && btnPlus->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
    {
        m_btnPlus = static_cast<m3d::ui::ButtonWnd*>(btnPlus);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_btnPlusName + " is not found or incorrect type");
        res = 0;
    }

    m3d::Object* btnMinus = GetChildByName(m_aif.m_btnMinusName);
    if (!btnMinus || !btnMinus->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
    {
        // NOTE: unlike the four above, a missing minus button skips the rest of
        // the function outright rather than just clearing res; as shipped.
        M3D_LOG_INFO("Get control error: control " + m_aif.m_btnMinusName + " is not found or incorrect type");
        M3D_LOG_INFO("DevideDlg: fail to init because of a bad resource");
        return 0;
    }
    m_btnMinus = static_cast<m3d::ui::ButtonWnd*>(btnMinus);

    if (!res)
    {
        M3D_LOG_INFO("DevideDlg: fail to init because of a bad resource");
        return 0;
    }

    m_slider->SetMinMax(0, 0);
    m_slider->SetNotch(0);
    m_isInited = true;
    return 1;
}

int DevideDlg::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x4433F0 - the plus/minus buttons nudge the slider, and any slider
    // move refreshes the two amount labels.
    if (!m_isInited)
    {
        return 0;
    }

    switch (idFrom)
    {
    case ID_BTN_PLUS:
        if (message == 1)
        {
            m_slider->SetNotch(m_slider->GetNotch() + 1);
            UpdateLabels();
        }
        return 1;
    case ID_BTN_MINUS:
        if (message == 1)
        {
            m_slider->SetNotch(m_slider->GetNotch() - 1);
            UpdateLabels();
        }
        return 1;
    case ID_SLIDER:
        if (message == 5)
        {
            UpdateLabels();
        }
        return 1;
    default:
        return m3d::ui::ModalWnd::OnWndNotify(from, idFrom, message, data);
    }
}

void DevideDlg::SetAmount(int amount)
{
    // RVA 0x4434A0 - the dialog opens with the stack split down the middle.
    if (!m_isInited)
    {
        return;
    }
    m_maxAmount = amount;
    m_slider->SetMinMax(0, amount);
    m_slider->SetNotch(m_maxAmount / 2);
}

int DevideDlg::GetDragAmount()
{
    // RVA 0x4434F0
    return m_isInited ? m_slider->GetNotch() : 0;
}

int DevideDlg::GetOriginalAmount()
{
    // RVA 0x443510
    return m_isInited ? m_maxAmount - m_slider->GetNotch() : 0;
}

void DevideDlg::UpdateLabels()
{
    // RVA 0x443540
    if (!m_isInited)
    {
        return;
    }

    int const dragAmount = m_slider->GetNotch();
    m_lblDragAmount->SetText(CStr(dragAmount));
    m_lblOriginalAmount->SetText(CStr(m_maxAmount - dragAmount));
}
