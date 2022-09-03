#include <ui/menu.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(MenuWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(MenuWnd);

        Object* MenuWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        Class* MenuWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        Class* MenuWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        void MenuWnd::SetMenu(Item*)
        {
            throw std::logic_error("Not implemented");
        }

        int MenuWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int MenuWnd::OnInitModal()
        {
            throw std::logic_error("Not implemented");
        }

        MenuWnd::~MenuWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int MenuWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        int MenuWnd::OnMouseOut()
        {
            throw std::logic_error("Not implemented");
        }

        int MenuWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        Object* MenuWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int MenuWnd::CanClose()
        {
            throw std::logic_error("Not implemented");
        }

        void MenuWnd::UpdateItems(Item*)
        {
            throw std::logic_error("Not implemented");
        }

        void MenuWnd::RecalcLayout()
        {
            throw std::logic_error("Not implemented");
        }

        MenuWnd::MenuWnd(MenuWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        MenuWnd::MenuWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int MenuWnd::StorePathToItem(std::list<Item*>&, Item*, Item*)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
