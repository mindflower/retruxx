#pragma once

class BooksWnd :  public m3d::ui::Wnd
{
public:
    bool BookExists(CStr const &) const ;
    virtual struct m3d::Class * GetClass() const ;
    virtual ~BooksWnd();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    int AddBook(CStr const &,CStr const &,bool);
protected:
    virtual int GameDataLoad(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    BooksWnd();
    BooksWnd(class BooksWnd const &);
    struct Book const * GetBookByName(CStr const &) const ;
    virtual int GameDataClear(bool);
    virtual int GameDataSetup();
    virtual int OnAfterAddToWndStation();
    virtual int GameDataSave(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    int ShowBook(CStr const &);
private:
    std::_Copy_backward_opt<Book *,Book *>(Book *,Book *,Book *,std::_Nonscalar_ptr_iterator_tag);
    std::_Destroy<Book>(Book *);
    std::_Allocate<Book>(uint,Book *);
    std::fill<Book *,Book>(Book *,Book *,Book const &);
    std::_Construct<Book,Book>(Book *,Book const &);
    std::_Ptr_cat<Book *,Book *>(Book * &,Book * &);
    std::allocator<Book>::allocator<Book>();
    std::allocator<Book>::allocator<Book>();
    std::copy_backward<Book *,Book *>(Book *,Book *,Book *);
    m3d::ui::TextBoxWnd *m_wndBookText;
    CheckList *m_bookList;
    BooksWnd::AuxInfo m_aif;
    std::vector<BooksWnd::Book> m_books;
};
