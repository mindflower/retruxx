#include "devidedlg.h"

RT_CLASS_EXPORTS_BEGIN(DevideDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DevideDlg);

DevideDlg::AuxInfo::AuxInfo()
{
    m_lblOriginalAmountName = "lblOriginalAmount";
    m_lblDragAmountName = "lblDragAmount";
    m_sliderName = "sldAmount";
    m_btnPlusName = "btnPlus";
    m_btnMinusName = "btnMinus";
}

m3d::Class* DevideDlg::GetClass() const
{
    return RT_CLASS_LOCAL(DevideDlg);
}

m3d::Object* DevideDlg::CreateObject()
{
    return new DevideDlg;
}

int DevideDlg::GameDataSetup()
{
    // TODO: implement DevideDlg::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void DevideDlg::UpdateLabels()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* DevideDlg::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DevideDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DevideDlg::GetDragAmount()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DevideDlg::GetOriginalAmount()
{
    RETRUXX_NOT_IMPLEMENTED;
}

DevideDlg::~DevideDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DevideDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

void DevideDlg::SetAmount(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

DevideDlg::DevideDlg(DevideDlg const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

DevideDlg::DevideDlg()
{
    m_maxAmount = 0;
    m_lblOriginalAmount = 0;
    m_lblDragAmount = 0;
    m_slider = 0;
    m_btnPlus = 0;
    m_btnMinus = 0;
    m_isInited = 0;
}
