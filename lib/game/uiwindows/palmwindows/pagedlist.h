#pragma once
#include <ui/ui.h>

class WndItem : public m3d::ui::Wnd
{
public:
    WndItem(WndItem const&);
    WndItem();
    virtual ~WndItem() override;
    int GetIdx() const;
    void SetIdx(int idx);

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    int m_idx;
};

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
    virtual ~PagedList() override;
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
        override;
    int PlaceItemAtBegin(WndItem* item);
    int PlaceItemAtEnd(WndItem* item);
    int PlaceItemOnPage(WndItem* item, int pageIdx, int page, int& itemsOnPage);
    int PlaceItemAtPageIdx(WndItem* item, int pageIdx);
    void UpdateNextPrevButtonsState();

    using ItemsVector = std::vector<WndItem*, std::allocator<WndItem*>>;

protected:
    std::vector<WndItem*, std::allocator<WndItem*>> m_items;
    int m_firstVisibleIdx;
    int m_lastVisibleIdx;
    std::vector<BoundsBase<float>, std::allocator<BoundsBase<float>>> m_pagesB;
    float m_space;
    m3d::ui::ButtonWnd* m_btnNext;
    m3d::ui::ButtonWnd* m_btnPrev;
};