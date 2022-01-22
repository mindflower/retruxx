#pragma once
#include <ui/ui.h>

class CheckList;

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}

class BooksWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndBookTextName;
        CStr m_bookListName;
    };

    class Book
    {
    private:
        CStr m_nameId;
        CStr m_textId;
    };

public:
    bool BookExists(CStr const &) const ;
    virtual m3d::Class * GetClass() const ;
    virtual ~BooksWnd();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    int AddBook(CStr const &,CStr const &,bool);

protected:
    virtual int GameDataLoad(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    BooksWnd();
    BooksWnd(BooksWnd const &);
    Book const * GetBookByName(CStr const &) const ;
    virtual int GameDataClear(bool);
    virtual int GameDataSetup();
    virtual int OnAfterAddToWndStation();
    virtual int GameDataSave(m3d::cmn::XmlFile *, m3d::cmn::XmlNode *);
    int ShowBook(CStr const &);

public:
    RT_CLASS_DECLARE(BooksWnd);

private:
    m3d::ui::TextBoxWnd *m_wndBookText;
    CheckList *m_bookList;
    BooksWnd::AuxInfo m_aif;
    std::vector<BooksWnd::Book> m_books;
};
