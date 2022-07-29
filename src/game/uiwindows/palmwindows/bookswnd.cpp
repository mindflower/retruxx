#include "bookswnd.h"

RT_CLASS_EXPORTS_BEGIN(BooksWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BooksWnd);

BooksWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

bool BooksWnd::BookExists(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BooksWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

BooksWnd::~BooksWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BooksWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BooksWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BooksWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::AddBook(CStr const&, CStr const&, bool)
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

BooksWnd::BooksWnd()
{
    throw std::logic_error("Not implemented");
}

BooksWnd::BooksWnd(BooksWnd const&)
{
    throw std::logic_error("Not implemented");
}

BooksWnd::Book const* BooksWnd::GetBookByName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

int BooksWnd::ShowBook(CStr const&)
{
    throw std::logic_error("Not implemented");
}
