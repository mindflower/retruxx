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
        /* 0x0000 */ CStr m_wndBookTextName;
        /* 0x000c */ CStr m_bookListName;
        AuxInfo(BooksWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0018 */

    struct Book
    {
        /* 0x0000 */ CStr m_nameId;
        /* 0x000c */ CStr m_textId;
    }; /* size: 0x0018 */

    using BooksVector = std::vector<BooksWnd::Book, std::allocator<BooksWnd::Book>>;

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool bedoreContinuousLevel) override /* 0x108 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x110 */;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x114 */;
    virtual int OnAfterAddToWndStation() override /* 0x6c */;
    int ShowBook(CStr const& nameId);
    BooksWnd::Book const* GetBookByName(CStr const& nameId) const;
    /* 0x0220 */ m3d::ui::TextBoxWnd* m_wndBookText;
    /* 0x0224 */ CheckList* m_bookList;
    /* 0x0228 */ BooksWnd::AuxInfo m_aif;
    /* 0x0240 */ std::vector<BooksWnd::Book, std::allocator<BooksWnd::Book>> m_books;
    BooksWnd();
    BooksWnd(BooksWnd const& rhs);

public:
    virtual ~BooksWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classBooksWnd;
}; /* size: 0x0250 */
