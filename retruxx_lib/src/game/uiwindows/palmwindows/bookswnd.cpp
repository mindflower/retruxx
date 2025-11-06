#include "bookswnd.h"

RT_CLASS_EXPORTS_BEGIN(BooksWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BooksWnd);

BooksWnd::AuxInfo::AuxInfo()
{
    m_wndBookTextName = "wndBookText";
    m_bookListName = "bookList";
}

bool BooksWnd::BookExists(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BooksWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BooksWnd);
}

BooksWnd::~BooksWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BooksWnd::CreateObject()
{
    return new BooksWnd;
}

m3d::Class* BooksWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* BooksWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BooksWnd::AddBook(CStr const&, CStr const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BooksWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BooksWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BooksWnd::BooksWnd()
{
    m_wndBookText = 0;
    m_bookList = 0;
}

BooksWnd::BooksWnd(BooksWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BooksWnd::Book const* BooksWnd::GetBookByName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BooksWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BooksWnd::GameDataSetup()
{
    // TODO: implement BooksWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int BooksWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BooksWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BooksWnd::ShowBook(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
