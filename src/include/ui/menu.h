#pragma once
#include "wnd.h"
#include <list>

namespace m3d
{
    namespace ui
    {
        class MenuWnd : public ModalWnd
        {
        public:
            class Item
            {
            public:
                Item(CStr const&, int, Item*, Item*);

            private:
                Item* m_next;
                Item* m_child;
                Item* m_parent;
                CStr m_strId;
                CStr m_caption;
                int m_cmd;
                BoundsBase<float> m_drawRect;
            };

        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            virtual Class* GetClass() const;
            void SetMenu(Item*);
            virtual int OnMouseMove(PointBase<float> const&, PointBase<float> const&);
            virtual int OnInitModal();
            virtual ~MenuWnd();
            virtual int OnPaint(DrawInfo const&);
            virtual int OnMouseOut();
            virtual int OnMouseButton0(unsigned int, PointBase<float> const&);
            virtual Object* Clone();
            virtual int CanClose();

        protected:
            void UpdateItems(Item*);
            void RecalcLayout();
            MenuWnd(MenuWnd const&);
            MenuWnd();
            int StorePathToItem(std::list<Item*>&, Item*, Item*);

        public:
            RT_CLASS_DECLARE(MenuWnd);

        private:
            bool m_isHoldDown;
            std::list<Item*> m_menuItemsStack;
            Item* m_itemUnderMouse;
            Item* m_menu;
        };
    }
}
