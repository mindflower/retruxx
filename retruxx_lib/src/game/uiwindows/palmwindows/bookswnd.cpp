#include "bookswnd.h"

RT_CLASS_EXPORTS_BEGIN(BooksWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BooksWnd);

BooksWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool BooksWnd::BookExists(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BooksWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

BooksWnd::~BooksWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BooksWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
