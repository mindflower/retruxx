#pragma once
#include "frame.h"
#include "scroll.h"
#include "ui.h"
#include "ui_srv.h"
#include <core/kernel.h>

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
            protected:
            private:
                CStr m_item;
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
            //GetItem(int);
            //ScrollList(bool);
            //SetClientEdges(float,float,float,float);
            //SetClientEdges(float,float,float,float);
            //SetBounds(BoundsBase<float> const &,bool);
            //RemoveAllItems();
            //OnKey(unsigned short,unsigned char, unsigned int);
            //OnMouseButton0(uint,PointBase<float> const &);
            //AddItem(T const &);
            //GetScrollWidth();
            //Scroll(bool);
            //GetClientBounds();
            //SetItem(int,T const &);
            //GetBottomVisibleItemId();
            //InsertItem(T const &,int);
            //RemoveItem(int);
            //GetCurSel();
            //OnMouseDblClick(PointBase<float> const &,PointBase<float> const &);
            //SetPane(CStr const &);  //Type??
            void SetDrawFlags(unsigned int flags)
            {
                m_drawFlags = flags;
            }
            //SetScrollPane(T const &);
            //GetItemData(int);
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
            //GetCount();
            //OnPaint(DrawInfo const &);
            //Create(T const &, unsigned int,BoundsBase<float> const &, unsigned int);
            //Create(T const &, unsigned int,BoundsBase<float> const &, unsigned int);
            //SetCurSel(int);
            //ScrollSelection(bool);
            unsigned GetDrawFlags()
            {
                return m_drawFlags;
            }
            //RecalcLayout();
            //ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            //OnMouseWheel(int,PointBase<float> const &);
            //SetItemData(int,int);
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
