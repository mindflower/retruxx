#include "historywnd.h"

RT_CLASS_EXPORTS_BEGIN(HistoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HistoryWnd);

HistoryWnd::AuxInfo::AuxInfo()
{
    m_txtName = "txt";
    m_timeColor = 0x88FF0000;
}

m3d::Class* HistoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* HistoryWnd::CreateObject()
{
    return new HistoryWnd;
}

m3d::Object* HistoryWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HistoryWnd::AddRecord(CStr const&, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HistoryWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HistoryWnd);
}

HistoryWnd::~HistoryWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HistoryWnd::GameDataSetup()
{
    // TODO: implement HistoryWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

HistoryWnd::HistoryWnd()
{
    m_txt = 0;
}

HistoryWnd::HistoryWnd(HistoryWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HistoryWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HistoryWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HistoryWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HistoryWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
