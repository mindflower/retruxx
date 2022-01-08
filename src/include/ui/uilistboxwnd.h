#pragma once
#include "ui.h"

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
            //SetDrawFlags(unsigned int);
            //SetScrollPane(T const &);
            //GetItemData(int);
            //WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            //SetPaneFlags(int);
            //RecalcNcLayout();
            //GetCount();
            //OnPaint(DrawInfo const &);
            //Create(T const &, unsigned int,BoundsBase<float> const &, unsigned int);
            //Create(T const &, unsigned int,BoundsBase<float> const &, unsigned int);
            //SetCurSel(int);
            //ScrollSelection(bool);
            //GetDrawFlags();
            //RecalcLayout();
            //ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            //OnMouseWheel(int,PointBase<float> const &);
            //SetItemData(int,int);
            //~ListBoxWnd();
            //ListBoxWnd();

        private:
            std::vector<ListBoxWnd<T>::Item> m_items;
            int m_curSel;
            unsigned int m_drawFlags;
        };
    }
}
