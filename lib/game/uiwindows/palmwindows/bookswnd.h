#pragma once
#include <ui/ui.h>

class CheckList;

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}  // namespace m3d

class BooksWnd : public m3d::ui::Wnd
{
public:
    int AddBook(CStr const& nameId, CStr const& textId, bool bNotify);
    bool BookExists(CStr const& nameId) const;

    struct AuxInfo
    {
        CStr m_wndBookTextName;
        CStr m_bookListName;
        AuxInfo(BooksWnd::AuxInfo const&);
        AuxInfo();
    };

    struct Book
    {
        CStr m_nameId;
        CStr m_textId;
    };

    using BooksVector = std::vector<BooksWnd::Book, std::allocator<BooksWnd::Book>>;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool bedoreContinuousLevel) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int OnAfterAddToWndStation() override;
    int ShowBook(CStr const& nameId);
    BooksWnd::Book const* GetBookByName(CStr const& nameId) const;
    m3d::ui::TextBoxWnd* m_wndBookText;
    CheckList* m_bookList;
    BooksWnd::AuxInfo m_aif;
    std::vector<BooksWnd::Book, std::allocator<BooksWnd::Book>> m_books;
    BooksWnd();
    BooksWnd(BooksWnd const& rhs);

public:
    virtual ~BooksWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classBooksWnd;
};
