#pragma once
#include "frame.h"
#include "scroll.h"
#include "ui.h"
#include "ui_srv.h"
#include <core/kernel.h>

#include "core/aiparam.h"
#include "core/ini.h"

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

            BoundsBase<float> GetItemBounds(int idx) const
            {
                BoundsBase<float> b(0.0, 0.0, 0.0, 0.0);
                if (idx < 0 || idx >= m_items.size())
                {
                    return b;
                }

                MeasureItem(idx, b);

                float curPosV = 0.0;
                if (m_scrollVWnd)
                {
                    curPosV = m_scrollVWnd->GetCurPos();
                }

                float curPosH = 0.0;
                if (m_scrollHWnd)
                {
                    curPosH = m_scrollHWnd->GetCurPos();
                }

                const auto& item = m_items[idx];
                BoundsBase<float> result;
                result.x0 = item.m_origin.x - curPosH + b.x0;
                result.width = b.width;
                result.y0 = item.m_origin.y - curPosV + b.y0;
                result.height = b.height;
                return result;
            }

            T GetItem(int idx) const
            {
                return m_items[idx].m_item;
            }

            void SetItem(int idx, T const& item)
            {
                m_items[idx].m_item = item;
                m_items[idx].m_rectValid = 0;
            }

            int ItemFromPoint(PointBase<float> const& pt)
            {
                RecalcLayout();
                float curPosH = 0.0;
                if (m_scrollHWnd)
                {
                    curPosH = m_scrollHWnd->GetCurPos();
                }
                float curPosV = 0.0;
                if (m_scrollVWnd)
                {
                    curPosV = m_scrollVWnd->GetCurPos();
                }
                float const clientW = GetClientBounds().width;
                for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
                {
                    auto const& item = m_items[i];
                    BoundsBase<float> hit;
                    hit.x0 = item.m_origin.x - curPosH + item.m_rect.x0;
                    hit.y0 = item.m_origin.y - curPosV + item.m_rect.y0;
                    hit.width = clientW;
                    hit.height = item.m_rect.height;
                    if (hit.IsPtInBounds(pt))
                    {
                        return i;
                    }
                }
                return -1;
            }

            virtual void SetBounds(BoundsBase<float> const& rc, bool bUpdateBaseOrigin)
            {
                m_bounds = rc;
                if (bUpdateBaseOrigin)
                {
                    m_baseOrigin.x = m_bounds.x0;
                    m_baseOrigin.y = m_bounds.y0;
                }
                RecalcNcLayout();
            }

            virtual void SetClientEdges(retruxx::vector<float> const& clientEdges)
            {
                m_clientEdges = clientEdges;
                RecalcNcLayout();
            }

            virtual void SetClientEdges(float left, float top, float right, float bottom)
            {
                Wnd::SetClientEdges(left, top, right, bottom);
                RecalcNcLayout();
            }

            virtual void SetPane(CStr const& name)
            {
                if (!name.empty())
                {
                    m_paneName = name;
                }
                RecalcNcLayout();
            }

            virtual void SetScrollPane(CStr const& name)
            {
                Wnd::SetScrollPane(name);
                RecalcNcLayout();
            }

            virtual void SetPaneFlags(int flags)
            {
                m_paneFlags = flags;
                RecalcNcLayout();
            }

            // Shadows Wnd::GetClientBounds (same vtable slot): temporarily widens the right
            // client edge by the vertical scrollbar width so the client area excludes it.
            BoundsBase<float> GetClientBounds() const override
            {
                auto* self = const_cast<ListBoxWnd<T>*>(this);
                retruxx::vector<float> const saved = m_clientEdges;
                float scrollW = 0.0;
                if (m_scrollVWnd)
                {
                    scrollW = m_scrollVWnd->GetBounds().width;
                }
                self->m_clientEdges[2] = saved[0] + saved[2] + scrollW;
                BoundsBase<float> const r = Wnd::GetClientBounds();
                self->m_clientEdges = saved;
                return r;
            }

            float GetScrollWidth() const
            {
                return m_scrollVWnd ? m_scrollVWnd->GetBounds().width : 0.0f;
            }

            virtual int GetTopVisibleItemId() const
            {
                auto const client = GetClientBounds();
                for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
                {
                    if (!client.Intersect(GetItemBounds(i)).Empty())
                    {
                        return i;
                    }
                }
                return -1;
            }

            virtual int GetBottomVisibleItemId() const
            {
                auto const client = GetClientBounds();
                for (int i = static_cast<int>(m_items.size()) - 1; i >= 0; --i)
                {
                    auto const ib = GetItemBounds(i);
                    float const ibRight = ib.x0 + ib.width;
                    float const ibBottom = ib.y0 + ib.height;
                    float const clientRight = client.x0 + client.width;
                    float const clientBottom = client.y0 + client.height;
                    float const xLo = ib.x0 > client.x0 ? ib.x0 : client.x0;
                    float const xHi = ibRight < clientRight ? ibRight : clientRight;
                    float const yLo = ib.y0 > client.y0 ? ib.y0 : client.y0;
                    float const yHi = ibBottom < clientBottom ? ibBottom : clientBottom;
                    if (xHi - xLo > 0.0f && yHi - yLo > 0.0f)
                    {
                        return i;
                    }
                }
                return -1;
            }

            virtual int ScrollList(bool toStart)
            {
                if (!m_scrollVWnd)
                {
                    return 0;
                }
                float delta;
                if (toStart)
                {
                    delta = -GetItemBounds(GetTopVisibleItemId()).height;
                }
                else
                {
                    delta = GetItemBounds(GetBottomVisibleItemId()).height;
                }
                m_scrollVWnd->SetCurPos(m_scrollVWnd->GetCurPos() + delta);
                return 1;
            }

            virtual int ScrollSelection(bool toStart)
            {
                int const n = static_cast<int>(m_items.size());
                if (n <= 0)
                {
                    return 0;
                }
                int idx = toStart ? m_curSel - 1 : m_curSel + 1;
                if (idx < 0)
                {
                    idx = 0;
                }
                if (idx > n - 1)
                {
                    idx = n - 1;
                }
                SetCurSel(idx);
                return 1;
            }

            virtual int Scroll(bool toStart)
            {
                if ((m_drawFlags & 4) != 0)
                {
                    return ScrollList(toStart);
                }
                return ScrollSelection(toStart);
            }

            int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override
            {
                unsigned const k = key & 0xff;
                if (k == 0x1b || k == 0x1c)
                {
                    if (state != 0)
                    {
                        Scroll(k == 0x1b);
                    }
                }
                return 1;
            }

            int OnMouseWheel(int ticks, PointBase<float> const& at) override
            {
                if (ticks != 0)
                {
                    Scroll(ticks > 0);
                }
                return Wnd::OnMouseWheel(ticks, at);
            }

            int OnMouseButton0(unsigned int state, PointBase<float> const& at) override
            {
                if (state != 0)
                {
                    int const idx = ItemFromPoint(at);
                    if (idx >= 0)
                    {
                        SetCurSel(idx);
                    }
                }
                return Wnd::OnMouseButton0(state, at);
            }

            int OnMouseDblClick(PointBase<float> const& firstClickPt, PointBase<float> const& secondClickPt) override
            {
                int const i1 = ItemFromPoint(firstClickPt);
                int const i2 = ItemFromPoint(secondClickPt);
                if (i1 != -1 && i2 != -1 && i1 == i2)
                {
                    return Wnd::OnMouseDblClick(firstClickPt, secondClickPt);
                }
                return 0;
            }

            int ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node) override
            {
                if (!Wnd::ReadFromXmlNode(file, node))
                {
                    return 0;
                }
                SafeUintAttrib(m_drawFlags, node, "drawFlags");
                return 1;
            }

            int RemoveAllItems()
            {
                while (!m_items.empty())
                {
                    RemoveItem(m_items.size() - 1);
                }
                return 1;
            }
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
            int InsertItem(T const& item, int idx)
            {
                auto const size = static_cast<int>(m_items.size());
                if (idx < 0 || idx > size)
                {
                    idx = size;
                }
                Item ii;
                ii.m_item = item;
                ii.m_data = 0;
                ii.m_rectValid = 0;
                m_items.insert(m_items.begin() + idx, ii);
                for (int i = idx; i < static_cast<int>(m_items.size()); ++i)
                {
                    m_items[i].m_rectValid = 0;
                }
                RecalcLayout();
                return idx;
            }

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

            // Item paint / sort hooks (vtable 0x138 / 0x140 in the shipped game).
            // The template's own layout code does not call these yet; subclasses
            // override them.
            virtual int RenderItem(int /*itemIdx*/, PointBase<float> const& /*org*/, m3d::ui::DrawInfo const& /*di*/)
            {
                return 1;
            }

            virtual int CompareItem(int /*itemIdx0*/, int /*itemIdx1*/)
            {
                return 0;
            }

            int GetCurSel() const
            {
                return m_curSel;
            }
            void SetDrawFlags(unsigned int flags)
            {
                m_drawFlags = flags;
            }

            int GetItemData(int idx) const
            {
                return m_items[idx].m_data;
            }

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
                    res.y0 = clientRc.y0;
                    res.width = scrollSz;
                    res.height = clientRc.height;
                    m_scrollVWnd->SetBounds(res, true);
                }
            }

            int GetCount() const
            {
                return m_items.size();
            }
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
                for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
                {
                    auto& item = m_items[i];
                    if (!item.m_rectValid)
                    {
                        MeasureItem(i, item.m_rect);
                        item.m_origin.y = orgY;
                        item.m_origin.x = 0.0;
                        item.m_rectValid = 1;
                    }
                    orgY = item.m_rect.height + orgY;
                    if (item.m_rect.width > maxX)
                    {
                        maxX = item.m_rect.width;
                    }
                }
                if (m_scrollVWnd)
                {
                    m_scrollVWnd->SetScrollRect(maxX, orgY);
                    auto bounds = m_scrollVWnd->GetBounds();
                    bool show = (bounds.height + 0.001) < orgY;
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
                    auto bounds = m_scrollHWnd->GetBounds();
                    bool show = (bounds.width + 0.001) < maxX;
                    if ((m_drawFlags & 2) != 0)
                    {
                        m_scrollHWnd->ShowWindow(show);
                    }
                    else
                    {
                        m_scrollHWnd->EnableWindow(show);
                    }
                }
            }

            virtual int MeasureItem(int, BoundsBase<float>&) const
            {
                RETRUXX_NOT_IMPLEMENTED;
            }

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
