#include <ui/menu.h>
#include <m3dapp.h>
#include <core/aiparam.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(MenuWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(MenuWnd);

        namespace
        {
            // Item colours: the menu bar (lit while the mouse is over it) and the opened menus (the item under the
            // mouse is highlighted).
            unsigned int const BAR_COLOR = 0xFF505050;
            unsigned int const BAR_HOVER_COLOR = 0xFF8F4F00;
            unsigned int const ITEM_COLOR = 0xFF7F7F7F;
            unsigned int const ITEM_HOVER_COLOR = 0xFF008F00;
        }  // namespace

        MenuWnd::Item::Item(CStr const& strId, int cmd, Item* child, Item* next)
        {
            // RVA 0x79B850 - NOTE: only the first child gets its parent set.
            m_next = next;
            m_child = child;
            m_strId = strId;
            m_cmd = cmd;
            m_parent = nullptr;
            if (m_child)
            {
                m_child->m_parent = this;
            }
        }

        Object* MenuWnd::CreateObject()
        {
            // RVA 0x79C430
            return new MenuWnd();
        }

        Class* MenuWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ModalWnd);
        }

        Class* MenuWnd::GetClass() const
        {
            // RVA 0x79B6C0
            return RT_CLASS_LOCAL(MenuWnd);
        }

        void MenuWnd::SetMenu(Item* menu)
        {
            // RVA 0x79C460 - the items hang under an unnamed root. NOTE: the previous root is not freed.
            Item* const root = new Item(CStr(""), 0, nullptr, nullptr);
            root->m_drawRect.x0 = 0.0f;
            root->m_drawRect.y0 = 0.0f;
            root->m_drawRect.width = 0.0f;
            root->m_drawRect.height = 0.0f;
            root->m_child = menu;
            m_menu = root;
            RecalcLayout();
        }

        int MenuWnd::OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas)
        {
            // RVA 0x79C500 - while open, the path to the item under the mouse is opened and its submenu shown.
            if (GetStation()->IsModal(this))
            {
                Item* itemUnderMouse = nullptr;
                for (auto it = m_menuItemsStack.begin(); it != m_menuItemsStack.end() && !itemUnderMouse; ++it)
                {
                    for (Item* child = (*it)->m_child; child; child = child->m_next)
                    {
                        BoundsBase<float> const& r = child->m_drawRect;
                        if (pt.x >= r.x0 && r.width + r.x0 > pt.x && pt.y >= r.y0 && r.height + r.y0 > pt.y)
                        {
                            itemUnderMouse = child;
                            break;
                        }
                    }
                }
                if (itemUnderMouse != m_itemUnderMouse)
                {
                    m_menuItemsStack.clear();
                    m_menuItemsStack.push_back(m_menu);
                    if (itemUnderMouse)
                    {
                        StorePathToItem(m_menuItemsStack, m_menu->m_child, itemUnderMouse);
                        if (itemUnderMouse->m_child)
                        {
                            m_menuItemsStack.push_back(itemUnderMouse);
                            UpdateItems(itemUnderMouse->m_child);
                        }
                    }
                }
                m_itemUnderMouse = itemUnderMouse;
            }
            return Wnd::OnMouseMove(pt, deltas);
        }

        int MenuWnd::OnInitModal()
        {
            // RVA 0x79C450
            RecalcLayout();
            return 1;
        }

        MenuWnd::~MenuWnd()
        {
            // RVA 0x79BF00
        }

        int MenuWnd::OnPaint(DrawInfo const& di)
        {
            // RVA 0x79BF40 - closed, only the bar is drawn; open, every level on the path.
            if (!GetStation()->IsModal(this))
            {
                for (Item* item = m_menuItemsStack.front()->m_child; item; item = item->m_next)
                {
                    m_gfx->AddFlatAxialQuad(di, item->m_drawRect, m_mouseOver ? BAR_HOVER_COLOR : BAR_COLOR);
                    PointBase<float> const at(item->m_drawRect.x0 + 2.0f, item->m_drawRect.y0);
                    m_gfx->AddText(di, at, item->m_caption, 0, TW_NOWRAP, TF_LEFT);
                }
                return 1;
            }

            for (Item* level : m_menuItemsStack)
            {
                for (Item* item = level->m_child; item; item = item->m_next)
                {
                    m_gfx->AddFlatAxialQuad(di, item->m_drawRect, m_itemUnderMouse == item ? ITEM_HOVER_COLOR : ITEM_COLOR);
                    PointBase<float> const at(item->m_drawRect.x0 + 2.0f, item->m_drawRect.y0);
                    m_gfx->AddText(di, at, item->m_caption, 0, TW_NOWRAP, TF_LEFT);
                }
            }
            return 1;
        }

        int MenuWnd::OnMouseOut()
        {
            // RVA 0x79B6D0
            m_isHoldDown = false;
            return Wnd::OnMouseOut();
        }

        int MenuWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            // RVA 0x79C660 - pressing opens the menu; releasing closes it and sends the command of the item under the
            // mouse, if it has no submenu.
            if (state == 1)
            {
                if (!GetStation()->IsModal(this))
                {
                    GetStation()->DoModal(this);
                }
            }
            else if (state == 0 && m_isHoldDown)
            {
                OnMouseMove(at, PointBase<float>(0.0f, 0.0f));
                CloseModal(0);
                if (m_itemUnderMouse && !m_itemUnderMouse->m_child)
                {
                    M3D_APP->EnqueueMessage(m_itemUnderMouse->m_cmd, 0, 0, 0, 0, CStr(""), AIParam());
                }
            }
            m_isHoldDown = state != 0;
            return Wnd::OnMouseButton0(state, at);
        }

        Object* MenuWnd::Clone()
        {
            // RVA 0x79C3E0
            return new MenuWnd(*this);
        }

        int MenuWnd::CanClose()
        {
            // RVA 0x79C260 - folds the menu back to the bar.
            m_menuItemsStack.clear();
            m_menuItemsStack.push_back(m_menu);
            UpdateItems(m_menu);
            return 1;
        }

        void MenuWnd::UpdateItems(Item* level)
        {
            // RVA 0x79BC30 - stacks the items of one level under each other, as wide as the widest caption, to the
            // right of the levels already open and level with their parent; then sizes the window to fit.
            float maxWidth = 0.0f;
            float y = 0.0f;
            for (Item* item = level; item; item = item->m_next)
            {
                bool const found = GetStation()->GetStringByStringId(item->m_caption, item->m_strId) != 0;
                PointBase<float> size = m_gfx->MeasureText(item->m_caption, 0, TW_NOWRAP, 0.0f);
                size.x += 8.0f;
                size.y += 2.0f;
                if (!found)
                {
                    size.x = 0.0f;
                    size.y = 0.0f;
                }
                item->m_drawRect.height = size.y;
                item->m_drawRect.x0 = 0.0f;
                item->m_drawRect.y0 = y;
                item->m_drawRect.width = 0.0f;
                y = size.y + y;
                if (size.x > maxWidth)
                {
                    maxWidth = size.x;
                }
            }

            float x = 0.0f;
            for (Item* open : m_menuItemsStack)
            {
                x = open->m_drawRect.width + x;
            }
            // NOTE: an empty level is dereferenced here.
            float const parentY = level->m_parent ? level->m_parent->m_drawRect.y0 : 0.0f;
            for (Item* item = level; item; item = item->m_next)
            {
                item->m_drawRect.width = maxWidth;
                item->m_drawRect.x0 = x;
                item->m_drawRect.y0 = item->m_drawRect.y0 + parentY;
            }

            float right = 0.0f;
            float bottom = 0.0f;
            for (Item* open : m_menuItemsStack)
            {
                for (Item* item = open->m_child; item; item = item->m_next)
                {
                    float const r = item->m_drawRect.width + item->m_drawRect.x0;
                    if (r > right)
                    {
                        right = r;
                    }
                    float const b = item->m_drawRect.height + item->m_drawRect.y0;
                    if (b > bottom)
                    {
                        bottom = b;
                    }
                }
            }
            SetBounds(BoundsBase<float>(0.0f, 0.0f, right, bottom), true);
        }

        void MenuWnd::RecalcLayout()
        {
            // RVA 0x79C2C0 - NOTE: the root is pushed again without clearing the stack first.
            m_menuItemsStack.push_back(m_menu);
            UpdateItems(m_menu);
            UpdateItems(m_menu->m_child);
        }

        MenuWnd::MenuWnd(MenuWnd const& mw) : ModalWnd(mw)
        {
            // RVA 0x79C120 - NOTE: the menu itself is not copied.
        }

        MenuWnd::MenuWnd()
        {
            // RVA 0x79C1C0
            BoundsBase<float> const rc(0.0f, 0.0f, 0.0f, 0.0f);
            CreateWnd(CStr(""), 0, rc, 0);
            m_itemUnderMouse = nullptr;
            m_paneFlags = 0;
            m_isHoldDown = false;
            m_style = 0x1200;
        }

        int MenuWnd::StorePathToItem(std::list<Item*>& il, Item* item, Item* toFind)
        {
            // RVA 0x79C320 - appends the items leading down to toFind (not toFind itself).
            for (Item* it = item; it; it = it->m_next)
            {
                if (it == toFind)
                {
                    return 1;
                }
                il.push_back(it);
                if (StorePathToItem(il, it->m_child, toFind))
                {
                    return 1;
                }
                il.pop_back();
            }
            return 0;
        }
    }
}
