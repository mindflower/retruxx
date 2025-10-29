#pragma once
#include "frame.h"
#include "scroll.h"
#include "ui.h"
#include "ui_srv.h"
#include <core/kernel.h>

#include "core/aiparam.h"

namespace m3d
{
    namespace ui
    {
        template<class T>
        class ListBoxWnd : public Wnd
        {
        public:
            class Item
            {
            public:
                T m_item;
                int m_data;
                BoundsBase<float> m_rect;
                int m_rectValid;
                PointBase<float> m_origin;
            };

        public:
            virtual int Create(BoundsBase<float> const& rc, unsigned int flags, unsigned int id)
            {
                if (!flags)
                {
                    flags = 406080;
                }
                if (CreateWnd({}, flags, rc, id) == 0)
                {
                    return 0;
                }
                BoundsBase<float> scrollBounds{0.0, 0.0, 0.0, 0.0};
                m_scrollVWnd = dynamic_cast<ScrollWnd*>(g_Kernel->New("ScrollWnd"));
                if (m_scrollVWnd)
                {
                    if (auto pane = GetGfxServer()->GetScrollPane(m_scrollPaneName))
                    {
                        scrollBounds.width = pane->GetWidth();
                    }
                }
                m_scrollVWnd->Create(scrollBounds, 1);
                m_scrollVWnd->SetScrollPane(m_scrollPaneName);
                AddChild(m_scrollVWnd);
                RecalcNcLayout();
                return 1;
            }

            virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id)
            {
                return Create(rc, style, id);
            }
            //ItemFromPoint(PointBase<float> const &);
            //GetTopVisibleItemId();
            //GetItemBounds(int);
            T GetItem(int idx) const
            {
                return m_items[idx].m_item;
            }
            //ScrollList(bool);
            //SetClientEdges(float,float,float,float);
            //SetClientEdges(float,float,float,float);
            //SetBounds(BoundsBase<float> const &,bool);
            int RemoveAllItems()
            {
                while (!m_items.empty())
                {
                    RemoveItem(m_items.size() - 1);
                }
                return 1;
            }
            //OnKey(unsigned short,unsigned char, unsigned int);
            //OnMouseButton0(uint,PointBase<float> const &);
            int AddItem(T const& item)
            {
                Item ii;
                ii.m_item = item;
                ii.m_data = 0;
                ii.m_rectValid = 0;
                m_items.push_back(ii);
                int res = m_items.size() - 1;
                RecalcLayout();
                return res;
            }
            //GetScrollWidth();
            //Scroll(bool);
            //GetClientBounds();
            //SetItem(int,T const &);
            //GetBottomVisibleItemId();
            //InsertItem(T const &,int);
            int RemoveItem(int idx)
            {
                DeleteItem(idx);
                m_items.erase(m_items.begin() + idx);
                auto newIdx = idx;
                if (idx >= m_items.size())
                {
                    newIdx = idx - 1;
                    SetCurSel(newIdx);
                }
                for (int i = newIdx; i < m_items.size(); ++i)
                {
                    m_items[i].m_rectValid = 0;
                }
                RecalcLayout();
                return 1;
            }

            virtual int DeleteItem(int) = 0;

            int GetCurSel() const
            {
                return m_curSel;
            }
            //OnMouseDblClick(PointBase<float> const &,PointBase<float> const &);
            //SetPane(CStr const &);  //Type??
            void SetDrawFlags(unsigned int flags)
            {
                m_drawFlags = flags;
            }
            //SetScrollPane(T const &);
            int GetItemData(int idx) const
            {
                return m_items[idx].m_data;
            }
            //WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            //SetPaneFlags(int);
            void RecalcNcLayout()
            {
                if (m_scrollVWnd)
                {
                    float scrollSz = 0.0;
                    if (auto pane = GetGfxServer()->GetScrollPane(m_scrollPaneName))
                    {
                        scrollSz = pane->GetWidth();
                    }
                    auto rc = GetBounds();
                    auto clientRc = GetClientBounds();
                    BoundsBase<float> res;
                    res.x0 = (rc.width - clientRc.x0) - scrollSz;
                    res.y0 = clientRc.height;
                    res.width = scrollSz;
                    res.height = clientRc.height;
                    m_scrollVWnd->SetBounds(res, true);
                }
            }

            int GetCount() const
            {
                return m_items.size();
            }
            //OnPaint(DrawInfo const &);
            //Create(T const &, unsigned int,BoundsBase<float> const &, unsigned int);
            //Create(T const &, unsigned int,BoundsBase<float> const &, unsigned int);
            virtual void SetCurSel(int i)
            {
                if (i >= -1)
                {
                    auto const itemsSize = static_cast<int>(m_items.size());
                    if (i < itemsSize)
                    {
                        auto const oldSel = m_curSel;
                        if ((m_style & 0x40000) != 0)
                        {
                            AIParam const param{CVector2{static_cast<float>(oldSel), static_cast<float>(i)}};
                            CallParentNotify(6u, param, true);
                        }
                        m_curSel = i;
                        if (i == -1)
                        {
                            if (m_scrollVWnd)
                            {
                                m_scrollVWnd->SetCurPos(0.0);
                            }
                        }
                        else
                        {
                            //TODO: check this and refactor
                            auto pt = GetOriginPoint();
                            auto bounds = GetClientBounds();
                            auto v8 = m_items[i].m_rect.y0 + m_items[i].m_origin.y;
                            auto v10 = m_items[i].m_rect.height;
                            auto v11 = v8 + (0.0 - pt.y);
                            auto v12 = bounds.height * 0.5;
                            auto v13 = (bounds.height - v12) * 0.5;
                            if (v13 > (v10 + v11) || v11 > (v13 + v12))
                            {
                                if (m_scrollVWnd)
                                    m_scrollVWnd->SetCurPos(m_items[i].m_origin.y - v12);
                            }
                        }
                        if ((m_style & 0x40000) != 0)
                        {
                            AIParam const param{CVector2{static_cast<float>(oldSel), static_cast<float>(m_curSel)}};
                            CallParentNotify(5u, param, false);
                        }
                    }
                }
            }
            //ScrollSelection(bool);
            unsigned GetDrawFlags()
            {
                return m_drawFlags;
            }

            void RecalcLayout()
            {
                float orgY = 0.0;
                float maxX = 0.0;
                int i = 0;
                for (auto& item : m_items)
                {
	                if (!item.m_rectValid)
	                {
                        MeasureItem(i, item.m_rect);
                        item.m_origin.y = orgY;
                        item.m_origin.x = 0.0;
                        item.m_rectValid = 1;
	                }
                    orgY = item.m_rect.height + orgY;
                    //TODO: check this
                    maxX = max(maxX, item.m_rect.width);
                }
                if (m_scrollVWnd)
                {
                    m_scrollVWnd->SetScrollRect(maxX, orgY);
                    auto bounds = m_scrollVWnd->GetBounds();
                    bool show = (bounds.height + 0.001) < maxX;
                    if ((m_drawFlags & 2) != 0)
                    {
                        m_scrollVWnd->ShowWindow(show);
                    }
                    else
                    {
                        m_scrollVWnd->EnableWindow(show);
                    }
                }
                if (m_scrollHWnd)
                {
                    m_scrollHWnd->SetScrollRect(maxX, orgY);
                    auto bounds = m_scrollVWnd->GetBounds();
                    //TODO: check this!!!!!!!!!!
                    bool show = (bounds.width + 0.001) < orgY;
                    if ((m_drawFlags & 2) != 0)
                    {
                        m_scrollVWnd->ShowWindow(show);
                    }
                    else
                    {
                        m_scrollVWnd->EnableWindow(show);
                    }
                }
            }

            virtual int MeasureItem(int, BoundsBase<float>&) const
            {
                RETRUXX_NOT_IMPLEMENTED;
            }

            //ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            //OnMouseWheel(int,PointBase<float> const &);
            void SetItemData(int idx,int data)
            {
                m_items[idx].m_data = data;
                m_items[idx].m_rectValid = 0;
            }

            ~ListBoxWnd()
            {
            }
            ListBoxWnd()
            {
                m_clientEdges[2] = 2.0;
            }

        protected:
            std::vector<Item> m_items;
            int m_curSel = -1;
            unsigned int m_drawFlags = 0;
        };
    }
}
