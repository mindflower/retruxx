#pragma once
#include <ui/ui.h>

class WndItem : public m3d::ui::Wnd
{
public:
    WndItem(WndItem const&);
    WndItem();
    virtual ~WndItem() override /* 0x00 */;
    int GetIdx() const;
    void SetIdx(int idx);

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    /* 0x0220 */ int m_idx;
}; /* size: 0x0224 */

namespace m3d::ui
{
    class ButtonWnd;
}

class PagedList : public m3d::ui::Wnd
{
public:
    using BoundsVector = std::vector<BoundsBase<float>, std::allocator<BoundsBase<float>>>;

public:
    PagedList(PagedList const&);
    PagedList();
    virtual ~PagedList() override /* 0x00 */;
    int AddItem(WndItem* item);
    int RemoveItem(WndItem* item);
    int SetUp(
        std::vector<BoundsBase<float>, std::allocator<BoundsBase<float>>> const& pagesB,
        m3d::ui::ButtonWnd* btnNext,
        m3d::ui::ButtonWnd* btnPrev);
    void SetSpace(float space);
    int ShowNext();
    int ShowPrev();
    void ShowLast();
    int ClearItems();

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    int PlaceItemAtBegin(WndItem* item);
    int PlaceItemAtEnd(WndItem* item);
    int PlaceItemOnPage(WndItem* item, int pageIdx, int page, int& itemsOnPage);
    int PlaceItemAtPageIdx(WndItem* item, int pageIdx);
    void UpdateNextPrevButtonsState();

    using ItemsVector = std::vector<WndItem*, std::allocator<WndItem*>>;

protected:
    /* 0x0220 */ std::vector<WndItem*, std::allocator<WndItem*>> m_items;
    /* 0x0230 */ int m_firstVisibleIdx;
    /* 0x0234 */ int m_lastVisibleIdx;
    /* 0x0238 */ std::vector<BoundsBase<float>, std::allocator<BoundsBase<float>>> m_pagesB;
    /* 0x0248 */ float m_space;
    /* 0x024c */ m3d::ui::ButtonWnd* m_btnNext;
    /* 0x0250 */ m3d::ui::ButtonWnd* m_btnPrev;
}; /* size: 0x0254 */