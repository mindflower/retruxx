#include "bookswnd.h"

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <ui/textbox.h>
#include <game/m3dgame.h>

#include "checklist.h"

RT_CLASS_EXPORTS_BEGIN(BooksWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BooksWnd);

BooksWnd::AuxInfo::AuxInfo()
{
    m_wndBookTextName = "wndBookText";
    m_bookListName = "bookList";
}

BooksWnd::AuxInfo::AuxInfo(BooksWnd::AuxInfo const& rhs) :
    m_wndBookTextName(rhs.m_wndBookTextName),
    m_bookListName(rhs.m_bookListName)
{
}

// ---- lifecycle --------------------------------------------------------------

BooksWnd::BooksWnd()
{
    m_wndBookText = nullptr;
    m_bookList = nullptr;
}

BooksWnd::BooksWnd(BooksWnd const& rhs) : Wnd(rhs)
{
    // RVA 0x4CF020: fresh Wnd base + fresh AuxInfo + empty book list; copies
    // nothing from rhs beyond the base window.
    m_wndBookText = nullptr;
    m_bookList = nullptr;
}

BooksWnd::~BooksWnd()
{
    // RVA 0x4CF060
    GameDataClear(false);
}

m3d::Object* BooksWnd::Clone()
{
    // RVA 0x4CEDF0: allocates a BooksWnd, runs the plain Wnd + AuxInfo ctors and
    // returns it - nothing is carried across.
    return new BooksWnd(*this);
}

m3d::Object* BooksWnd::CreateObject()
{
    return new BooksWnd;
}

m3d::Class* BooksWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* BooksWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BooksWnd);
}

// ---- book table -----------------------------------------------------------

BooksWnd::Book const* BooksWnd::GetBookByName(CStr const& nameId) const
{
    // RVA 0x4CF800
    if (nameId.empty())
    {
        return nullptr;
    }
    for (auto const& book : m_books)
    {
        if (book.m_nameId == nameId)
        {
            return &book;
        }
    }
    return nullptr;
}

bool BooksWnd::BookExists(CStr const& nameId) const
{
    // RVA 0x4D0320
    return GetBookByName(nameId) != nullptr;
}

int BooksWnd::AddBook(CStr const& nameId, CStr const& textId, bool bNotify)
{
    // RVA 0x4CF8D0
    if (nameId.empty() || textId.empty())
    {
        M3D_LOG_INFO("BooksWnd::AddBook error - invalid params");
        return 0;
    }
    if (GetBookByName(nameId))
    {
        M3D_LOG_INFO("BooksWnd::AddBook - book with name " + nameId + " already exists");
        return 1;
    }

    Book newBook;
    newBook.m_nameId = nameId;
    newBook.m_textId = textId;
    m_books.push_back(newBook);

    if (m_bookList)
    {
        int const idx = m_bookList->AddButtonByName(nameId, CStr());
        if (idx != -1)
        {
            m_bookList->SetCurSel(idx);
        }
    }
    if (bNotify)
    {
        M3D_APP->m_pInterfaceManager->AddFadingMsgByStrId("fm_book_got", {});
    }
    return 1;
}

int BooksWnd::ShowBook(CStr const& nameId)
{
    // RVA 0x4CF740
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!nameId.empty())
    {
        auto const* book = GetBookByName(nameId);
        if (!book)
        {
            return 0;
        }
        m_wndBookText->SetText(M3D_APP->GetStringByStringId0(book->m_textId));
    }
    else
    {
        m_wndBookText->SetText(CStr());
    }
    return 1;
}

// ---- game-data hooks ----------------------------------------------------

int BooksWnd::GameDataSetup()
{
    // RVA 0x4CF120
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto* textChild = GetChildByName(m_aif.m_wndBookTextName);
        if (textChild && textChild->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            m_wndBookText = dynamic_cast<m3d::ui::TextBoxWnd*>(textChild);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndBookTextName + " is not found or incorrect type");
            res = 0;
        }

        auto* listChild = GetChildByName(m_aif.m_bookListName);
        if (listChild && listChild->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            m_bookList = dynamic_cast<CheckList*>(M3D_KERNEL->New("CheckList"));
            if (m_bookList)
            {
                if (!m_bookList->CreateFromPattern(dynamic_cast<m3d::ui::Wnd*>(listChild), true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_bookListName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_bookListName +
                    " - cannot find rtti class CheckList");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_bookListName + " is not found or incorrect type");
            res = 0;
        }

        if (m_bookList)
        {
            MoveChildToFirstPosition(m_bookList);
        }
        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("BooksWnd: error - fail to init because of a bad resource");
    return 0;
}

int BooksWnd::GameDataClear(bool bedoreContinuousLevel)
{
    // RVA 0x4CF580
    if (!bedoreContinuousLevel)
    {
        m_books.clear();
        if (m_bookList)
        {
            m_bookList->RemoveAllItems();
        }
        if (m_wndBookText)
        {
            m_wndBookText->SetText(CStr());
        }
    }
    return 1;
}

int BooksWnd::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x4CFC00
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("BooksWnd::GameDataSave error - BooksWnd has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("BooksWnd::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr booksWndNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "BooksWnd");
    guiNode->AddChild(booksWndNode);

    CStr curBook;
    int const curSel = m_bookList->GetCurSel();
    if (curSel != -1)
    {
        if (auto* button = m_bookList->GetItem(curSel))
        {
            curBook = button->GetName();
        }
    }
    booksWndNode->SetAttribute("CurBook", curBook.c_str());

    ref_ptr booksNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Books");
    booksWndNode->AddChild(booksNode);
    for (auto const& book : m_books)
    {
        ref_ptr bookNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Book");
        booksNode->AddChild(bookNode);
        bookNode->SetAttribute("NameId", book.m_nameId.c_str());
        bookNode->SetAttribute("TextId", book.m_textId.c_str());
    }
    return 1;
}

int BooksWnd::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x4CFF10
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("BooksWnd::GameDataLoad error - BooksWnd has been not properly inited");
        return 0;
    }
    GameDataClear(false);
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("BooksWnd::GameDataLoad error - invalid params");
        return 0;
    }

    ref_ptr booksWndNode = xmlFile->CreateNode();
    guiNode->GetFirstChild(booksWndNode, "BooksWnd");
    if (booksWndNode->IsEmpty())
    {
        M3D_LOG_INFO("BooksWnd::GameDataLoad error - cannot find BooksWnd node");
        return 0;
    }

    CStr selBookNameId;
    m3d::SafeStrAttrib(selBookNameId, booksWndNode, "CurBook");

    int res = 1;
    ref_ptr booksNode = xmlFile->CreateNode();
    booksWndNode->GetFirstChild(booksNode, "Books");
    if (!booksNode->IsEmpty())
    {
        ref_ptr bookNode = xmlFile->CreateNode();
        booksNode->GetFirstChild(bookNode, "Book");
        while (!bookNode->IsEmpty())
        {
            CStr nameId;
            CStr textId;
            m3d::SafeStrAttrib(nameId, bookNode, "NameId");
            m3d::SafeStrAttrib(textId, bookNode, "TextId");
            res &= AddBook(nameId, textId, false);
            bookNode->GetNextSibling(bookNode, "Book");
        }
    }

    if (!selBookNameId.empty())
    {
        m_bookList->SelectButtonByName(selBookNameId);
    }
    if (!res)
    {
        M3D_LOG_INFO("BooksWnd::GameDataLoad - loading with errors");
    }
    return res;
}

int BooksWnd::OnAfterAddToWndStation()
{
    // RVA 0x4D0330
    int const res = Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

int BooksWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x4CF630
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id != 300000 || msg != 5)
    {
        return 0;
    }
    auto* checkList = dynamic_cast<CheckList*>(from);
    if (!checkList)
    {
        return 1;
    }
    int const sel = checkList->GetCurSel();
    if (sel == -1)
    {
        ShowBook(CStr());
    }
    else if (auto* button = checkList->GetItem(sel))
    {
        ShowBook(button->GetName());
    }
    return 1;
}
