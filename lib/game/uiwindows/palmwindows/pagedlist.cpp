#include "pagedlist.h"

#include <algorithm>

#include <core/kernel.h>
#include <m3dapp.h>
#include <ui/button.h>

// ---------------------------------------------------------------------------
//  WndItem
// ---------------------------------------------------------------------------

WndItem::WndItem() : m_idx(0)
{
}

WndItem::WndItem(WndItem const&) : WndItem()
{
}

WndItem::~WndItem() = default;

int WndItem::GetIdx() const
{
    return m_idx;
}

void WndItem::SetIdx(int idx)
{
    m_idx = idx;
}

int WndItem::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x4F5D90 - an item is only a container, so anything its children say
    // is passed straight out to whoever owns the list.
    return ReflectChildNotifyToParent(from, idFrom, message, data);
}

// ---------------------------------------------------------------------------
//  PagedList
// ---------------------------------------------------------------------------

PagedList::PagedList() :
    m_firstVisibleIdx(-1),
    m_lastVisibleIdx(-1),
    m_space(0.0f),
    m_btnNext(nullptr),
    m_btnPrev(nullptr)
{
}

PagedList::PagedList(PagedList const&) : PagedList()
{
}

PagedList::~PagedList() = default;

int PagedList::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x4F5F20
    return ReflectChildNotifyToParent(from, idFrom, message, data);
}

int PagedList::AddItem(WndItem* item)
{
    // RVA 0x4F5F30
    M3D_ASSERT(item);
    m_items.push_back(item);
    item->SetIdx(static_cast<int>(m_items.size()) - 1);
    // The visible window is invalidated; the next ShowNext rebuilds it.
    m_firstVisibleIdx = -1;
    m_lastVisibleIdx = -1;
    return 1;
}

int PagedList::RemoveItem(WndItem* item)
{
    // RVA 0x4F5FE0
    M3D_ASSERT(item);
    auto const it = std::find(m_items.begin(), m_items.end(), item);
    if (it == m_items.end())
    {
        return 0;
    }

    // NOTE: the list owns its items outright - the shipped code destroys the
    // item through its virtual destructor rather than releasing a reference.
    delete item;
    m_items.erase(it);

    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->SetIdx(static_cast<int>(i));
    }
    m_firstVisibleIdx = -1;
    m_lastVisibleIdx = -1;

    if (IsChildOf(M3D_APP))
    {
        // Already on screen, so re-flow from the top.
        while (ShowNext())
        {
        }
    }
    return 1;
}

int PagedList::ClearItems()
{
    // RVA 0x4F6770
    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        delete m_items[i];
        m_items[i] = nullptr;
    }
    m_items.clear();
    UpdateNextPrevButtonsState();
    return 1;
}

int PagedList::SetUp(
    std::vector<BoundsBase<float>, std::allocator<BoundsBase<float>>> const& pagesB,
    m3d::ui::ButtonWnd* btnNext,
    m3d::ui::ButtonWnd* btnPrev)
{
    // RVA 0x4F63C0 - the list's own bounds become the union of its pages.
    m_pagesB = pagesB;

    // NOTE: the running minimum starts at 0 rather than at the first page's
    // corner, so the union always swallows the origin even when every page sits
    // to the right of and below it. Preserved as shipped.
    float minX = 0.0f;
    float minY = 0.0f;
    float maxX = 0.0f;
    float maxY = 0.0f;
    for (unsigned i = 0; i < m_pagesB.size(); ++i)
    {
        auto const& page = m_pagesB[i];
        if (minX > page.x0)
        {
            minX = page.x0;
        }
        if (minY > page.y0)
        {
            minY = page.y0;
        }
        if (i == 0)
        {
            maxX = page.x0 + page.width;
            maxY = page.y0 + page.height;
        }
        else
        {
            if (page.x0 + page.width > maxX)
            {
                maxX = page.x0 + page.width;
            }
            if (page.y0 + page.height > maxY)
            {
                maxY = page.y0 + page.height;
            }
        }
    }
    m_bounds.x0 = minX;
    m_bounds.y0 = minY;
    m_bounds.width = maxX - minX;
    m_bounds.height = maxY - minY;

    m_btnNext = btnNext;
    m_btnPrev = btnPrev;
    if (m_btnNext && m_btnPrev)
    {
        UpdateNextPrevButtonsState();
    }
    return m_btnNext && m_btnPrev;
}

void PagedList::SetSpace(float space)
{
    m_space = space;
}

int PagedList::ShowNext()
{
    // RVA 0x4F6510
    int const count = static_cast<int>(m_items.size());
    if (m_lastVisibleIdx >= count - 1)
    {
        return 0;
    }

    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        if (IsDirectChild(m_items[i]))
        {
            RemoveChild(m_items[i]);
        }
    }

    int idx = m_lastVisibleIdx + 1;
    m_firstVisibleIdx = idx;
    for (; idx < count; ++idx)
    {
        WndItem* item = m_items[idx];
        if (!PlaceItemAtPageIdx(item, item->GetIdx() - m_firstVisibleIdx))
        {
            break;
        }
        m_lastVisibleIdx = idx;
    }

    UpdateNextPrevButtonsState();
    return 1;
}

int PagedList::ShowPrev()
{
    // RVA 0x4F6650
    if (m_firstVisibleIdx <= 0)
    {
        return 0;
    }

    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        if (IsDirectChild(m_items[i]))
        {
            RemoveChild(m_items[i]);
        }
    }

    int idx = m_firstVisibleIdx - 1;
    m_lastVisibleIdx = idx;
    for (; idx >= 0; --idx)
    {
        // Each step tries to fit one more item above the ones already shown.
        if (!PlaceItemAtBegin(m_items[idx]))
        {
            break;
        }
        m_firstVisibleIdx = idx;
    }

    UpdateNextPrevButtonsState();
    return 1;
}

void PagedList::ShowLast()
{
    // RVA 0x4F6760
    while (ShowNext())
    {
    }
}

int PagedList::PlaceItemAtEnd(WndItem* item)
{
    // RVA 0x4F60D0
    return PlaceItemAtPageIdx(item, item->GetIdx() - m_firstVisibleIdx);
}

int PagedList::PlaceItemAtBegin(WndItem* item)
{
    // RVA 0x4F60F0 - tries to re-flow [item .. m_lastVisibleIdx] from the top of
    // the first page. If they do not all fit, the whole attempt is undone and the
    // previously visible set is laid out again without `item`.
    int const firstIdx = item->GetIdx();
    int const count = m_lastVisibleIdx - firstIdx + 1;

    for (int i = firstIdx; i < firstIdx + count; ++i)
    {
        if (IsDirectChild(m_items[i]))
        {
            RemoveChild(m_items[i]);
        }
    }

    int placed = 0;
    int i = firstIdx;
    for (int slot = 0; slot < count; ++slot)
    {
        placed = PlaceItemAtPageIdx(m_items[i++], slot);
        if (!placed)
        {
            break;
        }
    }
    if (placed)
    {
        return placed;
    }

    for (int j = firstIdx; j < i; ++j)
    {
        if (IsDirectChild(m_items[j]))
        {
            RemoveChild(m_items[j]);
        }
    }
    for (int slot = 0; slot < count - 1; ++slot)
    {
        PlaceItemAtPageIdx(m_items[firstIdx + 1 + slot], slot);
    }
    return placed;
}

int PagedList::PlaceItemAtPageIdx(WndItem* item, int pageIdx)
{
    // RVA 0x4F6270 - the first page with room for it wins.
    int result = 0;
    for (unsigned page = 0; page < m_pagesB.size(); ++page)
    {
        int itemsOnPage = 0;
        result = PlaceItemOnPage(item, pageIdx, static_cast<int>(page), itemsOnPage);
        if (result)
        {
            break;
        }
    }
    return result;
}

int PagedList::PlaceItemOnPage(WndItem* item, int pageIdx, int page, int& itemsOnPage)
{
    // RVA 0x4F62C0. NOTE: itemsOnPage is never written - the parameter is
    // vestigial in the shipped build, and its one caller passes an
    // uninitialised local.
    (void)itemsOnPage;

    auto const& pageB = m_pagesB[page];
    float const itemX0 = pageB.x0;
    float const itemX1 = pageB.x0 + pageB.width;

    // Slot 0 starts at the top of the page; anything after it stacks under the
    // item before it in the list, wherever that one ended up.
    float itemY0;
    if (pageIdx)
    {
        auto const prevB = m_items[item->GetIdx() - 1]->GetBounds();
        itemY0 = prevB.y0 + prevB.height + m_space;
    }
    else
    {
        itemY0 = pageB.y0;
    }

    float const itemY1 = itemY0 + item->GetBounds().height;
    if (itemY1 > pageB.y0 + pageB.height)
    {
        return 0;
    }

    BoundsBase<float> itemB;
    itemB.x0 = itemX0;
    itemB.y0 = itemY0;
    itemB.width = itemX1 - itemX0;
    itemB.height = itemY1 - itemY0;
    item->SetBounds(itemB, true);
    AddChild(item);
    return 1;
}

void PagedList::UpdateNextPrevButtonsState()
{
    // RVA 0x4F6810 - style bit 2 is the "disabled" flag.
    int const count = static_cast<int>(m_items.size());
    if (m_lastVisibleIdx < count - 1)
    {
        m_btnNext->SetStyle(m_btnNext->GetStyle() & ~2u);
    }
    else
    {
        m_btnNext->SetStyle(m_btnNext->GetStyle() | 2u);
    }
    if (m_firstVisibleIdx <= 0)
    {
        m_btnPrev->SetStyle(m_btnPrev->GetStyle() | 2u);
    }
    else
    {
        m_btnPrev->SetStyle(m_btnPrev->GetStyle() & ~2u);
    }
}
